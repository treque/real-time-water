///////////////////////////////////////////////////////////////////////////////
///  @file NuanceurProg.cpp
///  @author  Frédéric Plourde
///  @brief   Ce fichier définit la classe CNuanceurProg, une classe utilitaire
///           contenant un shader et capable d'effectuer plusieurs fonctions
///           pratiques de gestion des nuanceurs.
///  @date    2007-12-12
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////

#include "NuanceurProg.h"

///////////////////////////////////////////////////////////////////////////////
///  public overloaded constructor  CNuanceurProg \n
///
///  Constructeur par défaut utilisé pour créer le programme vide (fixed Functionality d'openGL)
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
CNuanceurProg::CNuanceurProg(void)
{
    // indiquer que le programme en cours sera un programme vide (et déjà compilé)
    estVide_         = true;
    estCompileEtLie_ = true;

    // met prog_ à  0 parce que glUseProgram(0) va activer la fonctionalité fixe
    prog_ = 0;
}

///////////////////////////////////////////////////////////////////////////////
///  global public overloaded  CNuanceurProg \n
///
///  Constructeur utilisé pour construire un programme de nuanceurs comportant
///  un nuanceur de sommets et/ou un nuanceur de fragments. Peut produire
///  un programme vide (pour fonctionnalité fixe d'openGL).
///
///  @param [in]  nuanceurSommetsStr char *    Nom de fichier du nuanceur de sommets  (si = 0 (ou NULL) -> pas de
///  nuanceurs de sommets)
///  @param [in]  nuanceurFragmentStr char *   Nom de fichier du nuanceur de fragments(si = 0 -> pas de nuanceurs de
///  fragments)
///  @param [in]  compilerMaintenant bool      Indique si l'on doit compiler les nuanceurs à la construction de l'objet
///
///  @return int
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
CNuanceurProg::CNuanceurProg(const std::string& nuanceurSommetsStr, const std::string& nuanceurFragmentsStr,
                              const std::string& nuanceurTessCtrlStr, const std::string& nuanceurTessEvalStr, const bool compilerMaintenant)
    : nuanceurSommetsStr_(nuanceurSommetsStr)
    , nuanceurFragmentsStr_(nuanceurFragmentsStr)
    , nuanceurTessCtrlStr_( nuanceurTessCtrlStr)
    , nuanceurTessEvalStr_( nuanceurTessEvalStr)
    , estCompileEtLie_(false)

{
    // s'assurer qu'au moins UN des deux nuanceurs est défini
    assert(!(nuanceurSommetsStr_.empty() && nuanceurFragmentsStr_.empty()));

    // indiquer que le programme de nuanceurs n'est pas vide
    estVide_ = false;

    if (compilerMaintenant)
    {
        compilerEtLierNuanceurs(nuanceurSommetsStr, nuanceurFragmentsStr, nuanceurTessCtrlStr, nuanceurTessEvalStr);
    }
}

CNuanceurProg::CNuanceurProg(const std::string& nuanceurSommetsStr, const std::string& nuanceurFragmentsStr,
                             const bool compilerMaintenant)
    : nuanceurSommetsStr_(nuanceurSommetsStr)
    , nuanceurFragmentsStr_(nuanceurFragmentsStr)
    , estCompileEtLie_(false)

{
    // s'assurer qu'au moins UN des deux nuanceurs est défini
    assert(!(nuanceurSommetsStr_.empty() && nuanceurFragmentsStr_.empty()));

    // indiquer que le programme de nuanceurs n'est pas vide
    estVide_ = false;

    if (compilerMaintenant)
    {
        compilerEtLierNuanceurs(nuanceurSommetsStr, nuanceurFragmentsStr);
    }
}

///////////////////////////////////////////////////////////////////////////////
///  global public  compilerEtLier \n
///
///  Compile et lie les nuanceurs (dans openGL).
///  Vérifie au préalable si les nuanceurs de sommets et de fragments furent
///  définis, car un nuanceur vide (pour la "fixed functionality d'openGL")
///  ne se compile pas.
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
void CNuanceurProg::compilerEtLier()
{
    // vérifie si le programme en cours est un programme de nuanceurs
    assert(!estVide_);

    compilerEtLierNuanceurs(nuanceurSommetsStr_, nuanceurFragmentsStr_, nuanceurTessCtrlStr_, nuanceurTessEvalStr_);

}

void CNuanceurProg::enregistrerUniformFloat(const char* nom, const float& val)
{
    floatUniform u(nom, &val);
    floatUniforms_.push_back(u);
}

void CNuanceurProg::enregistrerUniformInteger(const char* nom, const int& val)
{
    integerUniform u(nom, &val);
    integerUniforms_.push_back(u);
}

///////////////////////////////////////////////////////////////////////////////
///  public  activer \n
///
///  Fonction publique qui active le programme de nuanceurs dans openGL
///  La fonction vérifie au préalable si le programme de nuanceurs fut compilé et lié
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
void CNuanceurProg::activer()
{
    // activer le programme de nuanceurs
    glUseProgram(prog_);

    // inscrire les uniforms float requis par les nuanceurs
    if (!floatUniforms_.empty())
    {
        for (unsigned int i = 0; i < floatUniforms_.size(); i++)
        {
            glUniform1f(glGetUniformLocation(prog_, floatUniforms_[i].nom), *floatUniforms_[i].val);
        }
    }

    // inscrire les uniforms int requis par les nuanceurs
    if (!integerUniforms_.empty())
    {
        for (unsigned int i = 0; i < integerUniforms_.size(); i++)
        {
            glUniform1i(glGetUniformLocation(prog_, integerUniforms_[i].nom), *integerUniforms_[i].val);
        }
    }
}

void CNuanceurProg::uniform1(const char* nom, const int v)
{
    glUniform1i(glGetUniformLocation(prog_, nom), v);
}

void CNuanceurProg::uniform1(const char* nom, const float v)
{
    glUniform1f(glGetUniformLocation(prog_, nom), v);
}

///////////////////////////////////////////////////////////////////////////////
///  private  afficherShaderInfoLog \n
///
///  Cette fonction sert à afficher les informations de compilation
///  d'un nuanceur. Elle utilise la fonction glGetShaderInfoLog()".
///
///  @param [in]       obj GLuint   Le programme nuanceurs qui vient d'être créé
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
void CNuanceurProg::afficherShaderInfoLog(const GLuint obj, const std::string& message)
{
    int infologLength = 0;
    int charsWritten  = 0;

    // afficher le message d'en-tête
    printf("%s\n", message.c_str());

    // afficher le message d'erreur, le cas échéant
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 1)
    {
        std::vector<char> infoLog(static_cast<size_t>(infologLength));
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog.data());
        printf("%s\n", infoLog.data());
    }
    else
    {
        printf("Aucune erreur :-)\n\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
///  private  afficherProgramInfoLog \n
///
///  Cette fonction sert à afficher les informations de compilation et d'édition
///  de liens (link) du programme de nuanceurs. Elle utilise la fonction
///  "glGetProgramInfoLog()".
///
///  @param [in]       obj GLuint   Le programme nuanceurs qui vient d'être créé
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
void CNuanceurProg::afficherProgramInfoLog(const GLuint obj, const std::string& message)
{
    int infologLength = 0;
    int charsWritten  = 0;

    // afficher le message d'en-tête
    printf("%s\n", message.c_str());

    // afficher le message d'erreur, le cas échéant
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 1)
    {
        std::vector<char> infoLog(static_cast<size_t>(infologLength));
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog.data());
        printf("%s\n", infoLog.data());
    }
    else
    {
        printf("Aucune erreur :-)\n\n");
    }
}

///////////////////////////////////////////////////////////////////////////////
///  private  compilerEtLierNuanceurs \n
///
///  Lance la compilation et l'édition des liens (link) des différents nuanceurs
///  spécifiés (!= NULL). Crée aussi le programme de nuanceurs.
///
///  @param [in, out]  nsStr char *    le nom de fichier du nuanceur de sommets
///  @param [in, out]  nfStr char *    le nom de fichier du nuanceur de fragments
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2007-12-12
///
///////////////////////////////////////////////////////////////////////////////
void CNuanceurProg::compilerEtLierNuanceurs(const std::string& nsStr, const std::string& nfStr, const std::string& ntcStr, const std::string& nteStr)
{
    GLuint nuanceurSommets   = 0;
    GLuint nuanceurTessCtrl = 0;
    GLuint nuanceurTessEval = 0;
    GLuint nuanceurFragments = 0;


    if (!nsStr.empty())
    {
        // indiquer la progression...
        printf("Compilation du nuanceur de sommets   : %s \n", nsStr.c_str());

        // créer le nuanceur en GLSL
        nuanceurSommets = glCreateShader(GL_VERTEX_SHADER);

        // lecture du code du nuanceur
        const auto ns = textFileRead(nsStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char* ns_ptr = ns.c_str();

        // sourcer le nuanceur
        glShaderSource(nuanceurSommets, 1, &ns_ptr, nullptr);

        glCompileShader(nuanceurSommets);
        afficherShaderInfoLog(nuanceurSommets, "ERREURS DE COMPILATION DU NUANCEUR DE SOMMETS : ");
    }


    if( !ntcStr.empty() )
    {
        // indiquer la progression...
        printf( "Compilation du nuanceur de tesselation   : %s \n", ntcStr.c_str() );

        // créer le nuanceur en GLSL
        nuanceurTessCtrl = glCreateShader( GL_TESS_CONTROL_SHADER );

        // lecture du code du nuanceur
        const auto ntc = textFileRead( ntcStr );

        // créer un pointeur sur le texte du code du nuanceur
        const char* ntc_ptr = ntc.c_str();

        // sourcer le nuanceur
        glShaderSource( nuanceurTessCtrl, 1, &ntc_ptr, nullptr );

        glCompileShader( nuanceurTessCtrl );
        afficherShaderInfoLog( nuanceurTessCtrl, "ERREURS DE COMPILATION DU NUANCEUR DE TESSELATIO CTRL : " );
    }


    if( !nteStr.empty() )
    {
        // indiquer la progression...
        printf( "Compilation du nuanceur de tesselation   : %s \n", nteStr.c_str() );

        // créer le nuanceur en GLSL
        nuanceurTessEval = glCreateShader( GL_TESS_EVALUATION_SHADER );

        // lecture du code du nuanceur
        const auto nte = textFileRead( nteStr );

        // créer un pointeur sur le texte du code du nuanceur
        const char* nte_ptr = nte.c_str();

        // sourcer le nuanceur
        glShaderSource( nuanceurTessEval, 1, &nte_ptr, nullptr );

        glCompileShader( nuanceurTessEval );
        afficherShaderInfoLog( nuanceurTessEval, "ERREURS DE COMPILATION DU NUANCEUR DE TESSELATIO EVAL : " );
    }

    // création du NUANCEUR DE FRAGMENTS (si spécifié)
    if (!nfStr.empty())
    {
        // indiquer la progression...
        printf("Compilation du nuanceur de fragments : %s \n", nfStr.c_str());

        // créer le nuanceur en GLSL
        nuanceurFragments = glCreateShader(GL_FRAGMENT_SHADER);

        // lecture du code du nuanceur
        const auto nf = textFileRead(nfStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char* nf_ptr = nf.c_str();

        // sourcer le nuanceur
        glShaderSource(nuanceurFragments, 1, &nf_ptr, nullptr);

        glCompileShader(nuanceurFragments);
        afficherShaderInfoLog(nuanceurFragments, "ERREURS DE COMPILATION DU NUANCEUR DE FRAGMENTS : ");
    }

    // créer le programme des nuanceurs et lier
    prog_ = glCreateProgram();
    glAttachShader(prog_, nuanceurSommets);
    if( !ntcStr.empty() )
    {
        glAttachShader( prog_, nuanceurTessCtrl );
    }

    if( !nteStr.empty() )
    {
        glAttachShader( prog_, nuanceurTessEval );
    }
    glAttachShader(prog_, nuanceurFragments);
    glLinkProgram(prog_);

    // afficher les erreurs de compilation et de linkage
    afficherProgramInfoLog(prog_, "ERREURS DE L'EDITION DES LIENS : ");

    // marquer les nuanceurs compilés
    estCompileEtLie_ = true;
}

void CNuanceurProg::compilerEtLierNuanceurs(const std::string& nsStr, const std::string& nfStr)
{
    GLuint nuanceurSommets   = 0;
    GLuint nuanceurFragments = 0;

    if (!nsStr.empty())
    {
        // indiquer la progression...
        printf("Compilation du nuanceur de sommets   : %s \n", nsStr.c_str());

        // créer le nuanceur en GLSL
        nuanceurSommets = glCreateShader(GL_VERTEX_SHADER);

        // lecture du code du nuanceur
        const auto ns = textFileRead(nsStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char* ns_ptr = ns.c_str();

        // sourcer le nuanceur
        glShaderSource(nuanceurSommets, 1, &ns_ptr, nullptr);

        glCompileShader(nuanceurSommets);
        afficherShaderInfoLog(nuanceurSommets, "ERREURS DE COMPILATION DU NUANCEUR DE SOMMETS : ");
    }

    // création du NUANCEUR DE FRAGMENTS (si spécifié)
    if (!nfStr.empty())
    {
        // indiquer la progression...
        printf("Compilation du nuanceur de fragments : %s \n", nfStr.c_str());

        // créer le nuanceur en GLSL
        nuanceurFragments = glCreateShader(GL_FRAGMENT_SHADER);

        // lecture du code du nuanceur
        const auto nf = textFileRead(nfStr);

        // créer un pointeur sur le texte du code du nuanceur
        const char* nf_ptr = nf.c_str();

        // sourcer le nuanceur
        glShaderSource(nuanceurFragments, 1, &nf_ptr, nullptr);

        glCompileShader(nuanceurFragments);
        afficherShaderInfoLog(nuanceurFragments, "ERREURS DE COMPILATION DU NUANCEUR DE FRAGMENTS : ");
    }

    // créer le programme des nuanceurs et lier
    prog_ = glCreateProgram();
    glAttachShader(prog_, nuanceurSommets);
    glAttachShader(prog_, nuanceurFragments);
    glLinkProgram(prog_);

    // afficher les erreurs de compilation et de linkage
    afficherProgramInfoLog(prog_, "ERREURS DE L'EDITION DES LIENS : ");

    // marquer les nuanceurs compilés
    estCompileEtLie_ = true;
}

GLuint CNuanceurProg::getProg()
{
    return prog_;
}
