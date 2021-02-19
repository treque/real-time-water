///////////////////////////////////////////////////////////////////////////////
///  @file TextureAbstraite.h
///  @author  Frédéric Plourde
///  @brief   Déclare la classe CTextureAbstraite qui encapsule une texture
///           openGL générique
///  @date    2007-12-13
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////
#include "TextureAbstraite.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
///  public constructor  CTextureAbstraite \n
///  Cconstruite une texture abstraite
///  Elle est abstraite afin de permettre le polymorphisme des textures en 2D, ou cubemap, ou etc...
///
///  @param [in, out]  nomFichier char *    le nom de fichier de la texture
///  @param [in]       cible GLenum         la cible openGL associée à la texture
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CTextureAbstraite::CTextureAbstraite(const std::string& nomFichier, const GLenum cible, const bool mipmap,
                                     const bool genTex)
    : nomFichier_(nomFichier)
    , CIBLE_(cible)
    , derniereUniteTexture_(GL_NONE)
    , mipmap_(mipmap)
{
    if (genTex)
    {
        // créer la texture
        glGenTextures(1, &nomTexture_);
        glBindTexture(CIBLE_, nomTexture_);
    }
}

CTextureAbstraite::CTextureAbstraite(GLenum cible, bool mipmap, bool genTex)
    : CIBLE_(cible)
    , derniereUniteTexture_(GL_NONE)
    , mipmap_(mipmap)
{
    if (genTex)
    {
        // créer la texture
        glGenTextures(1, &nomTexture_);
        glBindTexture(CIBLE_, nomTexture_);
    }
}

///////////////////////////////////////////////////////////////////////////////
///  public destructor  ~CTextureAbstraite \n
///  Destructeur par défaut d'une texture générique. Libère la mémoire de texture
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
CTextureAbstraite::~CTextureAbstraite(void)
{
    // destruction du nom de texture associé afin de le libérer pour d'autres textures
    const GLuint textureADetruire = nomTexture_;
    glDeleteTextures(1, &textureADetruire);
}

///////////////////////////////////////////////////////////////////////////////
///  protected pure virtual  chargerTexture \n
///  Fonction virtuelle PURE servant d'interface au chargement de textures
///  dans les versions spécialisées. Ne devrait JAMAIS être implémentée.
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
void CTextureAbstraite::chargerTexture()
{
    // NE SERA JAMAIS IMPLÉMENTÉE À CE NIVEAU HIÉRARCHIQUE !
    // devra être implémentée dans les versions enfants pour spécialisation
}

///////////////////////////////////////////////////////////////////////////////
///  public  appliquer \n
///  Applique la texture au niveau d'openGL
///
///  @param [in]       flagParams uint    les différents paramètres à spécifier. Voir le haut du fichier
///                                       textureAbstraite.h pour les différents tag à insérer ici, séparés
///                                       par des ou " | ".
///  @param [in]       gl_texture_wrap_s GLenum       le wrap_s de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_wrap_t GLenum       le wrap_t de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_wrap_r GLenum       le wrap_r de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_min_filter GLenum   le min_filter de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_mag_filter GLenum   le mag_filter de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_min_lod GLenum      le min_lod de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_max_lod GLenum      le max_lod de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_base_level GLenum   le base_level de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_max_level GLenum    le max_level de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_border_color GLenum le border_color de la texture, si spéficié dans le flag
///  @param [in]       gl_texture_priority GLenum     le priority de la texture, si spéficié dans le flag
///  @param [in]       mipmapping bool                indique si l'on veut activer le mipmapping ou non
///  @param [in]       uniteTexture GLenum [=0]       indique quelle unité de texture sera associée. Si aucune -> passer
///  GL_NONE
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
void CTextureAbstraite::appliquer(uint flagParams, GLenum gl_texture_wrap_s, GLenum gl_texture_wrap_t,
                                  GLenum gl_texture_wrap_r, GLenum gl_texture_min_filter, GLenum gl_texture_mag_filter,
                                  GLenum gl_texture_min_lod, GLenum gl_texture_max_lod, GLenum gl_texture_base_level,
                                  GLenum gl_texture_max_level, GLenum gl_texture_border_color,
                                  GLenum gl_texture_priority, bool mipmapping, GLenum uniteTexture)
{
    // activer l'unité de texture désiré, s'il y a lieu
    if (uniteTexture != GL_NONE)
    {
        derniereUniteTexture_ = uniteTexture;
        glActiveTexture(uniteTexture);
    }

    // ensuite, lier la texture avec la cible de classe GL_TEXTURE_2D
    glBindTexture(CIBLE_, nomTexture_);
    glEnable(CIBLE_);

    // activer le mipmapping automatique si désiré
    if (mipmapping)
    {
        glTexParameteri(CIBLE_, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    }

    // activer l'anisotropie
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
    glTexParameterf(CIBLE_, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);

    // fixer les différents paramètres choisis par l'appelant
    if (flagParams != 0)
    {
        if ((flagParams & WRAP_S) == WRAP_S)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_WRAP_S, gl_texture_wrap_s);
        }

        if ((flagParams & WRAP_T) == WRAP_T)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_WRAP_T, gl_texture_wrap_t);
        }

        if ((flagParams & WRAP_R) == WRAP_R)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_WRAP_R, gl_texture_wrap_r);
        }

        if ((flagParams & MIN_FILTER) == MIN_FILTER)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_MIN_FILTER, gl_texture_min_filter);
        }

        if ((flagParams & MAG_FILTER) == MAG_FILTER)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_MAG_FILTER, gl_texture_mag_filter);
        }

        if ((flagParams & MIN_LOD) == MIN_LOD)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_MIN_LOD, gl_texture_min_lod);
        }

        if ((flagParams & MAX_LOD) == MAX_LOD)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_MAX_LOD, gl_texture_max_lod);
        }

        if ((flagParams & BASE_LEVEL) == BASE_LEVEL)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_BASE_LEVEL, gl_texture_base_level);
        }

        if ((flagParams & MAX_LEVEL) == MAX_LEVEL)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_MAX_LEVEL, gl_texture_max_level);
        }

        if ((flagParams & BORDER_COLOR) == BORDER_COLOR)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_BORDER_COLOR, gl_texture_border_color);
        }

        if ((flagParams & PRIORITY) == PRIORITY)
        {
            glTexParameteri(CIBLE_, GL_TEXTURE_PRIORITY, gl_texture_priority);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///  public  desactiver \n
///  Désactive le texturage sur la cible en cours. Très important de désactiver
///  le texturage entre les différents objets openGL spécifiés, car deux cibles
///  différentes activées simultanémant donnent lieu à des bugs bizarres en
///  openGL.
///
///  @return Aucune
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
void CTextureAbstraite::desactiver()
{
    // il est important de toujours désactiver la cible de texture
    // une fois celle-ci utilisée (compilée dans une liste), car
    // les prochains vertex à texturer auront peut-être une cible
    // différente, et plusieurs problèmes de texturage sont amenés
    // par le conflit de deux cibles de texturages actives simult.
    glDisable(CIBLE_);
    if (derniereUniteTexture_ != GL_NONE)
    {
        glActiveTexture(derniereUniteTexture_);
        glDisable(CIBLE_);
    }
}

///////////////////////////////////////////////////////////////////////////////
///  protected  loadBMP \n
///  Fonction utilitaire servant à charger en mémoire un BMP
///
///  @param [in, out]  nomFichier char *    le nom de fichier de la texture
///  @param [in, out]  image Image *        une variable de type Image qui recueillera les données
///
///  @return int <TODO: insérer une valeur de retour ici>
///
///  @author Frédéric Plourde
///  @date   2008-02-20
///
///////////////////////////////////////////////////////////////////////////////
int CTextureAbstraite::loadBMP(const std::string& nomFichier, Image* image)
{
    std::ifstream file(nomFichier, std::ios::binary);

    // make sure the file is there.
    if (!file.is_open())
    {
        std::cout << "Unable to open file " << nomFichier << std::endl;
        return 0;
    }

    // seek through the bmp header, up to the width/height:
    file.seekg(18, std::ios_base::cur);

    // read the width and height
    file.read(reinterpret_cast<char*>(&image->tailleX), 4);
    file.read(reinterpret_cast<char*>(&image->tailleY), 4);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    const auto size = static_cast<size_t>(image->tailleX * image->tailleY * 3);

    // read the planes
    unsigned short int numPlanes; // number of planes in image (must be 1)
    file.read(reinterpret_cast<char*>(&numPlanes), 2);

    if (numPlanes != 1)
    {
        std::cout << "Planes from " << nomFichier << " is not 1: " << numPlanes << std::endl;
        return 0;
    }

    // read the bpp
    unsigned short int bpp; // number of bits per pixel (must be 24)
    file.read(reinterpret_cast<char*>(&bpp), 2);

    if (bpp != 24)
    {
        std::cout << "Bpp from " << nomFichier << " is not 24: " << bpp << std::endl;
        return 0;
    }

    // seek past the rest of the bitmap header.
    file.seekg(24, std::ios_base::cur);

    // read the data.
    image->data.resize(size);
    file.read(reinterpret_cast<char*>(&image->data[0]), size);

    for (size_t i = 0; i < size; i += 3)
    { // reverse all of the colors. (bgr -> rgb)
        std::swap(image->data[i], image->data[i + 2]);
    }

    return 1; // OK
}

void CTextureAbstraite::genTex()
{
    // créer la texture
    glGenTextures(1, &nomTexture_);
    glBindTexture(CIBLE_, nomTexture_);
}
