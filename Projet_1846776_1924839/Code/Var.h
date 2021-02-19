///////////////////////////////////////////////////////////////////////////////
///  @file Var.h
///  @author  Frédéric Plourde
///  @author  Félix Gingras Harvey
///  @brief   Déclare les VARIABLES GLOBALES du programme
///  @date    2007 - 2016
///  @version 2.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/transform.hpp>

#include "Lumiere.h"
#include "Singleton.h"

///////////////////////////////////////////////
// Structure de données globales             //
///////////////////////////////////////////////

struct Image
{
    bool     valide;
    int      tailleX;
    int      tailleY;
    std::vector<GLubyte> data;
};

///////////////////////////////////////////////////////////////////////////
/// @class CVar
/// @brief Les VARIABLES globales du programme.
///        C'est une classe singleton.
///
/// @author Frédéric Plourde
/// @date 2007-12-11
///////////////////////////////////////////////////////////////////////////
class CVar : public Singleton<CVar>
{
    SINGLETON_DECLARATION_CLASSE(CVar)

public:
    /// indique si on désire activer l'utilisation des FBOs
    static int FBOon;

    /// indique si on désiere utiliser l'éclairage IBL;
    static int IBLon;

    /// indique si on désire activer le cell/toon shading
    static int toonOn;

    /// Indique le filtre à utiliser pour le minFilter (utilisé comme index à CCst::mapFilters[])
    static int minFilter;
    /// Indique le filtre à utiliser pour le minFilter (utilisé comme index à CCst::mapFilters[])
    static int magFilter;

    /// le ID de la fenêtre graphique GLUT
    static int g_nWindowID;

    /// Largeur et hauteur courantes de la fenêtre graphique (viewport)
    static int currentW, currentH;

    /// indique si on est en mode Perspective (true) ou glOrtho (false)
    static bool isPerspective;

    /// indique si on fait tourner le/les objets
    static bool mouseControl;

    /// angle de rotation de la caméra en theta (coord. sphériques)
    static double theta;

    /// angle de rotation de la caméra en phi (coord. sphériques)
    static double phi;

    /// angle de rotation de la caméra en rho (coord. sphériques)
    static double rho;

    /// la position de la camera en X
    static float x;

    /// la position de la camera en Y
    static float y;

    /// la position de la camera en Z
    static float z;

    /// angle courant de rotation en X du modèle
    static float angleRotX;

    /// angle courant de rotation en Y du modèle
    static float angleRotY;

    /// angle courant de rotation en Z du modèle
    static float angleRotZ;

    /// indique si la souris fut bougée déjà
    static bool premierMouvement;

    /// la dernière valeur en X de position lue de la souris
    static double dernierX;

    /// la derniere valeur en Y de position lus de la souris
    static double dernierY;

    /// déclaration de la liste d'affichage du modèle
    static GLuint listeAffModele;

    /// la texture de perlin
    static GLuint perlinTex;

    /// temps
    static double temps;

    /// Matrice vue courante
    static glm::mat4 vue;

    /// Matrice projection courante
    static glm::mat4 projection;

    // la liste des lumieres openGL de la scène
    static std::vector<CLumiere*> lumieres;

    /// Lumiere ponctuelle allumée?
    static bool pointLightOn;

    /// Lumiere directionnelle allumée?
    static bool dirLightOn;

    /// Lumière spot allumée?
    static bool spotLightOn;

    static bool showDebugInfo;
    static bool isSeaGrid;
    static GLint waveSize;
};
