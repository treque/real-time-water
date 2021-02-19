///////////////////////////////////////////////////////////////////////////////
///  @file Texture2D.h
///  @author  Frédéric Plourde 
///  @brief   Déclare la classe CTexture2D qui implémente une texture2D openGL
///  @date    2008-01-18
///  @version 1.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TextureAbstraite.h"

///////////////////////////////////////////////////////////////////////////////
///  CTexture2D
///  Cette classe encapsule une texture 2D openGL. (type GL_TEXTURE_2D)
///
///  Base class CTextureAbstraite <TODO: insérer la description de la classe de base ici>
///
///  @remarks La classe possède deux constructeurs. Un premier ne prenant qu'un nom de 
///           fichier, est utile pour charger des images 2D à partir du disque (BMP
///           uniquement !), et uu deuxième, prenant un vect3D* data, est utile pour 
///           créer une texture 2D openGL à partir de données déjà existantes en mémoire.
///           à noter cependant que cette version ne prend QUE des vect3D, i.e. une 
///           texture 2D en format GL_FLOAT. Pour d'autres supports de formats, 
///           veuillez ajouter les constructeurs appropriés et le code associé.
///
///  @author Frédéric Plourde 
///  @date   2008-03-11   <TODO: insérer initiales ici> <TODO: insérer le texte ici> 
///
///////////////////////////////////////////////////////////////////////////////
class CTexture2D :
   public CTextureAbstraite
{
public:
   /// constructeur d'une texture 2D openGL (de type GL_TEXTURE_2D) à partir du nom de fichier de la texture
   CTexture2D(char* filename, bool mipmap = false);
   /// constructeur d'une texture 2D openGL à partir d'un poiteur sur des données float
   CTexture2D(vect3D* data, int hauteur, int largeur, bool mipmap = false);
   /// destructeur par défaut
   ~CTexture2D(void);

private:
   /// charge la texture en mémoire avec la cible GL_TEXTURE_2D
   virtual void chargerTexture(); // devrait toujours être privée
   /// membre privé temporaire servant la conserver les texels lus du fichier
   Image TextureImage;
};
