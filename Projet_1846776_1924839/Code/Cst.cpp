///////////////////////////////////////////////////////////////////////////////
///  @file Cst.cpp
///  @author  Frédéric Plourde
///  @brief Définit les CONSTANTES GLOBALES du programme
///  @date    2007-12-11
///  @version 1.0
///////////////////////////////////////////////////////////////////////////////
#include "Cst.h"

SINGLETON_DECLARATION_CPP(CCst);

/// spécificateur du mode de séparation de la couleur spéculaire
const GLfloat CCst::gl_separate_specular_color = GL_SEPARATE_SPECULAR_COLOR;

const int CCst::g_nWidth  = 640;
const int CCst::g_nHeight = 480;

const float CCst::freqRotX = 0.15f;
const float CCst::freqRotY = 0.1f;
const float CCst::freqRotZ = 0.2f;

const float CCst::frameRate = 50.0f;

const int CCst::nbLumieresOpenGL = 3;

const float CCst::largeurModele  = 20.0f;
const float CCst::hauteurModele  = 12.46f;
const float CCst::grandeurSkybox = 1000.0f;
const float CCst::largeurGazon   = 360.0f;
const float CCst::longueurGazon  = 160.0f;
const float CCst::hauteurGazon   = 10.0f;

const int CCst::tailleShadowMap = 512;

const int CCst::nbColonnes = 96;
const int CCst::nbRangees  = 60;

const int CCst::texUnit_0 = 0;
const int CCst::texUnit_1 = 1;
const int CCst::texUnit_2 = 2;
const int CCst::texUnit_3 = 3;
const int CCst::texUnit_4 = 4;

const GLuint CCst::mapFilters[6] = {GL_NEAREST,
                                    GL_LINEAR,
                                    GL_NEAREST_MIPMAP_NEAREST,
                                    GL_LINEAR_MIPMAP_NEAREST,
                                    GL_NEAREST_MIPMAP_LINEAR,
                                    GL_LINEAR_MIPMAP_LINEAR};

const GLenum CCst::unitesTextures[32] = {
    GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,  GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
    GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
    GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
    GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31};

const GLuint CCst::indexTangente = 1;

const Point CCst::Points[32] = {
    // V0
    {0, 3, 0}, // U0
    {1, 3, 0}, // U1
    {2, 3, 0}, // U2
    {3, 3, 0}, // U3
    {4, 3, 0}, // U4
    {5, 3, 0}, // U5
    {6, 3, 0}, // U6
    {7, 3, 0}  // U7
    ,
    // V1
    {0, 2, 0}, // U0
    {1, 2, 6}, // U1
    {2, 2, 5}, // U2
    {3, 2, 1}, // U3
    {4, 2, 0}, // U4
    {5, 2, 2}, // U5
    {6, 2, 1}, // U6
    {7, 2, 3}  // U7
    ,
    // V2
    {0, 1, 0},  // U0
    {1, 1, 3},  // U1
    {2, 1, 6},  // U2
    {3, 1, 3},  // U3
    {4, 1, 1},  // U4
    {5, 1, -1}, // U5
    {6, 1, 3},  // U6
    {7, 1, 4}   // U7
    ,
    // V3
    {0, 0, 0}, // U0
    {1, 0, 0}, // U1
    {2, 0, 0}, // U2
    {3, 0, 0}, // U3
    {4, 0, 0}, // U4
    {5, 0, 0}, // U5
    {6, 0, 0}, // U6
    {7, 0, 0}  // U7
};

const float CCst::VNodalU[12] = {0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f};
const float CCst::VNodalV[8]  = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
