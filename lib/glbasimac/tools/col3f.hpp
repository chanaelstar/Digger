/***************************************************************************
                        col3f.hpp  -  description
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

#ifndef _STP3D_COL3F_HPP_
#define _STP3D_COL3F_HPP_

#include <iostream>
#include <cmath>
#include "globals.hpp"

namespace STP3D {

	/**
	  * \brief Col3f is a 3 float structure representing a color. 
	  * 
	  */
	struct Col3f
	{
		float r, g, b;

		////////////////////////////////////////////////////////////////////////////////
		/// Construction
		////////////////////////////////////////////////////////////////////////////////

		inline Col3f           ( )                    { r = 0.0; g = 0.0; b = 0.0; }
		inline Col3f           ( const Col3f& other ) { r = other.r; g = other.g; b = other.b; }
		inline Col3f& operator=( const Col3f& other ) { r = other.r; g = other.g; b = other.b; return *this; }

		inline explicit Col3f (const float& v)                         : r(v), g(v), b(v) {}
		inline explicit Col3f (const float* v, int stride = 1)         : r(v[0*stride]), g(v[1*stride]), b(v[2*stride]) {}
		inline          Col3f (const float& r, const float& g, const float& b) : r(r), g(g), b(b) {}

	};


	////////////////////////////////////////////////////////////////////////////////
	/// Unary Operators
	////////////////////////////////////////////////////////////////////////////////
	inline const Col3f operator+ (const Col3f& v) { return Col3f(+v.r,+v.g,+v.b); }
	inline const Col3f operator- (const Col3f& v) { return Col3f(-v.r,-v.g,-v.b); }
	inline const Col3f abs       (const Col3f& a) { return Col3f(std::fabs(a.r),std::fabs(a.g),std::fabs(a.b)); }
	inline const Col3f rcp       (const Col3f& a) { return Col3f(1.0/a.r,1.0/a.g,1.0/a.b); }
	inline const Col3f sqrt      (const Col3f& a) { return Col3f(std::sqrt(a.r),std::sqrt(a.g),std::sqrt(a.b)); }

	////////////////////////////////////////////////////////////////////////////////
	/// Binary Operators
	////////////////////////////////////////////////////////////////////////////////
	inline const Col3f operator+(const Col3f& a, const Col3f& b) { return Col3f(a.r+b.r,a.g+b.g,a.b+b.b); }
	inline const Col3f operator-(const Col3f& a, const Col3f& b) { return Col3f(a.r-b.r,a.g-b.g,a.b-b.b); }
	inline const Col3f operator*(const float& a, const Col3f& b) { return Col3f(a*b.r,a*b.g,a*b.b); }
	inline const Col3f operator*(const Col3f& a, const float& b) { return Col3f(a.r*b,a.g*b,a.b*b); }
	inline const Col3f operator*(const Col3f& a, const Col3f& b) { return Col3f(a.r*b.r,a.g*b.g,a.b*b.b); }
	inline const Col3f operator/(const Col3f& a, const Col3f& b) { return Col3f(a.r/b.r,a.g/b.g,a.b/b.b); }
	inline const Col3f operator/(const Col3f& a, const float& b) { return Col3f(a.r/b,a.g/b,a.b/b); }

	inline const Col3f min(const Col3f a, const Col3f b) { return Col3f(min(a.r,b.r),min(a.g,b.g),min(a.b,b.b)); }
	inline const Col3f max(const Col3f a, const Col3f b) { return Col3f(max(a.r,b.r),max(a.g,b.g),max(a.b,b.b)); }

	////////////////////////////////////////////////////////////////////////////////
	/// Assignment Operators
	////////////////////////////////////////////////////////////////////////////////

	inline const Col3f operator+=(Col3f& a, const Col3f& b) { return a = a + b; }
	inline const Col3f operator-=(Col3f& a, const Col3f& b) { return a = a - b; }
	inline const Col3f operator*=(Col3f& a, const Col3f& b) { return a = a * b; }
	inline const Col3f operator/=(Col3f& a, const Col3f& b) { return a = a / b; }
	inline const Col3f operator*=(Col3f& a, const float& b)       { return a = a * b; }
	inline const Col3f operator/=(Col3f& a, const float& b)       { return a = a / b; }

	////////////////////////////////////////////////////////////////////////////////
	/// Reduction Operators
	////////////////////////////////////////////////////////////////////////////////

	inline const float reduce_add(const Col3f& a) { return a.r+a.g+a.b; }
	inline const float reduce_mul(const Col3f& a) { return a.r*a.g*a.b; }
	inline const float reduce_min(const Col3f& a) { return min(a.r,a.g,a.b); }
	inline const float reduce_max(const Col3f& a) { return max(a.r,a.g,a.b); }

	////////////////////////////////////////////////////////////////////////////////
	/// Comparison Operators
	////////////////////////////////////////////////////////////////////////////////

	inline bool operator ==(const Col3f& a, const Col3f& b) { return a.r == b.r && a.g == b.g && a.b == b.b; }
	inline bool operator !=(const Col3f& a, const Col3f& b) { return a.r != b.r || a.g != b.g || a.b != b.b; }
	inline bool operator < (const Col3f& a, const Col3f& b ) {
		if (a.r != b.r) return a.r < b.r;
		if (a.g != b.g) return a.g < b.g;
		if (a.b != b.b) return a.b < b.b;
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////
	/// Special Operators
	////////////////////////////////////////////////////////////////////////////////

	/*! computes luminance of a color */
	inline const float luminance (const Col3f& a) { return 0.212671f*a.r + 0.715160f*a.g + 0.072169f*a.b; }

	inline Col3f exp (const Col3f& a) { return Col3f(std::exp(a.r),std::exp(a.g),std::exp(a.b)); }
	inline Col3f log (const Col3f& a) { return Col3f(std::log(a.r),std::log(a.g),std::log(a.b)); }
	inline Col3f pow (const Col3f& a, float e) { return exp(log(max(Col3f(1E-10f),a))*e); }

	/*! output operator */
	inline std::ostream& operator<<(std::ostream& cout, const Col3f& a) {
	  return cout << "(" << a.r << ", " << a.g << ", " << a.b << ")";
	}
} // fin namespace

#endif
