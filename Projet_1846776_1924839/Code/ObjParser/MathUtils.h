///////////////////////////////////////////////////////////////////////////////
///  @file MathUtils.h
///
///  @brief    Ce fichier contient plusieurs constantes et fonctions mathématiques utiles.
///
///  @author  Olivier Dionne
///  @date    13/08/2008
///  @version 1.0.0
///
///////////////////////////////////////////////////////////////////////////////
#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>
#include <iomanip>
#include <iostream>

namespace Math3D {
///////////////////// TYPEDEFS ///////////////////////
using REAL = float;

/////////////////////// DEFINES ////////////////////////
constexpr REAL PI =
#ifdef M_PI
    REAL(M_PI);
#else
    3.14159265359;
#endif

const REAL EPSILON = REAL(1e-6);

///////////////////// FONCTIONS ///////////////////////

///////////////////////////////////////////////////////////////////////////////
///  @brief Borne la valeur du paramètre fourni entre deux valeurs frontières
///
///  @param [in]       Val const T &    La valeur à borner
///  @param [in]       Min const T &    La frontière min
///  @param [in]       Max const T &    La frontière max
///
///  @return const T La valeur bornée
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline const T Borner(const T& Val, const T& Min, const T& Max)
{
    return (Val < Min ? Min : (Val > Max ? Max : Val));
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Convertit les degrés en radians
///
///  @param [in]       A const T &    Les degrés à convertir
///
///  @return const T La valeur en radians
///
///  @author Olivier Dionne
///  @date   13/08/2008
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline const T Deg2Rad(const T& A)
{
    return (A / T(180.0)) * T(PI);
}

///////////////////////////////////////////////////////////////////////////////
///  @brief Convertit les radians en degrés
///
///  @param [in]       A const T &    Les radians à convertir
///
///  @return const T La valeur en degrés
///
///  @author Olivier Dionne
///  @date   13/08/2008
///
///////////////////////////////////////////////////////////////////////////////
template <typename T>
inline const T Rad2Deg(const T& A)
{
    return (A / T(PI)) * T(180.0);
}

} // namespace Math3D

#endif
