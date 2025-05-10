/***************************************************************************
                      vector4d.hpp  -  description
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

#ifndef _STP3D_VECTOR4D_HPP_
#define _STP3D_VECTOR4D_HPP_

#include <iostream>
#include <ostream>
#include <cstdlib>
#include <cmath>
#include "globals.hpp"
#include "vector3d.hpp"

namespace STP3D {

/** Classe representing a 4D vector
  * Very usefull (and very used) class representing a 3D vector in homogeneous coordinates (so in 4D).
  * @author Venceslas BIRI (biri@univ-mlv.fr)
  */
class Vector4D {
public:
	/** 
	  * \name Constructors et destructors
	  */
	//@{
	/// Default constructor.
	/// Create the null vector (0,0,0,0)
	Vector4D() {val[0] = val[1] = val[2] = val[3] = 0.0;};
	/// Set constructor.
	/// All coordinate will have the parameter value
	/// \param in parameter used to fill all coordinates
	Vector4D(float in) {val[0] = val[1] = val[2] = val[3] = in;};
	/// Set constructor from 4 coordinates.
	Vector4D(float in_x,float in_y,float in_z,float in_w) {val[0] = in_x;val[1] = in_y; val[2] = in_z; val[3] = in_w;};
	/// Copy constructor.
	Vector4D(const Vector4D& src) {val[0] = src.val[0];val[1] = src.val[1]; val[2] = src.val[2]; val[3] = src.val[3];};
	/// Constructor with a Vector3D and a float.
	Vector4D(const Vector3D& src,float w) {val[0] = src.val[0]; val[1] = src.val[1]; val[2] = src.val[2]; val[3] = w;};
	/// Destructor.
	~Vector4D() {};
	//@}
	
	/** \name Coordinates (public access for simplicity)
	  */
	//@{
	union {
		float val[4];
		struct {float x,y,z,w;};
	};
	//@}

	/** \name Fonctions d'affectation
	  * Ces fonctions permettent d'éviter la recréation de vecteur
	  */
	//@{
	/// Standard set.
	inline void set(float in_x,float in_y,float in_z,float in_w) {val[0] = in_x; val[1] = in_y; val[2] = in_z; val[3] = in_w;};
	/// Set from one vector
	/// \param src Source vector
	inline void set(const Vector4D& src) {val[0] = src.val[0]; val[1] = src.val[1]; val[2] = src.val[2]; val[3] = src.val[3];};
	//@}
	
	/** \name Utility functions
	  */
	//@{
	/// Return the L2 vector's norme
	inline float norme() {return std::sqrt(val[0]*val[0]+val[1]*val[1]+val[2]*val[2]+val[3]*val[3]);};
	/// Return the L2 vector's norme
	inline float norme2() {return (val[0]*val[0]+val[1]*val[1]+val[2]*val[2]+val[3]*val[3]);};
	/// Normalize a 4D vector
	float normalize();
	/// Create a 4D vector from a 3D point
	void fromPoint(float x,float y,float z) {set(x,y,z,1.0);};
	void fromPoint(float coord[3]) {set(coord[0],coord[1],coord[2],1.0);};
	void fromPoint(const Vector3D& src) {set(src.val[0],src.val[1],src.val[2],1.0);};
	/// Create a 4D vector from a 3D direction
	void fromDirection(float x,float y,float z) {set(x,y,z,0.0);};
	void fromDirection(float coord[3]) {set(coord[0],coord[1],coord[2],0.0);};
	void fromDirection(const Vector3D& src) {set(src.val[0],src.val[1],src.val[2],0.0);};
	/// Create a 3D vector from a homogeneate 4D vector.
	/// If the fourth component of src is null, then it is a vector and we just copy the first three coordinates.
	/// Else it is a point and we perform a homogeneous division.
	inline Vector3D homogeneate();
	//@}
	
	/** \name Operators
	  */
	//@{
	/// Selection
	inline const float& operator[](const int i) const;
	inline float& operator[](const int i);
	/// Produit avec un float
	inline Vector4D operator*(float mult) const;
	/// Produit reflectif avec un float
	inline Vector4D operator*=(float mult);
	/// Division avec un float
	inline Vector4D operator/(float mult) const;
	/// Division reflective avec un float
	inline Vector4D operator/=(float mult);
	/// Set
	inline Vector4D operator=(const Vector4D& vec);
	/// Egality function (to avoid)
	inline bool operator==(const Vector4D& vec) const;
	/// Inversion
	inline Vector4D operator-() const;
	/// Addition
	inline Vector4D operator+(const Vector4D& vec2) const;
	/// Reflective addition
	inline Vector4D operator+=(const Vector4D& vec);
	/// Substraction
	inline Vector4D operator-(const Vector4D& vec2) const;
	/// Reflective substraction 
	inline Vector4D operator-=(const Vector4D& vec);
	/// Cast operator (float*)
	inline operator float*() {return val;};
	/// Scalar product
	inline float operator*(const Vector4D& vec) const;
	//@}
};

/*****************************************************************
 *							UTILITY FUNCTIONS
 *****************************************************************/
inline float Vector4D::normalize() {
	float norm = this->norme();
	if (norm < STP3D_EPSILON) {
		STP3D::setError("Normalization of the null 4D vector impossible.");
		return norm;
	}
	else {
		val[0] /= norm;
		val[1] /= norm;
		val[2] /= norm;
		val[3] /= norm;
	}
	return norm;
}

inline Vector3D Vector4D::homogeneate() {
	if (fabs(val[3]) < STP3D_EPSILON) {
		// This case handles direction. This not necesseraly an error.
		return Vector3D(val[0],val[1],val[2]);
	}
	else {
		return Vector3D(val[0]/val[3],val[1]/val[3],val[2]/val[3]);
	}
}

/*****************************************************************
 *							OPERATORS
 *****************************************************************/
// MEMBER ACCES
inline const float& Vector4D::operator[](const int i) const {
	return val[i];
}

inline float& Vector4D::operator[](const int i) {
	return val[i];
}

inline bool Vector4D::operator==(const Vector4D& vec) const {
	return ((fabs(val[0]-vec.val[0])+fabs(val[1]-vec.val[1])+fabs(val[2]-vec.val[2])+fabs(val[3]-vec.val[3])) < STP3D_EPSILON);
}

inline Vector4D Vector4D::operator=(const Vector4D& vec) {
	val[0] = vec.val[0];
	val[1] = vec.val[1];
	val[2] = vec.val[2];
	val[3] = vec.val[3];
	return *this;
}

// INVERSION
inline Vector4D Vector4D::operator-() const {
	return Vector4D(-val[0],-val[1],-val[2],-val[3]);
}

// UNITARY PRODUCT WITH A DOUBLE
inline Vector4D Vector4D::operator*(float mult) const {
	return Vector4D(val[0]*mult,val[1]*mult,val[2]*mult,val[3]*mult);
}

// UNITARY REFLECTIVE PRODUCT WITH A DOUBLE
inline Vector4D Vector4D::operator*=(float mult) {
	val[0] *= mult;
	val[1] *= mult;
	val[2] *= mult;
	val[3] *= mult;
	return *this;
}

// UNITARY DIVISION WITH A DOUBLE
inline Vector4D Vector4D::operator/(float mult) const {
	return Vector4D(val[0]/mult,val[1]/mult,val[2]/mult,val[3]/mult);
}

// UNITARY REFLECIVE DIVISION WITH A DOUBLE
inline Vector4D Vector4D::operator/=(float mult) {
	val[0] /= mult;
	val[1] /= mult;
	val[2] /= mult;
	val[3] /= mult;
	return *this;
}

// ADDITION
inline Vector4D Vector4D::operator+(const Vector4D& vec2) const {
	return Vector4D(val[0]+vec2.val[0],val[1]+vec2.val[1],val[2]+vec2.val[2],val[3]+vec2.val[3]);
}

// REFLECTIVE ADDITION 
inline Vector4D Vector4D::operator+=(const Vector4D& vec) {
	val[0] += vec.val[0];
	val[1] += vec.val[1];
	val[2] += vec.val[2];
	val[3] += vec.val[3];
	return *this;
}

// SUBSTRACTION
inline Vector4D Vector4D::operator-(const Vector4D& vec2) const {
	return Vector4D(val[0]-vec2.val[0],val[1]-vec2.val[1],val[2]-vec2.val[2],val[3]-vec2.val[3]);
}

// REFLECTIVE SUBSTRACTION
inline Vector4D Vector4D::operator-=(const Vector4D& vec) {
	val[0] -= vec.val[0];
	val[1] -= vec.val[1];
	val[2] -= vec.val[2];
	val[3] -= vec.val[3];
	return *this;
}

// SCALAR PRODUCT
inline float Vector4D::operator*(const Vector4D& vec) const {
	return (val[0]*vec.val[0]+val[1]*vec.val[1]+val[2]*vec.val[2]+val[3]*vec.val[3]);
}

/// Vector4D stream writing
inline std::ostream& operator<<(std::ostream& os, const Vector4D& vec) {
	return os << vec.val[0] << " " << vec.val[1] << " " << vec.val[2] << " " << vec.val[3];
}

/// 3D Vector reading on a stream
inline std::istream& operator>>(std::istream& is, Vector4D& pt){
	float x,y,z,w;

	is >> x >> y >> z >> w;
	pt.val[0] = x;
	pt.val[1] = y;
	pt.val[2] = z;
	pt.val[3] = w;
	return is;
}

} // Namespace ends

#endif
