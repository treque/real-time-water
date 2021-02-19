#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "OBJParser.h"

using namespace std;

float   g_Yaw	= 0.0f;
float   g_Pitch	= 0.0f;
float   g_Zoom	= 5.0f;
int	  g_MouseX, g_MouseY, g_Button;
float   g_Scale = 10.0f;

vector<Vertex>   g_Vertices;
vector<Triangle> g_Triangles;
vector<Quad>     g_Quads;
CVecteur3        g_ObjCentroid;

void Display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
    float x	= g_Zoom * cos( g_Pitch ) * sin( g_Yaw );
	float y	= g_Zoom * sin( g_Pitch );
	float z	= g_Zoom * cos( g_Pitch ) * cos( g_Yaw );
	
    gluLookAt( x, y, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

    glPushMatrix();
    glScalef( g_Scale, g_Scale, g_Scale );

    glTranslatef( -g_ObjCentroid.x, -g_ObjCentroid.y, -g_ObjCentroid.z );

    if( !g_Triangles.empty() )
        glDrawElements( GL_TRIANGLES, ( int )g_Triangles.size() * 3, GL_UNSIGNED_INT, &g_Triangles[ 0 ] );
    if( !g_Quads.empty() )
        glDrawElements( GL_QUADS, ( int )g_Quads.size() * 3, GL_UNSIGNED_INT, &g_Quads[ 0 ] );

    glPopMatrix();

	glDisable( GL_LIGHTING );
	
    glLineWidth( 2.0f );
    glBegin( GL_LINES );
        // X axis
        glColor3ub( 255, 0, 0 );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 1.0f, 0.0f, 0.0f );

        // Y axis
        glColor3ub( 0, 255, 0 );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 1.0f, 0.0f );

        // Z axis
        glColor3ub( 0, 0, 255 );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 1.0f );
    glEnd();
    glLineWidth( 1.0f );
	
	
	glEnable( GL_LIGHTING );
	
	glutSwapBuffers();
}


void Reshape( int width, int height )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 50.0, (double)width / height, 0.1, 1000.0 );
	glMatrixMode( GL_MODELVIEW );
	glViewport( 0, 0, width, height );
}


void OnMouseButton( int Button, int State, int x, int y )
{
	g_MouseX = x;
	g_MouseY = y;
	g_Button = Button;
}


void OnMouseMove( int x, int y )
{
	float DiffX = g_MouseX - x;
	float DiffY = g_MouseY - y;


	if( g_Button == GLUT_LEFT_BUTTON )
	{
		g_Pitch	+= DiffY * 0.01f;
		g_Yaw	+= DiffX * 0.01f;
	}
	else if( g_Button == GLUT_RIGHT_BUTTON )
	{
		float k;
		if( DiffX + DiffY > 0 )
			k = sqrt( DiffX * DiffX + DiffY * DiffY );
		else
			k = -sqrt( DiffX * DiffX + DiffY * DiffY );

		if( g_Zoom * ( 1 + k * 0.01 ) > 0.0 )
			g_Zoom	*= 1 + k * 0.01;
	}
 
	g_MouseX = x;
	g_MouseY = y;
}


void OnKeyPressed( unsigned char Key, int x, int y )
{
	switch( Key )
	{
	    case 27: // ESC.
            g_Vertices.clear();
            g_Triangles.clear();
            g_Quads.clear();
		    exit( 0 );
	}
}


void Init()
{
	float	a[ 4 ]	=	{ 0.2f, 0.2f, 0.2f, 1.0f };
	float	d[ 4 ]	=	{ 0.3f, 0.3f, 0.9f, 1.0f };
	float	s[ 4 ]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
    float	lp[ 4 ]	=	{ 5.0f, 5.0f, 5.0f, 1.0f };
    

	glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_POSITION, lp );

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, a );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, d );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, s );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 50.0f );

	glShadeModel( GL_SMOOTH );
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable( GL_NORMALIZE );
}

void Idle( void )
{
	static int waitUntil = glutGet( GLUT_ELAPSED_TIME );
	
	int time = glutGet( GLUT_ELAPSED_TIME );

	if ( time > waitUntil )
	{
		glutPostRedisplay();
		waitUntil = time + ( 1000 / 60 );
	}
}

int	main( int argc, char** argv )
{
    if( argc < 2 )
    {
        cerr << "Pass model.obj file as argument!" << endl;
        system( "pause" );
        exit( 0 );
    }

	glutInit( &argc, argv );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "Obj Parser" );
    glutPositionWindow( 100, 100 );
	Init();

    COBJParser* Parser = new COBJParser();

    if( Parser->OpenFile( argv[ 1 ] ) )
    {
        cerr << "Parsing file : " << argv[ 1 ] << endl;
        Parser->ParseFile();
        Parser->CloseFile();

        g_Vertices    = Parser->GetVertices();
        g_Triangles   = Parser->GetTriangles();
        g_Quads       = Parser->GetQuads();
        g_ObjCentroid = Parser->GetCentroid();

        cerr << "Number of faces : " << ( g_Triangles.size() + g_Quads.size() ) << endl;

        glEnableClientState( GL_VERTEX_ARRAY );
	    glEnableClientState( GL_NORMAL_ARRAY );
        glInterleavedArrays( GL_T2F_N3F_V3F, 0, &g_Vertices[ 0 ] );

        cerr << "Done!" << endl;
    }

    delete Parser;

	glutDisplayFunc( Display );
	glutMouseFunc( OnMouseButton );
	glutMotionFunc( OnMouseMove );
	glutKeyboardFunc( OnKeyPressed );
	glutIdleFunc( Idle );
	glutReshapeFunc( Reshape );
	glutMainLoop();

    // Clean up
    g_Vertices.clear();
    g_Triangles.clear();
    g_Quads.clear();

	return	0;
}
