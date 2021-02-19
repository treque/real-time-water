// The code here is initially from:
// https://bitbucket.org/victorbush/ufl.cap5705.terrain/src/master/

#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Var.h"
#include "NuanceurProg.h"

struct SurfaceNode
{
	float origin[3];
	float width;
	float height;

	unsigned int vaoId;

	int type;  // child #, 0 = root

	//tessellation scale
	float tscale_negx; // negative x edge
	float tscale_posx; // Positive x edge
	float tscale_negz; // Negative z edge
	float tscale_posz; // Positive z edge

	SurfaceNode* parent;
	SurfaceNode* child1;
	SurfaceNode* child2;
	SurfaceNode* child3;
	SurfaceNode* child4;

	SurfaceNode* north;
	SurfaceNode* south;
	SurfaceNode* east;
	SurfaceNode* west;
};

// Sea Info
GLuint  sea_ibo = 0;
GLint   seaSize = 0;

// size of the patch in meters where you stop subdiv a once its w is > cutoff
#define SURFACE_CUTOFF 25

SurfaceNode* surfaceTree;
SurfaceNode* surfaceTreeTail;
int numSurfaceNodes = 0;

///From the main
glm::mat4 sea_M;	    // Model matrix
glm::mat4 sea_V;	    // View matrix
glm::mat4 sea_P;	    // Projection matrix
glm::mat4 sea_MV;	    // Movel view matrix
glm::mat4 sea_MVP;		// Model-view-projection matrix
glm::mat3 sea_N;        // Normal matrix

#define MAX_SURFACE_NODES 500
GLuint vaos[ MAX_SURFACE_NODES ];
GLuint vbos[ MAX_SURFACE_NODES ];
GLsizei nBuffers;


void clearTree()
{
	surfaceTreeTail = surfaceTree;

	glDeleteVertexArrays( nBuffers + 1, vaos );
	glDeleteBuffers( nBuffers + 1, vbos );
	nBuffers = 0;

	memset(surfaceTree, 0, MAX_SURFACE_NODES * sizeof(SurfaceNode));
	numSurfaceNodes = 0;
}

void surfaceInit()
{
    surfaceTree = (SurfaceNode*)malloc(MAX_SURFACE_NODES * sizeof(SurfaceNode));

	glGenBuffers( 1, &sea_ibo );
	unsigned int positions_indexes[] = { 0, 1, 2, 3 };
	seaSize = sizeof( positions_indexes );

	// Indexes
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sea_ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( positions_indexes ), positions_indexes, GL_STATIC_DRAW );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    clearTree();
}

void surfaceShutdown()
{
    free(surfaceTree);
    surfaceTree = NULL;
    surfaceTreeTail = NULL;

	glDeleteVertexArrays( nBuffers + 1, vaos );
	glDeleteBuffers( nBuffers + 1, vbos );
	nBuffers = 0;
}

void createNodeVao( SurfaceNode* node )
{
	float x = node->origin[ 0 ];
	float y = node->origin[ 1 ];
	float z = node->origin[ 2 ];

	float w = node->width;
	float h = node->height;

	float positions[] =
	{ x + w / 2, y, z + h / 2,
	  x + w / 2, y, z - h / 2,
	  x - w / 2, y, z - h / 2,
	  x - w / 2, y, z + h / 2,
	};

	// Generate buffers
	glGenVertexArrays( 1, &vaos[ nBuffers ] );
	glBindVertexArray( vaos[ nBuffers ] );

	glGenBuffers( 1, &vbos[ nBuffers ] );

	// Link buffers and data:
	// Positions
	glBindBuffer( GL_ARRAY_BUFFER, vbos[ nBuffers ] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( positions ), positions, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	// Indexes
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sea_ibo );

	glEnableVertexAttribArray( 0 );

	glBindVertexArray( 0 );
	
	node->vaoId = nBuffers;
	nBuffers++;
}

SurfaceNode* createNode(SurfaceNode* parent, int type, float x, float y, float z, float w, float h)
{
    if (numSurfaceNodes >= MAX_SURFACE_NODES) return NULL;
    numSurfaceNodes++;

    surfaceTreeTail++;
    surfaceTreeTail->type = type;
    surfaceTreeTail->origin[0] = x;
    surfaceTreeTail->origin[1] = y;
    surfaceTreeTail->origin[2] = z;
    surfaceTreeTail->width = w;
    surfaceTreeTail->height = h;
    surfaceTreeTail->tscale_negx = 1.0;
	surfaceTreeTail->tscale_negz = 1.0;
	surfaceTreeTail->tscale_posx = 1.0;
	surfaceTreeTail->tscale_posz = 1.0;
	surfaceTreeTail->parent = parent;
	surfaceTreeTail->north = NULL;
	surfaceTreeTail->south = NULL;
	surfaceTreeTail->east = NULL;
	surfaceTreeTail->west = NULL;

	return surfaceTreeTail;
}

/**
* Determines whether a node should be subdivided based on its distance to the camera.
* Returns true if the node should be subdivided.
*/
GLboolean needsSubdivision(SurfaceNode* node, glm::vec3 cam_position)
{
    float d = abs(sqrt(pow(cam_position.x - node->origin[0], 2.0) + pow(cam_position.z - node->origin[2], 2.0))); // replace with distance and float3s

	// Distance to camera is greater than twice the length of the diagonal
	// from current origin to corner of current square.
	// OR
	// Max recursion level has been hit

    if (d > 2.5 * sqrt(pow(0.5 * node->width, 2.0) + pow(0.5 * node->height, 2.0)) || node->width < SURFACE_CUTOFF)
	{
		return GL_FALSE;
	}

	return GL_TRUE;
}

/**
* Returns true if node is sub-divided. False otherwise.
*/
void divideNode(SurfaceNode *node, glm::vec3 cam_position)
{
	// Subdivide
	float w_new = 0.5 * node->width;
	float h_new = 0.5 * node->height;

	// Create the child nodes
	node->child1 = createNode(node, 1, node->origin[0] - 0.5 * w_new, node->origin[1], node->origin[2] - 0.5 * h_new, w_new, h_new);
	node->child2 = createNode(node, 2, node->origin[0] + 0.5 * w_new, node->origin[1], node->origin[2] - 0.5 * h_new, w_new, h_new);
	node->child3 = createNode(node, 3, node->origin[0] + 0.5 * w_new, node->origin[1], node->origin[2] + 0.5 * h_new, w_new, h_new);
	node->child4 = createNode(node, 4, node->origin[0] - 0.5 * w_new, node->origin[1], node->origin[2] + 0.5 * h_new, w_new, h_new);

	// Assign neighbors
	if (node->type == 1)
	{
		node->east = node->parent->child2;
		node->north = node->parent->child4;
	}
	else if (node->type == 2)
	{
		node->west = node->parent->child1;
		node->north = node->parent->child3;
	}
	else if (node->type == 3)
	{
		node->south = node->parent->child2;
		node->west = node->parent->child4;
	}
	else if (node->type == 4)
	{
		node->south = node->parent->child1;
		node->east = node->parent->child3;
	}

	// Check if each of these four child nodes will be subdivided.
	GLboolean div1, div2, div3, div4;
	div1 = needsSubdivision(node->child1, cam_position);
	div2 = needsSubdivision(node->child2, cam_position);
	div3 = needsSubdivision(node->child3, cam_position);
	div4 = needsSubdivision(node->child4, cam_position);

	if( div1 )
	{
		divideNode( node->child1, cam_position );
	} else
	{
		createNodeVao( node->child1 );
	}

	if( div2 )
	{
		divideNode( node->child2, cam_position );
	} else 
	{
		createNodeVao( node->child2 );
	}
		
	if( div3 )
	{
		divideNode( node->child3, cam_position );
	} else
	{
		createNodeVao( node->child3 );
	}

	if( div4 )
	{
		divideNode( node->child4, cam_position );
	} else
	{
		createNodeVao( node->child4 );
	}
}

void createTree(float x, float y, float z, float width, float height, glm::vec3 cam_position)
{
	clearTree();

	surfaceTree->type = 0; // Root node
	surfaceTree->origin[0] = x;
	surfaceTree->origin[1] = y;
	surfaceTree->origin[2] = z;
	surfaceTree->width = width;
	surfaceTree->height = height;
	surfaceTree->tscale_negx = 1.0;
	surfaceTree->tscale_negz = 1.0;
	surfaceTree->tscale_posx = 1.0;
	surfaceTree->tscale_posz = 1.0;
	surfaceTree->parent = NULL;
	surfaceTree->north = NULL;
	surfaceTree->south = NULL;
	surfaceTree->east = NULL;
	surfaceTree->west = NULL;

	// Recursively subdivide the terrain
	divideNode(surfaceTree, cam_position);
}

/**
* Search for a node in the tree.
* x, z == the point we are searching for (trying to find the node with an origin closest to that point)
* n = the current node we are testing
*/
SurfaceNode* find(SurfaceNode* n, float x, float z)
{
    if (n->origin[0] == x && n->origin[2] == z) return n;

	if (n->child1 == NULL && n->child2 == NULL && n->child3 == NULL && n->child4 == NULL) return n;

	if (n->origin[0] >= x && n->origin[2] >= z && n->child1) return find(n->child1, x, z);
	else if (n->origin[0] <= x && n->origin[2] >= z && n->child2) return find(n->child2, x, z);
	else if (n->origin[0] <= x && n->origin[2] <= z && n->child3) return find(n->child3, x, z);
	else if (n->origin[0] >= x && n->origin[2] <= z && n->child4) return find(n->child4, x, z);

	return n;
}

/**
* Calculate the tessellation scale factor for a node depending on the neighboring patches.
*/
void calcTessScale(SurfaceNode *node)
{
	SurfaceNode *t;

	// Positive Z (north)
	t = find(surfaceTree, node->origin[0], node->origin[2] + 1 + node->width / 2.0);
	if (t->width > node->width)
		node->tscale_posz = 2.0;

	// Positive X (east)
	t = find(surfaceTree, node->origin[0] + 1 + node->width / 2.0, node->origin[2]);
	if (t->width > node->width)
		node->tscale_posx = 2.0;

	// Negative Z (south)
	t = find(surfaceTree, node->origin[0], node->origin[2] - 1 - node->width / 2.0);
	if (t->width > node->width)
		node->tscale_negz = 2.0;

	// Negative X (west)
	t = find(surfaceTree, node->origin[0] - 1 - node->width / 2.0, node->origin[2]);
	if (t->width > node->width)
		node->tscale_negx = 2.0;
}

/**
* Pushes a node (patch) to the GPU to be drawn.
* note: height parameter is here 4but not used. currently only dealing with square terrains (width is used only)
*/
void renderNode(SurfaceNode* node, CNuanceurProg& progNuanceurGazon, glm::vec3 cam_position)
{

	// Calculate the tess scale factor
	calcTessScale(node);

	glm::vec3 t( 0.f, -20.f, 0.f );
    sea_MV = glm::mat4();
    sea_M = glm::translate( t );
    sea_MV = CVar::vue * sea_M;
	glm::mat4 mvp = CVar::projection * CVar::vue * sea_M;

    GLint handle;

	handle = glGetUniformLocation( progNuanceurGazon.getProg(), "Time" );
	glUniform1f( handle, CVar::temps );

    handle = glGetUniformLocation(progNuanceurGazon.getProg(), "V");
    glUniformMatrix4fv(handle, 1, GL_FALSE, &CVar::vue[0][0]);

    handle = glGetUniformLocation(progNuanceurGazon.getProg(), "M");
    glUniformMatrix4fv(handle, 1, GL_FALSE, &sea_M[0][0]);

    handle = glGetUniformLocation(progNuanceurGazon.getProg(), "P");
    glUniformMatrix4fv(handle, 1, GL_FALSE, &CVar::projection[0][0]);

    handle = glGetUniformLocation(progNuanceurGazon.getProg(), "MV");
    glUniformMatrix4fv(handle, 1, GL_FALSE, &sea_MV[0][0]);

	handle = glGetUniformLocation( progNuanceurGazon.getProg(), "MVP" );
	glUniformMatrix4fv( handle, 1, GL_FALSE, &mvp[ 0 ][ 0 ] );

	// Calc normal matrix
    sea_N = glm::mat3(sea_MV);
    sea_N = glm::transpose(sea_N);
	sea_N = glm::inverse(sea_N);

	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "N");
	glUniformMatrix3fv(handle, 1, GL_FALSE, &sea_N[0][0]);

	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "waveSize");
	glUniform1ui( handle, CVar::waveSize );

	// Send patch neighbor edge tess scale factors
	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "tscale_negx");
	glUniform1f(handle, node->tscale_negx);
	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "tscale_negz");
	glUniform1f(handle, node->tscale_negz);
	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "tscale_posx");
	glUniform1f(handle, node->tscale_posx);
	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "tscale_posz");
	glUniform1f(handle, node->tscale_posz);

	handle = glGetUniformLocation(progNuanceurGazon.getProg(), "eyePos");
	glUniform3fv(handle, 1, &cam_position[0]);

	glBindVertexArray( vaos[ node->vaoId ] );
    if( CVar::isSeaGrid )
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glPatchParameteri( GL_PATCH_VERTICES, 4 );
        glDrawElements( GL_PATCHES, 4, GL_UNSIGNED_INT, NULL );
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glPatchParameteri( GL_PATCH_VERTICES, 4 );
        glDrawElements( GL_PATCHES, 4, GL_UNSIGNED_INT, NULL );
    }

	glBindBuffer( GL_ARRAY_BUFFER, NULL );
	glBindVertexArray( NULL );
}


/**
* Traverses the terrain quadtree to draw nodes with no children.
*/
void renderRecursive(SurfaceNode *node, CNuanceurProg& progNuanceurGazon, glm::vec3 cam_position)
{
	// If all children are null, render this node
	if (!node->child1 && !node->child2 && !node->child3 && !node->child4)
	{
		renderNode(node, progNuanceurGazon, cam_position);
		return;
	}

	// Otherwise, recurse to the children.
	// Note: we're checking if the child exists. Theoretically, with our algorithm,
	// either all the children are null or all the children are not null.
	// There shouldn't be any other cases, but we check here for safety.
	if (node->child1)
		renderRecursive(node->child1, progNuanceurGazon, cam_position);
	if (node->child2)
		renderRecursive(node->child2, progNuanceurGazon, cam_position);
	if (node->child3)
		renderRecursive(node->child3, progNuanceurGazon, cam_position);
	if (node->child4)
		renderRecursive(node->child4, progNuanceurGazon, cam_position);
}

/**
* Draw the terrrain.
*/
void renderSea(CNuanceurProg& progNuanceurGazon, glm::vec3 cam_position)
{
	renderRecursive(surfaceTree, progNuanceurGazon, cam_position);
}