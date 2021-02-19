///////////////////////////////////////////////////////////////////////////////
///  @file NuanceurProg.h
///  @author  Frédéric Plourde
///  @brief   Ce fichier encapsule la classe CNuanceurProg, une classe utilitaire
///           contenant un shader et capable d'effectuer plusieurs fonctions
///           pratiques de gestion des nuanceurs.
///  @date    2007-12-12
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "Cst.h"
#include "textfile.h"

///////////////////////////////////////////////////////////////////////////////
///  floatUniform
///  Structure encapsulant un uniform de type float
///
///  @remarks cette structure possède un contructeur assignant nom et val
///
///  @author Frédéric Plourde @date 2007-12-14
///////////////////////////////////////////////////////////////////////////////
struct floatUniform
{
    const char*  nom;
    const float* val;

    floatUniform(const char* n, const float* v)
    {
        nom = n;
        val = v;
    }
};

///////////////////////////////////////////////////////////////////////////////
///  integerUniform
///  Structure encapsulant un uniform de type integer
///
///  @remarks cette structure possède un contructeur assignant nom et val
///
///  @author Frédéric Plourde @date 2007-12-14
///////////////////////////////////////////////////////////////////////////////
struct integerUniform
{
    const char* nom;
    const int*  val;

    integerUniform(const char* n, const int* v)
    {
        nom = n;
        val = v;
    }
};

///////////////////////////////////////////////////////////////////////////////
///  @class CNuanceurProg
///  @brief Classe qui encapsule un programme de nuanceurs (1 programme + 1 ou 2 nuanceurs)
///
///
///  @remarks le programme de nuanceur peut être vide (0 nuanceurs) pour simuler la fonctionalité fixe d'openGL
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
class CNuanceurProg
{
public:
    /// constructeur par défaut : utilisé pour créer un PROGRAMME VKDE de nuanceur (0)
    CNuanceurProg(void);
    CNuanceurProg( const std::string& nuanceurSommetsStr, const std::string& nuanceurFragmentsStr, const std::string& nuanceurTessCtrlStr, const std::string& nuanceurTessEvalStr, const bool compilerMaintenant );
    /// constructeur spécifique : utilisé pour créer un PROGRAMME DE NUANCEURS.
    CNuanceurProg(const std::string& nuanceurSommetsStr, const std::string& nuanceurFragmentsStr,
                  const bool compilerImmediatement);

    /// compile et lie dans openGL les nuanceurs du programme
    void compilerEtLier();

    /// Activer le nuanceur dans openGL
    void activer();

    /// Permet de lancer une valeur uniform (int) immédiatement
    void uniform1(const char* nom, const int v);

    /// Permet de lancer une valeur uniform (float) immédiatement
    void uniform1(const char* nom, const float v);

    /// Enregistre au sein du programme un uniform float qui sera inscrit à l'activation du nuanceur
    void enregistrerUniformFloat(const char* nom, const float& val);

    /// Enregistre au sein du programme un uniform entier qui sera inscrit à l'activation du nuanceur
    void enregistrerUniformInteger(const char* nom, const int& val);

    /// Retourne l'identificateur du programme de nuanceurs pour utiliser externe spécifique
    GLuint getProg();

private:
    /// Compile et lie les nuanceurs définis à la construction
    void compilerEtLierNuanceurs(const std::string& nsStr, const std::string& nfStr);

    /// Affiche les informations de compilation d'un nuanceur
    void afficherShaderInfoLog(const GLuint obj, const std::string& message);

    /// Affiche les informations de compilation et d'édition de liens (link) du programme de nuanceurs
    void afficherProgramInfoLog(const GLuint obj, const std::string& message);

    void compilerEtLierNuanceurs( const std::string& nsStr, const std::string& nfStr, const std::string& ntcStr, const std::string& nteStr );

    /// la chaîne de caractères du nom de fichier du nuanceur de sommets
    std::string nuanceurSommetsStr_;

    /// la chaîne de caractères du nom de fichier du nuanceur de fragments
    std::string nuanceurFragmentsStr_;

    /// la chaîne de caractères du nom des fichier du nuanceur de tess
    std::string nuanceurTessCtrlStr_;
    std::string nuanceurTessEvalStr_;

    /// la liste des uniforms float requis par les nuanceurs
    std::vector<floatUniform> floatUniforms_;

    /// la liste des uniforms integer requis par les nuanceurs
    std::vector<integerUniform> integerUniforms_;

    /// indique si le programme en cours est vide (fonctionalité fixe d'openGL)
    bool estVide_;

    /// Indique si les nuanceurs du programme ont été compilés et linkés
    bool estCompileEtLie_;

    /// l'identificateur du programme de nuanceurs
    GLuint prog_;
};
