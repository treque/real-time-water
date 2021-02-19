///////////////////////////////////////////////////////////////////////////////
///  @file Var.cpp
///  @author  Frédéric Plourde
///  @author  Félix Gingras Harvey
///  @brief   Déclare les VARIABLES GLOBALES du programme
///  @date    2007 - 2016
///  @version 2.0
///
///////////////////////////////////////////////////////////////////////////////
#include "Var.h"

#include "Cst.h"
#include "ObjParser/MathUtils.h"

using namespace Math3D;

SINGLETON_DECLARATION_CPP(CVar);

int CVar::g_nWindowID;
int CVar::currentW = 1280;
int CVar::currentH = 720;

int CVar::FBOon = true;
int CVar::IBLon = false;

int CVar::toonOn = false;

int CVar::minFilter = 0;
int CVar::magFilter = 0;

bool CVar::isPerspective = true;
bool CVar::mouseControl  = true;

std::vector<CLumiere*> CVar::lumieres      = std::vector<CLumiere*>(CCst::nbLumieresOpenGL);
bool                   CVar::pointLightOn  = true;
bool                   CVar::dirLightOn    = true;
bool                   CVar::spotLightOn   = true;
bool                   CVar::showDebugInfo = false;
bool CVar::isSeaGrid = false;
GLint CVar::waveSize = 2;

double CVar::theta = Deg2Rad(270.0);
double CVar::phi   = Deg2Rad(90.0);
double CVar::rho   = 30.0;

float CVar::x;
float CVar::y;
float CVar::z;

float CVar::angleRotX = 0.0f;
float CVar::angleRotY = 0.0f;
float CVar::angleRotZ = 0.0f;

bool   CVar::premierMouvement = true;
double CVar::dernierX         = 0;
double CVar::dernierY         = 0;
double CVar::temps            = 0;

glm::mat4 CVar::vue        = glm::mat4();
glm::mat4 CVar::projection = glm::mat4();

GLuint CVar::listeAffModele;

GLuint CVar::perlinTex = 0;
