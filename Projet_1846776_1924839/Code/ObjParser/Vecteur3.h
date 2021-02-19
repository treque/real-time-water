#ifndef VECTEUR_3_H
#define VECTEUR_3_H

#include <cassert>

#include "MathUtils.h"

namespace Math3D {
///////////////////////////////////////////////////////////////////////////////
///  CVecteur3
///  Encapsule un simple vecteur 3D [ x, y, z ]
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
class CVecteur3
{
public:
    /// vecteur nul
    static const CVecteur3 ZERO;
    /// Vecteur unitaire X
    static const CVecteur3 UNIT_X;
    /// Vecteur unitaire Y
    static const CVecteur3 UNIT_Y;
    /// Vecteur unitaire Z
    static const CVecteur3 UNIT_Z;

    /// Compostantes du vecteur
    REAL x, y, z;

    /// Les constructeurs
    inline CVecteur3(void);
    inline CVecteur3(const REAL X, const REAL Y, const REAL Z);
    inline explicit CVecteur3(const REAL* const Coordonnees);
    inline explicit CVecteur3(const REAL Scalaire);
    inline CVecteur3(const CVecteur3& Vecteur);

    /// Opérateurs unitaires
    inline REAL&      operator[](const int i);
    inline CVecteur3& operator=(const CVecteur3& Vecteur);

    inline bool operator==(const CVecteur3& Vecteur) const;
    inline bool operator!=(const CVecteur3& Vecteur) const;

    inline CVecteur3& operator+=(const CVecteur3& Vecteur);
    inline CVecteur3& operator-=(const CVecteur3& Vecteur);
    inline CVecteur3& operator*=(const REAL Scalaire);
    inline CVecteur3& operator/=(const REAL Scalaire);

    inline const CVecteur3 operator-(void) const;
    inline const CVecteur3 operator/(const REAL Scalaire) const;

    /// Opérateurs binaires
    inline friend const CVecteur3 operator+(const CVecteur3& V1, const CVecteur3& V2);
    inline friend const CVecteur3 operator-(const CVecteur3& V1, const CVecteur3& V2);
    inline friend const CVecteur3 operator*(const REAL Scalaire, const CVecteur3& Vecteur);
    inline friend const CVecteur3 operator*(const CVecteur3& Vecteur, const REAL Scalaire);

    inline friend std::ostream& operator<<(std::ostream& Out, const CVecteur3& Vecteur);

    /// Méthodes statiques
    inline static REAL            ProdScal(const CVecteur3& V1, const CVecteur3& V2);
    inline static const CVecteur3 ProdVect(const CVecteur3& V1, const CVecteur3& V2);
    inline static REAL            Norme(const CVecteur3& Vecteur);
    inline static REAL            Distance(const CVecteur3& V1, const CVecteur3& V2);
    inline static const CVecteur3 Normaliser(const CVecteur3& Vecteur);
    inline static const CVecteur3 Reflect(const CVecteur3& Vecteur, const CVecteur3& Normal);
    inline static const CVecteur3
        Refract(const CVecteur3& Vecteur, const CVecteur3& Normal, const REAL IndiceRefractionRatio);
};

///////////////////////////////////////////////////////////////////////////////
///  inline public overloaded constructor  CVecteur3 \n
///  Description : Constructeur par défaut
///
///  @return None
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3::CVecteur3(void)
    : x(0)
    , y(0)
    , z(0)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur de surcharge
///
///  @param [in]       X const Math3D::REAL    Vecteur element x
///  @param [in]       Y const Math3D::REAL    Vecteur element y
///  @param [in]       Z const Math3D::REAL    Vecteur element z
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3::CVecteur3(const REAL X, const REAL Y, const REAL Z)
    : x(X)
    , y(Y)
    , z(Z)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur de surcharge
///
///  @param [in]       Coordonnees const Math3D::REAL *const    pointeur sur tableau
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3::CVecteur3(const REAL* const Coordonnees)
    : x(Coordonnees[0])
    , y(Coordonnees[1])
    , z(Coordonnees[2])
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur de surcharge
///
///  @param [in]       Scalaire const Math3D::REAL    Éléments vecteurs comme étatn des scalaires
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3::CVecteur3(const REAL Scalaire)
    : x(Scalaire)
    , y(Scalaire)
    , z(Scalaire)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Constructeur par défaut
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Vecteur à copier
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3::CVecteur3(const CVecteur3& Vecteur)
    : x(Vecteur.x)
    , y(Vecteur.y)
    , z(Vecteur.z)
{
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'indexation du vecteur (lecteur/écriture)
///
///  @param [in]       i const int    L'index
///
///  @return Math3D::REAL & Référence à la valeur de l'élément
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline REAL& CVecteur3::operator[](const int i)
{
    assert(i > -1 && i < 3);
    return *(&x + i);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de copie
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Vecteur à copier
///
///  @return Math3D::CVecteur3 & Le vecteur courant
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3& CVecteur3::operator=(const CVecteur3& Vecteur)
{
    x = Vecteur.x;
    y = Vecteur.y;
    z = Vecteur.z;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'égalité
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à tester
///
///  @return bool Retourne vrai si les vecteurs contiennent les mêmes valeurs, sinon retourn faux
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline bool CVecteur3::operator==(const CVecteur3& Vecteur) const
{
    REAL dx = std::abs(x - Vecteur.x);
    REAL dy = std::abs(y - Vecteur.y);
    REAL dz = std::abs(z - Vecteur.z);

    return (dx < EPSILON && dy < EPSILON && dz < EPSILON);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de différence
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à tester
///
///  @return bool Retourne faux si les vecteurs ont les mêmes valeurs, sinon vrai
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline bool CVecteur3::operator!=(const CVecteur3& Vecteur) const
{
    return !(*this == Vecteur);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur d'addition
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à ajouter
///
///  @return Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne @date 11/08/2008
///
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3& CVecteur3::operator+=(const CVecteur3& Vecteur)
{
    x += Vecteur.x;
    y += Vecteur.y;
    z += Vecteur.z;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de soustraction
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à soustraire
///
///  @return Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3& CVecteur3::operator-=(const CVecteur3& Vecteur)
{
    x -= Vecteur.x;
    y -= Vecteur.y;
    z -= Vecteur.z;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Opérateur de multiplication scalaire
///
///  @param [in]       Scalaire const Math3D::REAL    Le scalaire à multiplier
///
///  @return Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3& CVecteur3::operator*=(const REAL Scalaire)
{
    x *= Scalaire;
    y *= Scalaire;
    z *= Scalaire;
    return (*this);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de division
///
///  @param [in]       Scalaire const Math3D::REAL    Le scalaire à diviser
///
///  @return Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline CVecteur3& CVecteur3::operator/=(const REAL Scalaire)
{
    REAL invScalaire = 0;
    assert(std::abs(Scalaire) > EPSILON);
    invScalaire = REAL(1) / Scalaire;
    return ((*this) *= invScalaire);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de négation
///
///  @return const Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 CVecteur3::operator-() const
{
    return CVecteur3(-x, -y, -z);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de division scalaire
///
///  @param [in]       Scalaire const Math3D::REAL    Le scalaire à diviser
///
///  @return Math3D::CVecteur3 Le vecteur résultant
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 CVecteur3::operator/(const REAL Scalaire) const
{
    REAL invScalaire = 0;
    assert(std::abs(Scalaire) > EPSILON);
    invScalaire = REAL(1.0) / Scalaire;

    return CVecteur3(x * invScalaire, y * invScalaire, z * invScalaire);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur d'addition vectorielle
///
///  @param [in]       V1 const Math3D::CVecteur3 &    V1
///  @param [in]       V2 const Math3D::CVecteur3 &    V2
///
///  @return Math3D::CVecteur3   le résultat
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 operator+(const CVecteur3& V1, const CVecteur3& V2)
{
    return CVecteur3(V1.x + V2.x, V1.y + V2.y, V1.z + V2.z);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de soustraction vectorielle
///
///  @param [in]       V1 const Math3D::CVecteur3 &    V1
///  @param [in]       V2 const Math3D::CVecteur3 &    V2
///
///  @return Math3D::CVecteur3   le résultat
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 operator-(const CVecteur3& V1, const CVecteur3& V2)
{
    return CVecteur3(V1.x - V2.x, V1.y - V2.y, V1.z - V2.z);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de multiplication scalaire/vecteur
///
///  @param [in]       Scalaire const Math3D::REAL          Scalaire
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Vecteur
///
///  @return Math3D::CVecteur3   le résultat
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 operator*(const REAL Scalaire, const CVecteur3& Vecteur)
{
    return CVecteur3(Vecteur.x * Scalaire, Vecteur.y * Scalaire, Vecteur.z * Scalaire);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de multiplication vecteur/scalaire
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Vecteur
///  @param [in]       Scalaire const Math3D::REAL          Scalaire
///
///  @return Math3D::CVecteur3   Le résultat
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 operator*(const CVecteur3& Vecteur, const REAL Scalaire)
{
    return CVecteur3(Vecteur.x * Scalaire, Vecteur.y * Scalaire, Vecteur.z * Scalaire);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de flux de sortie
///
///  @param [in,out]  Out std::ostream &    Le flux de sortie
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à afficher
///
///  @return std::ostream & Le flux de sortie modifié
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator<<(std::ostream& Out, const CVecteur3& Vecteur)
{
    Out << std::setprecision(3) << "[ " << std::setw(3) << Vecteur.x << ", " << Vecteur.y << ", " << Vecteur.z << " ]"
        << std::setw(0);
    return Out;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de produit scalaire entre vecteurs
///
///  @param [in]       V1 const Math3D::CVecteur3 &    V1
///  @param [in]       V2 const Math3D::CVecteur3 &    V2
///
///  @return Math3D::REAL  le résultat du produit scalaire
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline REAL CVecteur3::ProdScal(const CVecteur3& V1, const CVecteur3& V2)
{
    return (V1.x * V2.x + V1.y * V2.y + V1.z * V2.z);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief opérateur de produit vectoriel entre vecteurs
///
///  @param [in]       V1 const Math3D::CVecteur3 &    V1
///  @param [in]       V2 const Math3D::CVecteur3 &    V2
///
///  @return Math3D::CVecteur3   le résultat du produit vectoriel
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 CVecteur3::ProdVect(const CVecteur3& V1, const CVecteur3& V2)
{
    return CVecteur3(V1.y * V2.z - V1.z * V2.y, V1.z * V2.x - V1.x * V2.z, V1.x * V2.y - V1.y * V2.x);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief retourne la norme d'un vecteur
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Vecteur
///
///  @return Math3D::REAL   La norme du vecteur
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline REAL CVecteur3::Norme(const CVecteur3& Vecteur)
{
    return std::sqrt(Vecteur.x * Vecteur.x + Vecteur.y * Vecteur.y + Vecteur.z * Vecteur.z);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief retourne la distance entre deux vecteurs
///
///  @param [in]       V1 const Math3D::CVecteur3 &    V1
///  @param [in]       V2 const Math3D::CVecteur3 &    V2
///
///  @return Math3D::REAL   la distance
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline REAL CVecteur3::Distance(const CVecteur3& V1, const CVecteur3& V2)
{
    return Norme(V1 - V2);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Normalise un vecteur
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &    Le vecteur à normaliser
///
///  @return Math3D::CVecteur3   Le vecteur normalisé
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 CVecteur3::Normaliser(const CVecteur3& Vecteur)
{
    CVecteur3 Result;
    REAL      VecLength = Norme(Vecteur);

    if (VecLength > EPSILON)
    {
        VecLength = REAL(1.0) / VecLength;
        Result    = Vecteur * VecLength;
    }

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Effectue la réflexion d'un vecteur selon une normale spécifiée
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &   Le vecteur à réfléchir
///  @param [in]       Normal const Math3D::CVecteur3 &    La normale
///
///  @return Math3D::CVecteur3   Le vecteur réfléchi
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3 CVecteur3::Reflect(const CVecteur3& Vecteur, const CVecteur3& Normal)
{
    return (Vecteur - (REAL(2.0) * ProdScal(Vecteur, Normal) * Normal));
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Effectue la réfraction d'un vecteur selon une normale spécifiée et un indice de réfraction
///
///  @param [in]       Vecteur const Math3D::CVecteur3 &   Le vecteur à réfracter
///  @param [in]       Normal const Math3D::CVecteur3 &    La normale
///  @param [in]       IndiceRefractionRatio const Math3D::REAL Le ratio des deux indices de réfractions
///
///  @return Math3D::CVecteur3   Le vecteur réfracté
///
///  @author Olivier Dionne @date 11/08/2008
///////////////////////////////////////////////////////////////////////////////
inline const CVecteur3
    CVecteur3::Refract(const CVecteur3& Vecteur, const CVecteur3& Normal, const REAL IndiceRefractionRatio)
{
    CVecteur3 Result;

    // La formule de Heckbert, fournie par [The Computer Image written by Alan Watt - Chapter 24.4.6 - ]
    // Si l'angle est supérieur à l'angle critique, on obtient une réflexion totale interne
    REAL Cos1   = -CVecteur3::ProdScal(Vecteur, Normal);
    REAL Cos2Sq = (REAL(1.0) - (IndiceRefractionRatio * IndiceRefractionRatio) * (REAL(1.0) - (Cos1 * Cos1)));

    if (Cos2Sq < 0)
        Result = Reflect(Vecteur, Normal);
    else
    {
        // if squared cos is positive then there is a internal reflection
        Result = IndiceRefractionRatio * Vecteur - ((std::sqrt(Cos2Sq) - (IndiceRefractionRatio * Cos1)) * Normal);
    }

    return Result;
}

} // namespace Math3D

#endif
