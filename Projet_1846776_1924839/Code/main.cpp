///////////////////////////////////////////////////////////////////////////////
///  @file main.cpp
///  @brief   le main du programme ProjetNuanceur pour le cours INF8702 de Polytechnique
///  @author  Frédéric Plourde (2007)
///  @author  Félix Gingras Harvey (2016)
///  @date    2007 / 2016
///  @version 2.0
///
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/transform.hpp>

#include "Cst.h"
#include "NuanceurProg.h"
#include "ObjParser/MathUtils.h"
#include "Texture2D.h"
#include "Var.h"
#include "textfile.h"
#include "SurfaceNode.h"

#include <string>

using namespace Math3D;

///////////////////////////////////////////////
// Objects                                   //
///////////////////////////////////////////////

// Shaders
static CNuanceurProg progNuanceurSea( "Nuanceurs/seaSommets.glsl", "Nuanceurs/seaFragments.glsl", 
                                      "Nuanceurs/nuanceurTessCtrl.glsl", "Nuanceurs/nuanceurTessEval.glsl", false );

// Camera Attributes
static float horizontalAngle = 0.f;
static float verticalAngle   = 0.f;
static float cameraSpeed = 50.0f; // unités / seconde
static float mouseSpeed = 0.075f;

// Camera position
static glm::vec3 cam_position = glm::vec3(0, 0, 0);
static glm::vec3 prev_cam_position = glm::vec3(0, 0, 0);
static glm::vec3 direction    = glm::vec3(0.f, 0.f, -1.0f);
static glm::vec3 cam_right    = glm::vec3(1.f, 0.f, 0.f);
static glm::vec3 cam_up       = glm::vec3(0.f, 1.f, 0.f);

// Models matrix
static glm::mat4 seaModelMatrix;

// Debug tessellation levels
static GLboolean stopComputingTree = false;

///////////////////////////////////////////////
// PROTOTYPES DES FONCTIONS DU MAIN          //
///////////////////////////////////////////////
void      initialisation(void);
void      drawScene(void);
glm::mat4 getModelMatrixSea(void);
void      setLightsAttributes(const GLuint progNuanceur);
void      keyboard(GLFWwindow* fenetre, int touche, int scancode, int action, int mods);
void      mouseMovement(GLFWwindow* window, double deltaT, glm::vec3& direction, glm::vec3& right, glm::vec3& up);
void      resize(GLFWwindow* fenetre, int w, int h);
void      refreshCamera(GLFWwindow* window, double deltaT);
void      compileShaders();

// le main
int main(int /*argc*/, char* /*argv*/[])
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit())
    {
        fprintf(stderr, "ERREUR: impossible d'initialiser GLFW3\n");
        return 1;
    }

    GLFWwindow* fenetre = glfwCreateWindow(CVar::currentW, CVar::currentH, "INF8702 - Labo", nullptr, nullptr);
    if (!fenetre)
    {
        fprintf(stderr, "ERREUR: impossibe d'initialiser la fenêtre avec GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwSetWindowPos(fenetre, 600, 100);

    // Rendre le contexte openGL courrant celui de la fenêtre
    glfwMakeContextCurrent(fenetre);

    // Combien d'updates d'écran on attend après l'appel à glfwSwapBuffers()
    // pour effectivement échanger les buffers et retourner
    glfwSwapInterval(1);

    // Définir la fonction clavier
    glfwSetKeyCallback(fenetre, keyboard);

    // Reset mouse position for next frame
    glfwSetCursorPos(fenetre, CVar::currentW / 2, CVar::currentH / 2);

    // Définire le comportement du curseur
    glfwSetInputMode(fenetre, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Définir la fonction de redimensionnement
    glfwSetWindowSizeCallback(fenetre, resize);

    // vérification de la version 4.X d'openGL
    glewInit();
    if (glewIsSupported("GL_VERSION_4_5"))
        printf("Pret pour OpenGL 4.5\n\n");
    else
    {
        printf("\nOpenGL 4.5 n'est pas supporte! \n");
        exit(1);
    }

    // Specifier le context openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // recueillir des informations sur le système de rendu
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);
    printf("Materiel de rendu graphique: %s\n", renderer);
    printf("Plus récente version d'OpenGL supportee: %s\n\n", version);

    GLint max;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max);
    printf("GL_MAX_TEXTURE_UNITS = %d\n", max);

    glGetIntegerv(GL_MAX_VARYING_FLOATS, &max);
    printf("GL_MAX_VARYING_FLOATS = %d\n\n", max);

    if (!glfwExtensionSupported("GL_EXT_framebuffer_object"))
    {
        printf("Objets 'Frame Buffer' NON supportes! :( ... Je quitte !\n");
        exit(1);
    }
    else
    {
        printf("Objets 'Frame Buffer' supportes :)\n\n");
    }

    // Initialiser position curseur
    int mid_width, mid_height;
    glfwGetWindowSize(fenetre, &mid_width, &mid_height);
    CVar::currentW = mid_width;
    CVar::currentH = mid_height;
    mid_width /= 2;
    mid_height /= 2;
    glfwSetCursorPos(fenetre, mid_width, mid_height);

    GLenum err;
    while( ( err = glGetError() ) != GL_NO_ERROR )
    {
        printf( "Error drawing\n" );
    }
    // compiler et lier les nuanceurs
    compileShaders();

    // initialisation de variables d'état openGL et création des listes
    initialisation();


    double dernierTemps = glfwGetTime();
    int    nbFrames     = 0;

    // boucle principale de gestion des evenements
    while (!glfwWindowShouldClose(fenetre))
    {
        // Temps ecoule en secondes depuis l'initialisation de GLFW
        double temps  = glfwGetTime();
        double deltaT = temps - CVar::temps;
        CVar::temps   = temps;

        nbFrames++;
        // Si ça fait une seconde que l'on a pas affiché les infos
        if (temps - dernierTemps >= 1.0)
        {
            if (CVar::showDebugInfo)
            {
                printf("%f ms/frame\n", 1000.0 / double(nbFrames));
                printf("Position: (%f,%f,%f)\n", cam_position.x, cam_position.y, cam_position.z);
            }
            nbFrames = 0;
            dernierTemps += 1.0;
        }

        // Rafraichir le point de vue selon les input clavier et souris
        refreshCamera(fenetre, deltaT);

        // Afficher nos modèlests
        drawScene();

        // Swap buffers
        glfwSwapBuffers(fenetre);
        glfwPollEvents();
    }
    // close GL context and any other GLFW resources
    glfwTerminate();

    // on doit faire le ménage... !
    delete CVar::lumieres[ENUM_LUM::LumPonctuelle];
    delete CVar::lumieres[ENUM_LUM::LumDirectionnelle];
    delete CVar::lumieres[ENUM_LUM::LumSpot];
    surfaceShutdown();

    // le programme n'arrivera jamais jusqu'ici
    return EXIT_SUCCESS;
}

void attribuerValeursMateriel( const GLuint progNuanceur )
{
    GLint handle;
    GLfloat component[ 4 ] = { 0.15f , 0.26f , 0.55f , 1.0f };

    handle = glGetUniformLocation( progNuanceurSea.getProg(), "Material.Ambient" );
    glUniform4fv( handle, 1, component );

    handle = glGetUniformLocation( progNuanceurSea.getProg(), "Material.Diffuse" );
    glUniform4fv( handle, 1, component );

    handle = glGetUniformLocation( progNuanceurSea.getProg(), "Material.Specular" );
    glUniform4fv( handle, 1, component );

    handle = glGetUniformLocation( progNuanceurSea.getProg(), "Material.Exponent" );
    glUniform4fv( handle, 1, component );

    handle = glGetUniformLocation( progNuanceurSea.getProg(), "Material.Shininess" );
    glUniform1f( handle, 100.f );
}

void attribuerValeursLumieres( const GLuint progNuanceur )
{
    // Handle pour attribut de lumiere
    GLint li_handle;

    li_handle = glGetUniformLocation( progNuanceur, "dirLightOn" );
    glUniform1i( li_handle, CVar::lumieres[ ENUM_LUM::LumDirectionnelle ]->estAllumee() );
    li_handle = glGetUniformLocation( progNuanceur, "pointLightOn" );
    glUniform1i( li_handle, CVar::lumieres[ ENUM_LUM::LumPonctuelle ]->estAllumee() );
    li_handle = glGetUniformLocation( progNuanceur, "spotLightOn" );
    glUniform1i( li_handle, CVar::lumieres[ ENUM_LUM::LumSpot ]->estAllumee() );

    // Fournir les valeurs d'éclairage au nuanceur.
    // Les directions et positions doivent être en référenciel de caméra.
    for( size_t i = 0; i < CVar::lumieres.size(); i++ )
    {
        // Placeholders pour contenir les valeurs
        GLfloat   temp3[ 3 ];
        GLfloat   temp4[ 4 ];
        glm::vec4 pos;

        // Creer un descripteur basé sur l'index de lumière
        std::string begin = "Lights[";
        int         l_idx = (int)i;
        std::string end = "]";
        std::string light_desc = begin + std::to_string( l_idx ) + end;

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".Ambient" ).c_str() );
        CVar::lumieres[ i ]->obtenirKA( temp3 );
        glUniform3fv( li_handle, 1, &temp3[ 0 ] );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".Diffuse" ).c_str() );
        CVar::lumieres[ i ]->obtenirKD( temp3 );
        glUniform3fv( li_handle, 1, &temp3[ 0 ] );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".Specular" ).c_str() );
        CVar::lumieres[ i ]->obtenirKS( temp3 );
        glUniform3fv( li_handle, 1, &temp3[ 0 ] );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".Position" ).c_str() );
        CVar::lumieres[ i ]->obtenirPos( temp4 );

        // Transformer ici la direction/position de la lumière vers un référenciel de caméra
        pos = glm::vec4( temp4[ 0 ], temp4[ 1 ], temp4[ 2 ], temp4[ 3 ] );
        pos = CVar::vue * pos;

        temp4[ 0 ] = pos.x;
        temp4[ 1 ] = pos.y;
        temp4[ 2 ] = pos.z;
        temp4[ 3 ] = pos.w;
        glUniform4fv( li_handle, 1, &temp4[ 0 ] );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".SpotDir" ).c_str() );
        CVar::lumieres[ i ]->obtenirSpotDir( temp3 );
        // Transformer ici la direction du spot
        pos = glm::vec4( temp3[ 0 ], temp3[ 1 ], temp3[ 2 ], 0.0f );
        pos = CVar::vue * pos;
        temp3[ 0 ] = pos.x;
        temp3[ 1 ] = pos.y;
        temp3[ 2 ] = pos.z;
        glUniform3fv( li_handle, 1, &temp3[ 0 ] );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".SpotExp" ).c_str() );
        glUniform1f( li_handle, CVar::lumieres[ i ]->obtenirSpotExp() );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".SpotCutoff" ).c_str() );
        glUniform1f( li_handle, CVar::lumieres[ i ]->obtenirSpotCutOff() );

        li_handle = glGetUniformLocation( progNuanceur, ( light_desc + ".Attenuation" ).c_str() );
        glUniform3f( li_handle, CVar::lumieres[ i ]->obtenirConsAtt(), CVar::lumieres[ i ]->obtenirLinAtt(),
                     CVar::lumieres[ i ]->obtenirQuadAtt() );
    }
}

void initialisation(void)
{

    ////////////////////////////////////////////////////
    // CONSTRUCTION DES LUMIÈRES
    ////////////////////////////////////////////////////

    // LUMIÈRE PONCTUELLE (enum : LumPonctuelle - 0)
    CVar::lumieres[ENUM_LUM::LumPonctuelle] =
        new CLumiere(0.3f, 0.53f, 0.9f,
            0.3f, 0.53f, 0.9f,
            0.4f, 0.4f, 0.7f,
            0.0f, 20.0f, -20.0f,
            1.0f, true);
    CVar::lumieres[ENUM_LUM::LumPonctuelle]->modifierConstAtt(1.1f);
    CVar::lumieres[ENUM_LUM::LumPonctuelle]->modifierLinAtt(0.0);
    CVar::lumieres[ENUM_LUM::LumPonctuelle]->modifierQuadAtt(0.0);

    // LUMIÈRE SPOT (enum : LumSpot - 1)
    CVar::lumieres[ENUM_LUM::LumSpot] = new CLumiere(0.3f, 0.53f, 0.9f,
        0.3f, 0.53f, 0.9f,
        0.4f, 0.4f, 0.7f,
        10.0f, 10.0f,-10.0f,
        1.0f, true, -0.5f,
        -1.0f, 1.0f, 5.f, 60.0);

    // LUMIÈRE DIRECTIONNELLE (enum : LumDirectionnelle - 2)
    CVar::lumieres[ENUM_LUM::LumDirectionnelle] =
        new CLumiere(0.3f, 0.53f, 0.9f,
            0.3f, 0.53f, 0.9f,
            0.4f, 0.4f, 0.7f,
            5.0f, -10.0f, -5.0f,
            0.0f, true);
    
    seaModelMatrix = getModelMatrixSea();

    surfaceInit();

    // fixer la couleur de fond
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // activer les etats openGL
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Unlock the fps (To go higher than 60 hz)
    //glfwSwapInterval( 0 );

}

///////////////////////////////////////////////////////////////////////////////
///  global public  getModelMatrixGazon\n
///
///  fonction de construction de la matrice modèle pour le gazon
///
///
///  @return glm::mat4 : la matrice modèle
///
///  @author Félix G. Harvey
///  @date   2016-10
///
///////////////////////////////////////////////////////////////////////////////
glm::mat4 getModelMatrixSea(void)
{
    // Création d'une matrice-modèle.
    glm::mat4 translationMatrix = glm::translate(glm::vec3(0.f, -20.f, 0.f));
    return translationMatrix;
}

///////////////////////////////////////////////////////////////////////////////
///  global public  dessinerScene \n
///
///  fonction de d'affichage pour la scène au complet.
///	 Appelle la fonction dessiner de chaque modèle présent
///
///  @return Aucune
///
///////////////////////////////////////////////////////////////////////////////
void drawScene()
{
    //////////////////	 Préparer l'affichage:	//////////////////
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, CVar::currentW, CVar::currentH);
    

    //////////////////     Afficher les objets:  ///////////////////////////
    glUseProgram(progNuanceurSea.getProg());
    attribuerValeursLumieres( progNuanceurSea.getProg() );
    attribuerValeursMateriel( progNuanceurSea.getProg() );

    if( !stopComputingTree )
    {
        if( !glm::all( glm::equal( cam_position, prev_cam_position ) ) );
            createTree( 0, 0, 0, 1000, 1000, cam_position );
    }

    renderSea(progNuanceurSea, cam_position);
    // Flush les derniers vertex du pipeline graphique
    glFlush();
}

///////////////////////////////////////////////////////////////////////////////
///  global public  clavier \n
///
///  fonction de rappel pour la gestion du clavier
///
///  @param [in]       pointeur GLFWwindow	Référence à la fenetre GLFW
///  @param [in]       touche	int			ID de la touche
///  @param [in]       scancode int			Code spécifique à la plateforme et à l'ID
///  @param [in]       action	int			Action appliquée à la touche
///  @param [in]       mods		int			Modifier bits
///
///  @return Aucune
///
///  @author Félix G. Harvey
///  @date   2016-06-03
///
///////////////////////////////////////////////////////////////////////////////
void keyboard(GLFWwindow* fenetre, int touche, int /* scancode */, int action, int /* mods */)
{
    switch (touche)
    {
    case GLFW_KEY_Q:
    {
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(fenetre, GL_TRUE);
        break;
    }
    case GLFW_KEY_ESCAPE:
    {
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(fenetre, GL_TRUE);
        break;
    }
    case GLFW_KEY_P:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::isPerspective)
                CVar::isPerspective = false;
            else
                CVar::isPerspective = true;
        }
        break;
    }
    case GLFW_KEY_I:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::showDebugInfo)
                CVar::showDebugInfo = false;
            else
                CVar::showDebugInfo = true;
        }
        break;
    }
    case GLFW_KEY_B:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::IBLon)
                CVar::IBLon = false;
            else
                CVar::IBLon = true;
        }
        break;
    }

    case GLFW_KEY_G:
    {
        if (action == GLFW_PRESS)
        {
            CVar::isSeaGrid = !CVar::isSeaGrid;
        }
        break;
    }

    case GLFW_KEY_F:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::FBOon)
                CVar::FBOon = false;
            else
                CVar::FBOon = true;
        }
        break;
    }

    case GLFW_KEY_C:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::mouseControl)
            {
                CVar::mouseControl = false;
                glfwSetInputMode(fenetre, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                CVar::mouseControl = true;
                glfwSetInputMode(fenetre, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
        }

        break;
    }
    case GLFW_KEY_1:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::lumieres[ENUM_LUM::LumDirectionnelle]->estAllumee())
                CVar::lumieres[ENUM_LUM::LumDirectionnelle]->eteindre();
            else
                CVar::lumieres[ENUM_LUM::LumDirectionnelle]->allumer();
        }

        break;
    }
    case GLFW_KEY_2:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::lumieres[ENUM_LUM::LumPonctuelle]->estAllumee())
                CVar::lumieres[ENUM_LUM::LumPonctuelle]->eteindre();
            else
                CVar::lumieres[ENUM_LUM::LumPonctuelle]->allumer();
        }
        break;
    }
    case GLFW_KEY_3:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::lumieres[ENUM_LUM::LumSpot]->estAllumee())
                CVar::lumieres[ENUM_LUM::LumSpot]->eteindre();
            else
                CVar::lumieres[ENUM_LUM::LumSpot]->allumer();
        }
        break;
    }

    // permuter le minFilter
    case GLFW_KEY_N:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::minFilter >= 5)
                CVar::minFilter = 0;
            else
                CVar::minFilter++;
        }
        break;
    }

    // permuter le magFilter
    case GLFW_KEY_M:
    {
        if (action == GLFW_PRESS)
        {
            if (CVar::magFilter >= 1)
                CVar::magFilter = 0;
            else
                CVar::magFilter++;
        }
        break;
    }

    case GLFW_KEY_T:
    {
        if (action == GLFW_PRESS)
        {
            stopComputingTree = !stopComputingTree;
        }
        break;
    }
    case GLFW_KEY_Y:
    {
        if (action == GLFW_PRESS)
        {
            CVar::waveSize++;
            std::cout << "waveSize = " << CVar::waveSize;
            std::cout << "\n";
        }
        break;
    }
    case GLFW_KEY_U:
    {
        if (action == GLFW_PRESS)
        {
            if(CVar::waveSize > 0 )
                CVar::waveSize--;
            std::cout << "waveSize = " << CVar::waveSize;
            std::cout << "\n";
        }
        break;
    }
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global public  redimensionnement \n
///
///  fonction de rappel pour le redimensionnement de la fenêtre graphique
///
///  @param [in]       w GLsizei    nouvelle largeur "w" en pixels
///  @param [in]       h GLsizei    nouvelle hauteur "h" en pixels
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-14
///
///////////////////////////////////////////////////////////////////////////////
void resize(GLFWwindow* /*fenetre*/, int w, int h)
{
    CVar::currentW = w;
    CVar::currentH = h;
    glViewport(0, 0, w, h);

    drawScene();
}

void setLightsAttributes(const GLuint progNuanceur)
{
    // Handle pour attribut de lumiere
    GLint li_handle;

    li_handle = glGetUniformLocation(progNuanceur, "dirLightOn");
    glUniform1i(li_handle, CVar::lumieres[ENUM_LUM::LumDirectionnelle]->estAllumee());
    li_handle = glGetUniformLocation(progNuanceur, "pointLightOn");
    glUniform1i(li_handle, CVar::lumieres[ENUM_LUM::LumPonctuelle]->estAllumee());
    li_handle = glGetUniformLocation(progNuanceur, "spotLightOn");
    glUniform1i(li_handle, CVar::lumieres[ENUM_LUM::LumSpot]->estAllumee());

    // Fournir les valeurs d'éclairage au nuanceur.
    // Les directions et positions doivent être en référenciel de caméra.
    for (size_t i = 0; i < CVar::lumieres.size(); i++)
    {
        // Placeholders pour contenir les valeurs
        GLfloat   temp3[3];
        GLfloat   temp4[4];
        glm::vec4 pos;

        // Creer un descripteur basé sur l'index de lumière
        std::string begin      = "Lights[";
        int         l_idx      = (int)i;
        std::string end        = "]";
        std::string light_desc = begin + std::to_string(l_idx) + end;

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".Ambient").c_str());
        CVar::lumieres[i]->obtenirKA(temp3);
        glUniform3fv(li_handle, 1, &temp3[0]);

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".Diffuse").c_str());
        CVar::lumieres[i]->obtenirKD(temp3);
        glUniform3fv(li_handle, 1, &temp3[0]);

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".Specular").c_str());
        CVar::lumieres[i]->obtenirKS(temp3);
        glUniform3fv(li_handle, 1, &temp3[0]);

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".Position").c_str());
        CVar::lumieres[i]->obtenirPos(temp4);

        // Transformer ici la direction/position de la lumière vers un référenciel de caméra
        pos = glm::vec4(temp4[0], temp4[1], temp4[2], temp4[3]);
        pos = CVar::vue * pos;

        temp4[0] = pos.x;
        temp4[1] = pos.y;
        temp4[2] = pos.z;
        temp4[3] = pos.w;
        glUniform4fv(li_handle, 1, &temp4[0]);

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".SpotDir").c_str());
        CVar::lumieres[i]->obtenirSpotDir(temp3);
        // Transformer ici la direction du spot
        pos      = glm::vec4(temp3[0], temp3[1], temp3[2], 0.0f);
        pos      = CVar::vue * pos;
        temp3[0] = pos.x;
        temp3[1] = pos.y;
        temp3[2] = pos.z;
        glUniform3fv(li_handle, 1, &temp3[0]);

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".SpotExp").c_str());
        glUniform1f(li_handle, CVar::lumieres[i]->obtenirSpotExp());

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".SpotCutoff").c_str());
        glUniform1f(li_handle, CVar::lumieres[i]->obtenirSpotCutOff());

        li_handle = glGetUniformLocation(progNuanceur, (light_desc + ".Attenuation").c_str());
        glUniform3f(li_handle, CVar::lumieres[i]->obtenirConsAtt(), CVar::lumieres[i]->obtenirLinAtt(),
                    CVar::lumieres[i]->obtenirQuadAtt());
    }
}

//////////////////////////////////////////////////////////
////////////  FONCTIONS POUR LA SOURIS ///////////////////
//////////////////////////////////////////////////////////

void mouseMovement(GLFWwindow* window, double deltaT, glm::vec3& direction, glm::vec3& right, glm::vec3& up)
{
    if (CVar::mouseControl)
    {
        // Déplacement de la souris:
        // Taille actuelle de la fenetre
        int mid_width, mid_height;
        glfwGetWindowSize(window, &mid_width, &mid_height);
        mid_width /= 2;
        mid_height /= 2;

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Reset mouse position for next frame
        glfwSetCursorPos(window, mid_width, mid_height);

        // Nouvelle orientation
        horizontalAngle += mouseSpeed * float(deltaT * (mid_width - xpos));
        verticalAngle += mouseSpeed * float(deltaT * (mid_height - ypos));
    }
    // Direction : Spherical coordinates to Cartesian coordinates conversion
    direction = glm::vec3(std::cos(verticalAngle) * std::sin(horizontalAngle), std::sin(verticalAngle),
                          std::cos(verticalAngle) * std::cos(horizontalAngle));

    // Right vector
    right = glm::vec3(std::sin(horizontalAngle - 3.14f / 2.0f), 0, std::cos(horizontalAngle - 3.14f / 2.0f));

    // Up vector : perpendicular to both direction and right
    up = glm::cross(right, direction);
}

//////////////////////////////////////////////////////////
////////////  FONCTIONS POUR LA CAMÉRA ///////////////////
//////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///  @brief Fonction de gestion de la position de la caméra en coordonnées sphériques.
///  Elle s'occuper de trouver les coordonnées x et y de la caméra à partir
///  des theta et phi courants, puis fixe dans openGL la position de la caméra
///  à l'aide de gluLookAt().
///
///  @author Frédéric Plourde
///  @date   2007-12-14
///////////////////////////////////////////////////////////////////////////////
void refreshCamera(GLFWwindow* fenetre, double deltaT)
{
    mouseMovement(fenetre, deltaT, direction, cam_right, cam_up);

    // Move forward
    if (glfwGetKey(fenetre, GLFW_KEY_W) == GLFW_PRESS)
    {
        prev_cam_position = cam_position;
        cam_position += direction * (float)deltaT * cameraSpeed;
    }
    // Move backward
    if (glfwGetKey(fenetre, GLFW_KEY_S) == GLFW_PRESS)
    {
        prev_cam_position = cam_position;
        cam_position -= direction * (float)deltaT * cameraSpeed;
    }
    // Strafe right
    if (glfwGetKey(fenetre, GLFW_KEY_D) == GLFW_PRESS)
    {
        prev_cam_position = cam_position;
        cam_position += cam_right * (float)deltaT * cameraSpeed;
    }
    // Strafe left
    if (glfwGetKey(fenetre, GLFW_KEY_A) == GLFW_PRESS)
    {
        prev_cam_position = cam_position;
        cam_position -= cam_right * (float)deltaT * cameraSpeed;
    }

    // Matrice de projection:
    float ratio = static_cast<float>(CVar::currentW) / CVar::currentH;
    if (CVar::isPerspective)
    {
        // Caméra perspective:
        CVar::projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 2000.0f);
    }
    else
    {
        // Caméra orthographique :
        CVar::projection =
            glm::ortho(-5.0f * ratio, 5.0f * ratio, -5.0f, 5.0f, 0.001f, 3000.0f); // In world coordinates
    }

    // Matrice de vue:
    CVar::vue = glm::lookAt(cam_position,             // Position de la caméra
                            cam_position + direction, // regarde vers position + direction
                            cam_up                    // Vecteur "haut"
    );
}

//////////////////////////////////////////////////////////
//////////  FONCTIONS POUR LES NUANCEURS /////////////////
//////////////////////////////////////////////////////////
void compileShaders()
{
    // on compiler ici les programmes de nuanceurs qui furent prédéfinis
    progNuanceurSea.compilerEtLier();
}
