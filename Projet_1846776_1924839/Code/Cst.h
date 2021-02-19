///////////////////////////////////////////////////////////////////////////////
///  @file Cst.h
///  @author  Frédéric Plourde
///  @brief   Déclare les CONSTANTES GLOBALES du programme
///  @date    2007-12-11
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cmath>

#include <GL/glew.h>

#include "Singleton.h"

///////////////////////////////////////////////
// définitions de types globaux              //
///////////////////////////////////////////////
typedef unsigned int uint;

///////////////////////////////////////////////////////////////////////////////
///  colorRGB
///  Cette structure déclare une couleur RGB (r,g,b) sur des float (0..1)
///
///
///  @author Frédéric Plourde @date 2008-03-11
///////////////////////////////////////////////////////////////////////////////
struct colorRGB
{
    float r;
    float g;
    float b;
};

///////////////////////////////////////////////////////////////////////////////
///  vect3D
///  Cette structure déclare un vecteur 3D (x,y,z)
///
///  @remarks <TODO: insérer une remarque ici>
///
///  @author Frédéric Plourde @date 2008-03-11
///////////////////////////////////////////////////////////////////////////////
struct vect3D
{
    float x;
    float y;
    float z;
};

using Point = vect3D;

///////////////////////////////////////////////
// Énumérations globales                     //
///////////////////////////////////////////////

/// énumération des lumières de la scènes (à utiliser comme INDEX de lumieres[])
enum ENUM_LUM
{
    LumPonctuelle,
    LumSpot,
    LumDirectionnelle
};

///////////////////////////////////////////////
// déclaration des macros et defines globaux //
///////////////////////////////////////////////

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

#define PRODVECT(normale, vect1, vect2)                    \
    normale.x = (vect1.y * vect2.z) - (vect1.z * vect2.y); \
    normale.y = (vect1.z * vect2.x) - (vect1.x * vect2.z); \
    normale.z = (vect1.x * vect2.y) - (vect1.y * vect2.x)

/// Element-wise vector subtraction
inline const vect3D operator-(const vect3D& lhs, const vect3D& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

/// Element-wise vector addition
inline const vect3D operator+(const vect3D& lhs, const vect3D& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

/// Element-wise vector division
inline const vect3D& operator/=(vect3D& lhs, const float rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

inline const vect3D& normalize(vect3D& vecteur)
{
    float norm = std::sqrt((vecteur.x * vecteur.x) + (vecteur.y * vecteur.y) + (vecteur.z * vecteur.z));
    return vecteur /= norm;
}

#define CITYBLOCKNORM(V1) (V1.x * V1.x) + (V1.y * V1.y) + (V1.z * V1.z);

///////////////////////////////////////////////////////////////////////////
/// @class CCst
/// @brief Les CONSTANTES globales du programme.
///        C'est une classe singleton.
///
/// @author Frédéric Plourde
/// @date 2007-12-11
///////////////////////////////////////////////////////////////////////////
class CCst : public Singleton<CCst>
{
    SINGLETON_DECLARATION_CLASSE(CCst)

public:
    /// spécificateur du mode de séparation de la couleur spéculaire
    static const GLfloat gl_separate_specular_color;

    /// la largeur initiale de la fenêtre GLUT
    static const int g_nWidth;
    /// la hauteur initiale de la fenêtre GLUT
    static const int g_nHeight;

    /// la fréquence de rotation en X du modèle
    static const float freqRotX;
    /// la fréquence de rotation en Y du modèle
    static const float freqRotY;
    /// la fréquence de rotation en Z du modèle
    static const float freqRotZ;

    /// le framerate cible de l'application graphique.
    static const float frameRate;

    /// le nombre de lumières openGL de la scène
    static const int nbLumieresOpenGL;

    /// la largeur du modèle 3D
    static const float largeurModele;
    /// la hauteur du modèle 3D
    static const float hauteurModele;
    /// le nombre de colonnes (quads) dans la grille de quads
    static const int nbColonnes;
    /// le nombre de rangées (quads) dans la grille de quads
    static const int nbRangees;
    /// la grandeur du skybox
    static const float grandeurSkybox;
    /// la largeur du gazon (patch Bspline)
    static const float largeurGazon;
    /// la longueur du gazon (patch Bspline)
    static const float longueurGazon;
    /// la hauteur du gazon (patch Bspline)
    static const float hauteurGazon;

    /// la resolution des shadowMaps
    static const int tailleShadowMap;

    /// la constante "0" pour l'unité de texture 0
    static const int texUnit_0;
    /// la constante "1" pour l'unité de texture 1
    static const int texUnit_1;
    /// la constante "2" pour l'unité de texture 2
    static const int texUnit_2;
    /// la constante "3" pour l'unité de texture 3
    static const int texUnit_3;
    /// la constante "4" pour l'unité de texture 4
    static const int texUnit_4;

    /// les différents paramètres pour le filtrage mipmaping
    static const GLuint mapFilters[6];

    /// le vecteur des GLenum d'unités de textures
    static const GLenum unitesTextures[32];

    /// l'index unique de l'attribut de tangente
    static const GLuint indexTangente;

    /// les points de contrôle de la patch BSpline du gazon
    static const Point Points[32];

    /// le vecteur nodal en U de la patch BSpline (gazon)
    static const float VNodalU[12];

    /// le vecteur nodal en V de la patch BSpline (gazon)
    static const float VNodalV[8];
};
