/***************************************************************************
                        vector3d.hpp  -  description
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

#ifndef _STP3D_VECTOR3D_HPP_
#define _STP3D_VECTOR3D_HPP_

#include <iostream>
#include <cmath>
#include "globals.hpp"

namespace STP3D {

/** Class representing a 3D vector.
  * Core class for any 3D programming. We have keept it (VERY) simply. It is naive programming.
  * @author Venceslas BIRI (biri@univ-mlv.fr)
  */
class Vector3D {
public:
	/** \name Constructors & Destructors
	 */
	//@{
	/// Default constructor.
	/// Creates the vector (0,0,0)
	Vector3D() {val[0] = val[1] = val[2] = 0.0f;};
	/// Set constructor.
	/// All coordinate will have the parameter value
	/// \param in parameter used to fill all coordinates
	explicit Vector3D(float in) {val[0] = val[1] = val[2] = in;};
	/// Set constructor.
	/// \param in_a,in_b,in_c coordinates of the vector
	Vector3D(float in_a,float in_b,float in_c) {val[0] = in_a;val[1] = in_b; val[2] = in_c;};
	/// Copy constructor.
	/// \param P1 source vector
	Vector3D(const Vector3D& src) {val[0] = src.val[0];val[1] = src.val[1];val[2] = src.val[2];};
	/** Two point constructor.
	  * Compute vector P1P2
	  * \param P1,P2 Two points creating the resulting vector P1P2
	  */
	Vector3D(const Vector3D& P1,const Vector3D& P2) {
		val[0] = P2.val[0] - P1.val[0];
		val[1] = P2.val[1] - P1.val[1];
		val[2] = P2.val[2] - P1.val[2];
	}
	/// Destructor.
	~Vector3D() {};
	//@}

	/** \name Coordinates (public access)
	  */
	//@{
	union {
		struct {
			float x, y, z;
		};
		float val[3];
	};
	//@}

	/** \name Set functions
	  * These functions allow to avoid the recreation of the vector
	  */
	//@{
	/// Set from three reals
	/// \param nwa,nwb,nwc New coordinates
	inline void set(float nwa,float nwb,float nwc) {val[0] = nwa;val[1] = nwb;val[2] = nwc;};
	/// Set from one vector
	/// \param src Source vector
	inline void set(const Vector3D& src) {val[0] = src.val[0];val[1] = src.val[1];val[2] = src.val[2];};
	//@}

	/** \name Utility functions
	  */
	//@{
	/** Return a perpendical vector.
	  * To do that, we look if one of the two coordinates x ou y is not null.
	  * If this is the case, the return vector is (-b,a,0) <b>normalized</b> else it's (0,0,1)
	  */
	Vector3D getOrthog() const;

	/// Return the vector's norm.
	inline float norme() const {	return std::sqrt(val[0]*val[0]+val[1]*val[1]+val[2]*val[2]); };

	/// Return the squared vector's norm.
	inline float norme2() const {	return val[0]*val[0]+val[1]*val[1]+val[2]*val[2]; };

	/// Normalize a 3D vector.
	/// If the vector is null, then it is unchanged.
	/// \return Vector's norm
	inline float normalize();
	//@}

	/** \name Operateurs
	  */
	//@{
	/// Acces to one element (no checking)
	inline const float& operator[](const int i) const;
	inline float& operator[](const int i);
	/// Product with a float
	inline Vector3D operator*(float mult) const;
	/// Reflective product with a float
	inline Vector3D operator*=(float mult);
	/// Division with a float
	inline Vector3D operator/(float mult) const;
	/// Reflective division with a float
	inline Vector3D operator/=(float mult);
	/// Copy
	inline Vector3D operator=(const Vector3D& vec2);
	/// Egality (to avoid)
	inline bool operator==(const Vector3D& vec2);
	/// Addition
	inline Vector3D operator+(const Vector3D& vec2) const;
	/// Reflective addition
	inline Vector3D operator+=(const Vector3D& vec);
	/// Substraction
	inline Vector3D operator-(const Vector3D& vec2) const;
	/// Reflective substraction
	inline Vector3D operator-=(const Vector3D& vec);
	/// Inversion
	inline Vector3D operator-() const;
	/// Scalar product
	inline float operator*(const Vector3D& vec) const;
	/// Vectorial product
	inline Vector3D operator^(const Vector3D& vec) const;
	/// Cast operator (float*)
	inline operator float*() {return val;};
	//@}
};

/*****************************************************************
 *                         UTILITY FUNCTIONS
 *****************************************************************/
inline Vector3D Vector3D::getOrthog() const {
	if (fabs(val[0]) > STP3D_EPSILON || fabs(val[1]) > STP3D_EPSILON) {
		Vector3D src(-val[1],val[0],0.0);
		src.normalize();
		return src;
	}
	else {
		return Vector3D(1,0,0);
	}
};


inline float Vector3D::normalize() {
	float norm = norme();
	if (norm < STP3D_EPSILON) {
		STP3D::setError("Normalization of the null 3D vector impossible.");
		return norm;
	}
	else {
		val[0] /= norm; val[1] /= norm; val[2] /= norm;
	}
	return norm;
};

/*****************************************************************
 *							OPERATORS
 *****************************************************************/

// MEMBER ACCES
inline const float& Vector3D::operator[](const int i) const {
	return val[i];
}

inline float& Vector3D::operator[](const int i) {
	return val[i];
}

// SCALAR PRODUCT
inline float Vector3D::operator*(const Vector3D& vec) const {
	return (float) (val[0]*vec.val[0]+val[1]*vec.val[1]+val[2]*vec.val[2]);
}

// VECTORIAL PRODUCT
inline Vector3D Vector3D::operator^(const Vector3D& vec) const {
	return Vector3D(val[1]*vec.val[2] - vec.val[1]*val[2],vec.val[0]*val[2] - val[0]*vec.val[2],val[0]*vec.val[1] - val[1]*vec.val[0]);
}

// UNITARY PRODUCT WITH A DOUBLE
inline Vector3D Vector3D::operator*(float mult) const {
	return Vector3D(val[0]*mult,val[1]*mult,val[2]*mult);
}

// UNITARY REFLECIVE PRODUCT WITH A DOUBLE
inline Vector3D Vector3D::operator*=(float mult) {
	val[0] *= mult;
	val[1] *= mult;
	val[2] *= mult;
	return *this;
}

// UNITARY DIVISION WITH A DOUBLE
inline Vector3D Vector3D::operator/(float mult) const {
	return Vector3D(val[0]/mult,val[1]/mult,val[2]/mult);
}

// UNITARY REFLECIVE DIVISION WITH A DOUBLE
inline Vector3D Vector3D::operator/=(float mult) {
	val[0] /= mult;
	val[1] /= mult;
	val[2] /= mult;
	return *this;
}

// COPY
inline Vector3D Vector3D::operator=(const Vector3D& vec) {
	val[0] = vec.val[0];
	val[1] = vec.val[1];
	val[2] = vec.val[2];
	return *this;
}

// ADDITION
inline Vector3D Vector3D::operator+(const Vector3D& vec2) const {
	return Vector3D(val[0]+vec2.val[0],val[1]+vec2.val[1],val[2]+vec2.val[2]);
}

// REFLECTIVE ADDITION
inline Vector3D Vector3D::operator+=(const Vector3D& vec) {
	val[0] += vec.val[0];
	val[1] += vec.val[1];
	val[2] += vec.val[2];
	return *this;
}

// SUBSTRACTION
inline Vector3D Vector3D::operator-(const Vector3D& vec2) const {
	return Vector3D(val[0]-vec2.val[0],val[1]-vec2.val[1],val[2]-vec2.val[2]);
}

// REFLECTIVE SUBSTRACTION
inline Vector3D Vector3D::operator-=(const Vector3D& vec) {
	val[0] -= vec.val[0];
	val[1] -= vec.val[1];
	val[2] -= vec.val[2];
	return *this;
}

// EGALITY TEST (to avoid)
inline bool Vector3D::operator==(const Vector3D& vec) {
	return ((fabs(val[0]-vec.val[0])+fabs(val[1]-vec.val[1])+fabs(val[2]-vec.val[2])) < STP3D_EPSILON);
}

// INVERSION
inline Vector3D Vector3D::operator-() const {
	return Vector3D(-val[0],-val[1],-val[2]);
}

/*****************************************************************
 *							I/O
 *****************************************************************/

/// 3D Vector writing on a stream
inline std::ostream& operator<<(std::ostream& os, const Vector3D& pt) {
	return os << pt.val[0] << " " << pt.val[1] << " " << pt.val[2];
}

/// 3D Vector reading on a stream
inline std::istream& operator>>(std::istream& is, Vector3D& pt){
	float x,y,z;

	is >> x >> y >> z;
	pt.set(x,y,z);
	return is;
}

} // fin namespace

#endif
