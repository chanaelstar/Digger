/***************************************************************************
                        globals.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri
    email                : biri@univ-mlv.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   * 
 *                                                                         *
 ***************************************************************************/

#ifndef _STP3D_GLOBALS_H
#define _STP3D_GLOBALS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

/** \namespace STP3D 
  * STP3D for Simple_Teaching_Platform_for_3D is a simple (and naive) C++ 3D library for 3D programming. 
  * Inspired by GLM
  * Deals essentially with basic maths, basic image loading, basic shader and mesh handling, basic stufs. 
  * \author Venceslas BIRI
  */
namespace STP3D {

#define STP3D_VERSION 0.2
#define STP3D_EPSILON 1e-6

// ///////////////////////////////////////////////////////////////////////////
// Error msg mechanism
// ///////////////////////////////////////////////////////////////////////////
static std::string errSTP3D_Msg = std::string("No Error");
inline bool errorOccured() { return (strcmp(errSTP3D_Msg.c_str(),"No Error")!=0);}
inline std::string getError() {return errSTP3D_Msg;}
inline void setError(std::string inputErrMsg) { errSTP3D_Msg = inputErrMsg;}
inline void eraseError() { errSTP3D_Msg = std::string("No Error");}


// ///////////////////////////////////////////////////////////////////////////
// Some basic useful tool
// ///////////////////////////////////////////////////////////////////////////
inline std::string intToString(int in) {std::stringstream out;out<<in;return out.str();}

// ///////////////////////////////////////////////////////////////////////////
// Some basic mathematic constant (Zero and One)
// ///////////////////////////////////////////////////////////////////////////
static struct ZeroTy
{
	inline operator          double   ( ) const { return 0; }
	inline operator          float    ( ) const { return 0; }
	inline operator          long long( ) const { return 0; }
	inline operator unsigned long long( ) const { return 0; }
	inline operator          long     ( ) const { return 0; }
	inline operator unsigned long     ( ) const { return 0; }
	inline operator          int      ( ) const { return 0; }
	inline operator unsigned int      ( ) const { return 0; }
	inline operator          short    ( ) const { return 0; }
	inline operator unsigned short    ( ) const { return 0; }
	inline operator          char     ( ) const { return 0; }
	inline operator unsigned char     ( ) const { return 0; }
} zero;

static struct OneTy
{
	inline operator          double   ( ) const { return 1; }
	inline operator          float    ( ) const { return 1; }
	inline operator          long long( ) const { return 1; }
	inline operator unsigned long long( ) const { return 1; }
	inline operator          long     ( ) const { return 1; }
	inline operator unsigned long     ( ) const { return 1; }
	inline operator          int      ( ) const { return 1; }
	inline operator unsigned int      ( ) const { return 1; }
	inline operator          short    ( ) const { return 1; }
	inline operator unsigned short    ( ) const { return 1; }
	inline operator          char     ( ) const { return 1; }
	inline operator unsigned char     ( ) const { return 1; }
} one;


// ///////////////////////////////////////////////////////////////////////////
// Some basic mathematic tool (beware template inside)
// ///////////////////////////////////////////////////////////////////////////
template<typename T> inline T min(const T& a, const T& b)                                     { return a<b? a:b; }
template<typename T> inline T min(const T& a, const T& b, const T& c)                         { return min(min(a,b),c); }
template<typename T> inline T min(const T& a, const T& b, const T& c, const T& d)             { return min(min(a,b),min(c,d)); }
template<typename T> inline T min(const T& a, const T& b, const T& c, const T& d, const T& e) { return min(min(min(a,b),min(c,d)),e); }

template<typename T> inline T max(const T& a, const T& b)                                     { return a<b? b:a; }
template<typename T> inline T max(const T& a, const T& b, const T& c)                         { return max(max(a,b),c); }
template<typename T> inline T max(const T& a, const T& b, const T& c, const T& d)             { return max(max(a,b),max(c,d)); }
template<typename T> inline T max(const T& a, const T& b, const T& c, const T& d, const T& e) { return max(max(max(a,b),max(c,d)),e); }

template<typename T> inline T clamp(const T& x, const T& lower = T(zero), const T& upper = T(one)) { return max(lower, min(x,upper)); }


} // Namespace Ends

#endif // _STP3D_GLOBALS_H