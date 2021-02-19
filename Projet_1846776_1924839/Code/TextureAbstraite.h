///////////////////////////////////////////////////////////////////////////////
///  @file TextureAbstraite.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe abstraite CTextureAbstraite qui encapsule
///           l'interface générique d'une texture openGL
///  @date    2008-01-17
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>

#include "Cst.h"
#include "Var.h"

// définition plus commode (et abrégée) du type "unsigned int" (16-bit integer - word)
typedef unsigned int uint;

// définition des différents bits utilisés pour le choix des paramètres de texturage
const uint WRAP_S       = 0x0001;
const uint WRAP_T       = 0x0002;
const uint WRAP_R       = 0x0004;
const uint MIN_FILTER   = 0x0008;
const uint MAG_FILTER   = 0x0010;
const uint MIN_LOD      = 0x0020;
const uint MAX_LOD      = 0x0040;
const uint BASE_LEVEL   = 0x0080;
const uint MAX_LEVEL    = 0x0100;
const uint BORDER_COLOR = 0x0200;
const uint PRIORITY     = 0x0400;

/// définition des énumérations de types de texture
enum TEXTypeEnum
{
    TYPE_TEXTURE2D,
    TYPE_TEXTURE_CUBEMAP,
    TYPE_TEXTURE_CUBEMAP_HDR
};

///////////////////////////////////////////////////////////////////////////////
///  @class CTextureAbstraite
///  @brief Cette classe encapsule une texture générique openGL.
///
///  @remarks la fonction membre virtuelle pure chargerTexture() doit être
///         implémentée à la saveur de la texture surchargée. Par exemple,
///         une texture HDR surchargera la fonction chargerImage() de façon
///         à charger en mémoire une image HDR. La seule fonction qui est
///         publiée au public, est "appliquerTexture(...) qui permet les trois choses
///         suivantes :
///              1) Active l'unité de texture désirée (pour le multitexturage)
///              2) active le texturage sur la cible sélectionnée
///              3) Lie (bind) la texture à la cible associée
///
///  @author Frédéric Plourde
///  @date   2008-01-17
///
///////////////////////////////////////////////////////////////////////////////
class CTextureAbstraite
{
public:
    /// constructeur spécifique
    CTextureAbstraite(const std::string& nomFichier, const GLenum cible, const bool mipmap = false,
                      const bool genTex = true);
    /// constructeur spécifique
    CTextureAbstraite(GLenum cible, bool mipmap = false, bool genTex = true);
    /// destructeur par défaut
    virtual ~CTextureAbstraite(void);

    /// applique la texture dans openGL et spécifie les paramètres de texturage
    void appliquer(uint flagParams, GLenum gl_texture_wrap_s, GLenum gl_texture_wrap_t, GLenum gl_texture_wrap_r,
                   GLenum gl_texture_min_filter, GLenum gl_texture_mag_filter, GLenum gl_texture_min_lod,
                   GLenum gl_texture_max_lod, GLenum gl_texture_base_level, GLenum gl_texture_max_level,
                   GLenum gl_texture_border_color, GLenum gl_texture_priority, bool mipmapping,
                   GLenum uniteTexture = GL_NONE);

    /// désactive la texture (sous la cible associée) dans openGL
    void desactiver();

protected:
    /// méthode virtuelle pure de chargement de la texture en mémoire qui devrait toujours être implémentée dans la
    /// version surchargée
    virtual void chargerTexture() = 0; // devrait toujours être surchargée "privée"

    void genTex();

    /// fonction utilitaire servant à charger un BMP en mémoire à partir d'un nom de fichier
    int loadBMP(const std::string& nomFichier, Image* image);

    /// le nom de fichier de la texture
    std::string nomFichier_;
    /// le nom de texture openGL associé à cette texture
    GLuint nomTexture_;
    /// la cible openGL associée à cette texture
    const GLenum CIBLE_;
    /// la dernière unité de texture (GL_TEXTURE0, GL_TEXTURE1, etc...) appliquée à cette texture
    GLenum derniereUniteTexture_;
    /// doit-on mipmaper ?
    bool mipmap_;
};
