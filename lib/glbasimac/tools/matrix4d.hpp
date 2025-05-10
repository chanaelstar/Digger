/***************************************************************************
                          matrix4d.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri Venceslas
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

#ifndef _STP3D_MATRIX4D_HPP_
#define _STP3D_MATRIX4D_HPP_

#include "globals.hpp"
#include "vector4d.hpp"
#include "vector3d.hpp"
#include <string>

namespace STP3D {

/** A 4 dimensional matrix.
  * This class allow the creation, storing and manipulation of 4x4 matrix.
  * With many operators and utility functions
  * Data are stored in column major mode
  * @author Venceslas BIRI (biri@univ-mlv.fr)
  */
class Matrix4D {
public: 
	float mat[16];			///< values of the matrix...

	/** \name Constructors & destructors
	  */
	//@{
	/// Default constructor. 
	/// Gives the identity matrix.
	Matrix4D() {
		memset(mat,0,16*sizeof(float));
		mat[0] = 1.0;
		mat[5] = 1.0;
		mat[10] = 1.0;
		mat[15] = 1.0;
	};
	/// Constructor by float array
	/// \param src Float matrix array (column major)
	Matrix4D(const float* src) {
		memcpy(mat,src,16*sizeof(float));
	};
	/// Copy constructor.
	/// \param src Source matrix
	Matrix4D(const Matrix4D& src) {
		memcpy(mat,src.mat,16*sizeof(float));
	};
	/// Copy constructor.
	/// \param mt 4 columns of the matrix
	Matrix4D(const Vector4D mt[4]) {
		memcpy(mat   ,mt[0].val,4*sizeof(float));
		memcpy(mat+4 ,mt[1].val,4*sizeof(float));
		memcpy(mat+8 ,mt[2].val,4*sizeof(float));
		memcpy(mat+12,mt[3].val,4*sizeof(float));
	};
	/// Diagonal constructor. 
	/// Create the diagonal matrix with scaling \a diag
	/// \param diag diagonal component
	Matrix4D(double diag) {
		memset(mat,0,16*sizeof(float));
		mat[0] = mat[5] = mat[10] = mat[15] = diag;
	};
	/// Destructor.
	~Matrix4D() {};
	//@}

	/** \name Static matrix creation utility functions
	  */
	//@{
	/** Translation matrix creation.
	  * Return the translation matrix defined by the vector \a tr 
	  * \param tr Translation vector
	  */
	static Matrix4D translation(const Vector3D& tr);
	/** Translation matrix creation.
	  * Return the translation matrix defined by the vector (x,y,z)
	  * \param x,y,z Translation vector coordinates
	  */
	static Matrix4D translation(float x,float y,float z);
	/** Rotation matrix creation.
	  * Return the rotation matrix defined by axe \a axe (x, y, or z) 
	  * and angle \a angle
	  * \param angle Rotation angle (radians)
	  * \param axe Index of axe : 0 => x axe / 1 => y axe / 2 => z axe 
	  */
	static Matrix4D rotation(float angle,int axe);
	/** Rotation matrix creation.
	  * Return the rotation matrix defined by axe \a axe (x, y, or z) 
	  * and angle \a angle
	  * \param angle Rotation angle (radians)
	  * \param axe axe's vector
	  * \todo Verify this constructor
	  */
	static Matrix4D rotation(float angle,const Vector3D& axe);
	/** Homothety matrix creation.
	  * Return the homothety matrix defined by scaling factor \a scal 
	  * \param scal Scaling factor
	  * \todo Verify this constructor
	  */
	static Matrix4D homothety(float scal);
	/** Homothety matrix creation.
	  * Return the homothety matrix defined by scaling factor \a scalx \a scaly \a scalz on each axe 
	  * \param scalx Scaling factor on x axis
	  * \param scaly Scaling factor on y axis
	  * \param scalz Scaling factor on z axis
	  * \todo Verify this constructor
	  */
	static Matrix4D homothety(float scalx,float scaly,float scalz);
	/** Perspective projection matrix creation.
	  * Return the perspective projection matrix for all parameters indicated. 
	  * \param l,r left and right border of the virtual screen
	  * \param b,t bottom and top border of the virtual screen
	  * \param n,f near and far distance of the view
	  * \todo Verify this constructor
	  */
	static Matrix4D perspective(float l,float r,float b,float t,float n,float f);
	/** Perspective projection matrix creation.
	  * Return the perspective projection matrix for all parameters indicated. 
	  * \param fov field of view angle 'in degree' in y axis
	  * \param ratio ratio between width and height (w/h)
	  * \param znear,zfar near and far distance of the view
	  * \todo Verify this constructor
	  */
	static Matrix4D perspective(float fovy,float ratio,float znear,float zfar);
	/** Look At matrix creation.
	  * Return the matrix switching to the frame in pos viewing at center. 
	  * \param pos position of the new frame
	  * \param center position of the point seen in the (negative) z axe of the frame
	  * \param up y axe of the new frame (up vector)
	  * \todo Verify this constructor
	  */
	static Matrix4D lookAt(const Vector3D& pos,const Vector3D& center,const Vector3D& up);
	/** Ortho2D matrix creation.
	  * Return the 2D orthographic projection thanks to the parameters. 
	  * \param left,right left and right border of the virtual screen
	  * \param bottom,top bottom and top border of the virtual screen
	  * \todo Verify this constructor
	  */
	static Matrix4D ortho2D(float left,float right,float bottom, float top);
	//@}


	/** \name Operators
	  */
	//@{
	/// Column selection
	Vector4D operator[](unsigned int col);
	/// Addition of two matrix
	Matrix4D operator+(const Matrix4D& mat) const;
	/// Reflective Addition of two matrix
	Matrix4D operator+=(const Matrix4D& mat);
	/// Inversion
	Matrix4D operator-() const;
	/// Substraction of two matrix
	Matrix4D operator-(const Matrix4D& mat) const;
	/// Reflective Substraction of two matrix
	Matrix4D operator-=(const Matrix4D& mat);
	/// Multiplication of two matrix
	Matrix4D operator*(const Matrix4D& mat) const;
	/// Reflective multiplication of two matrix
	Matrix4D operator*=(const Matrix4D& mat);
	/// Multiplication Matrix Vector. 
	/// \return Resulting 4D vector.
	Vector4D operator*(const Vector4D& vec) const;
	/// Copy operator.
	Matrix4D operator=(const Matrix4D& src);
	/// Cast operator (float*)
	inline operator float*() {return mat;};
	//@}


	/** \name Utility function
	  */
	//@{
	/** Set the matrix by a OpenGL column order style array. 
	  * Take the 16 float elements of the array mat, stored in a OpenGL fashion (column major).
	  * \param mat Source array (of float)
	  */
	void set(const float mat[]);
	/** Return the matrix elements in an array.
	  * Return the matrix elements in an array in a OpenGL style ordering (column major).
	  * This array can be send directly with OpenGL 
	  * \param matren Resulting array recieving the matrix coefficients.
	  */
	void get(float matren[]) const;
	/** Multiply by a Vector3D considered as a point.
	  * Return the 4D vector, resulting of the mulitplication of the matrix with the 4D vector (ope,1.0)
	  * \param ope 3D vector operand considered as a point
	  */
	Vector4D xPoint(Vector3D ope) const { 
		return (*this)*Vector4D(ope,1.0);
	};
	/** Multiply by a Vector3D considered as a direction.
	  * Return the 4D vector, resulting of the mulitplication of the matrix with the 4D vector (ope,0.0)
	  * \param ope 3D vector operand considered as a direction
	  */
	Vector4D xDir(Vector3D ope) const{ 
		return (*this)*Vector4D(ope,0.0);
	};


	/** Invert the matrix. Inversion in place.
	  * \return Indicate whether the inversion was possible or not
	  */
	bool invert();
	/** Transpose the matrix.
	  * Transpose in place.
	  */
	void transpose();
	/** Get the normal matrix from a 4x4 modelview matrix
	  * Performs the following transformation : erase the translation part, invert and transpose.
	  */
	void normalFromModelview();
	/** Set a value.
	  * Set a value at column \a col and at line \a lgn (starting from 0) with value \a val
	  * \param col,lgn Case index (column and line)
	  * \param val Source value
	  */
	void set(unsigned int col,unsigned int lgn,float val);
	//@}
	
};

/// Affichage sur un flux d'une matrice 4x4
extern std::ostream& operator<<(std::ostream& os,const STP3D::Matrix4D& mat);

/* *************************************************************************************
 * ********** OPERATEURS
 * ************************************************************************************* */
inline Vector4D Matrix4D::operator[](unsigned int col) {
	return Vector4D(mat[4*col],mat[4*col+1],mat[4*col+2],mat[4*col+3]);
}

inline Matrix4D Matrix4D::operator-() const {
	Matrix4D m;

	for(unsigned int i=0;i<16;i++)
		m.mat[i] = -mat[i];

	return m;
}

inline Matrix4D Matrix4D::operator+(const Matrix4D& ml) const {
	Matrix4D m;

	for(unsigned int i=0;i<16;i++)
		m.mat[i] = mat[i]+ml.mat[i];

	return m;
}

inline Matrix4D Matrix4D::operator+=(const Matrix4D& ml) {
	for(unsigned int i=0;i<16;i++)
		mat[i] += ml.mat[i];

	return *this;
}

inline Matrix4D Matrix4D::operator-(const Matrix4D& ml) const {
	Matrix4D m;

	for(unsigned int i=0;i<16;i++)
		m.mat[i] = mat[i]-ml.mat[i];

	return m;
}

inline Matrix4D Matrix4D::operator-=(const Matrix4D& ml) {
	for(unsigned int i=0;i<16;i++)
		mat[i] -= ml.mat[i];

	return *this;
}

inline Matrix4D Matrix4D::operator*(const Matrix4D& ml) const {
	Matrix4D m;
	
	// First column
	m.mat[0]  = ml.mat[0]*mat[0] + ml.mat[1]*mat[4] + ml.mat[2]*mat[8]  + ml.mat[3]*mat[12];
	m.mat[1]  = ml.mat[0]*mat[1] + ml.mat[1]*mat[5] + ml.mat[2]*mat[9]  + ml.mat[3]*mat[13];
	m.mat[2]  = ml.mat[0]*mat[2] + ml.mat[1]*mat[6] + ml.mat[2]*mat[10] + ml.mat[3]*mat[14];
	m.mat[3]  = ml.mat[0]*mat[3] + ml.mat[1]*mat[7] + ml.mat[2]*mat[11] + ml.mat[3]*mat[15];
	// Second column
	m.mat[4]  = ml.mat[4]*mat[0] + ml.mat[5]*mat[4] + ml.mat[6]*mat[8]  + ml.mat[7]*mat[12];
	m.mat[5]  = ml.mat[4]*mat[1] + ml.mat[5]*mat[5] + ml.mat[6]*mat[9]  + ml.mat[7]*mat[13];
	m.mat[6]  = ml.mat[4]*mat[2] + ml.mat[5]*mat[6] + ml.mat[6]*mat[10] + ml.mat[7]*mat[14];
	m.mat[7]  = ml.mat[4]*mat[3] + ml.mat[5]*mat[7] + ml.mat[6]*mat[11] + ml.mat[7]*mat[15];
	// Third column
	m.mat[8]  = ml.mat[8]*mat[0] + ml.mat[9]*mat[4] + ml.mat[10]*mat[8]  + ml.mat[11]*mat[12];
	m.mat[9]  = ml.mat[8]*mat[1] + ml.mat[9]*mat[5] + ml.mat[10]*mat[9]  + ml.mat[11]*mat[13];
	m.mat[10] = ml.mat[8]*mat[2] + ml.mat[9]*mat[6] + ml.mat[10]*mat[10] + ml.mat[11]*mat[14];
	m.mat[11] = ml.mat[8]*mat[3] + ml.mat[9]*mat[7] + ml.mat[10]*mat[11] + ml.mat[11]*mat[15];
	// Fourth column
	m.mat[12] = ml.mat[12]*mat[0] + ml.mat[13]*mat[4] + ml.mat[14]*mat[8]  + ml.mat[15]*mat[12];
	m.mat[13] = ml.mat[12]*mat[1] + ml.mat[13]*mat[5] + ml.mat[14]*mat[9]  + ml.mat[15]*mat[13];
	m.mat[14] = ml.mat[12]*mat[2] + ml.mat[13]*mat[6] + ml.mat[14]*mat[10] + ml.mat[15]*mat[14];
	m.mat[15] = ml.mat[12]*mat[3] + ml.mat[13]*mat[7] + ml.mat[14]*mat[11] + ml.mat[15]*mat[15];
	return m;
}

inline Matrix4D Matrix4D::operator*=(const Matrix4D& ml) {
	float mt[16];
	// First column
	mt[0]  = ml.mat[0]*mat[0] + ml.mat[1]*mat[4] + ml.mat[2]*mat[8]  + ml.mat[3]*mat[12];
	mt[1]  = ml.mat[0]*mat[1] + ml.mat[1]*mat[5] + ml.mat[2]*mat[9]  + ml.mat[3]*mat[13];
	mt[2]  = ml.mat[0]*mat[2] + ml.mat[1]*mat[6] + ml.mat[2]*mat[10] + ml.mat[3]*mat[14];
	mt[3]  = ml.mat[0]*mat[3] + ml.mat[1]*mat[7] + ml.mat[2]*mat[11] + ml.mat[3]*mat[15];
	// Second column
	mt[4]  = ml.mat[4]*mat[0] + ml.mat[5]*mat[4] + ml.mat[6]*mat[8]  + ml.mat[7]*mat[12];
	mt[5]  = ml.mat[4]*mat[1] + ml.mat[5]*mat[5] + ml.mat[6]*mat[9]  + ml.mat[7]*mat[13];
	mt[6]  = ml.mat[4]*mat[2] + ml.mat[5]*mat[6] + ml.mat[6]*mat[10] + ml.mat[7]*mat[14];
	mt[7]  = ml.mat[4]*mat[3] + ml.mat[5]*mat[7] + ml.mat[6]*mat[11] + ml.mat[7]*mat[15];
	// Third column
	mt[8]  = ml.mat[8]*mat[0] + ml.mat[9]*mat[4] + ml.mat[10]*mat[8]  + ml.mat[11]*mat[12];
	mt[9]  = ml.mat[8]*mat[1] + ml.mat[9]*mat[5] + ml.mat[10]*mat[9]  + ml.mat[11]*mat[13];
	mt[10] = ml.mat[8]*mat[2] + ml.mat[9]*mat[6] + ml.mat[10]*mat[10] + ml.mat[11]*mat[14];
	mt[11] = ml.mat[8]*mat[3] + ml.mat[9]*mat[7] + ml.mat[10]*mat[11] + ml.mat[11]*mat[15];
	// Fourth column
	mt[12] = ml.mat[12]*mat[0] + ml.mat[13]*mat[4] + ml.mat[14]*mat[8]  + ml.mat[15]*mat[12];
	mt[13] = ml.mat[12]*mat[1] + ml.mat[13]*mat[5] + ml.mat[14]*mat[9]  + ml.mat[15]*mat[13];
	mt[14] = ml.mat[12]*mat[2] + ml.mat[13]*mat[6] + ml.mat[14]*mat[10] + ml.mat[15]*mat[14];
	mt[15] = ml.mat[12]*mat[3] + ml.mat[13]*mat[7] + ml.mat[14]*mat[11] + ml.mat[15]*mat[15];

	for(unsigned int i=0;i<16;i++) mat[i] = mt[i];

	return *this;
}

inline Vector4D Matrix4D::operator*(const Vector4D& ml) const {
	return Vector4D(ml[0]*mat[0] + ml[1]*mat[4] + ml[2]*mat[8]  + ml[3]*mat[12],
	                ml[0]*mat[1] + ml[1]*mat[5] + ml[2]*mat[9]  + ml[3]*mat[13],
	                ml[0]*mat[2] + ml[1]*mat[6] + ml[2]*mat[10] + ml[3]*mat[14],
	                ml[0]*mat[3] + ml[1]*mat[7] + ml[2]*mat[11] + ml[3]*mat[15]);
}

inline Matrix4D Matrix4D::operator=(const Matrix4D& src) {
	for(unsigned int i=0;i<16;i++) mat[i] = src.mat[i];
	return *this;
}


/* *************************************************************************************
 * ********** CONSTRUCTORS
 * ************************************************************************************* */

inline Matrix4D Matrix4D::translation(const Vector3D& trans) {
	Matrix4D m;
	m.mat[12] = trans[0];
	m.mat[13] = trans[1];
	m.mat[14] = trans[2];
	return m;
}

inline Matrix4D Matrix4D::translation(float tx,float ty,float tz) {
	Matrix4D m;
	m.mat[12] = tx;
	m.mat[13] = ty;
	m.mat[14] = tz;
	return m;
}

inline Matrix4D Matrix4D::rotation(float angle,int axe) {
	Matrix4D m;
	double c = cos(angle);
	double s = sin(angle);
	switch (axe) {
		// Axe x
		case (0) :
			m.mat[5] = c;
			m.mat[6] = s;
			m.mat[9] = -s;
			m.mat[10]= c;
			return m;
		// Axe y
		case (1) :
			m.mat[0] = c;
			m.mat[2] = -s;
			m.mat[8] = s;
			m.mat[10]= c;
			return m;
		// Axe z
		case (2) :
			m.mat[0] = c;
			m.mat[1] = s;
			m.mat[4] = -s;
			m.mat[5] = c;
			return m;
		default :
			STP3D::setError(std::string("Internal error. Call matrice4D constructor with an illegal axe flag (")+
			                intToString(axe)+std::string(") ! Choose 0(x), 1(y) or 2(z) !"));
			return Matrix4D();
	}
}

inline Matrix4D Matrix4D::rotation(float angle,const Vector3D& axe) {
	Matrix4D m;
	Vector3D n_axe(axe);
	n_axe.normalize();
	double c = cos(angle/2.0);
	double s = sin(angle/2.0);
	double qx,qy,qz,qw,xx,yy,zz,xy,xz,yz,xw,yw,zw;

	qx = n_axe[0] * s;
	qy = n_axe[1] * s;
	qz = n_axe[2] * s;
	qw = c;

	xx = qx*qx;
	yy = qy*qy;
	zz = qz*qz;
	xy = qx*qy;
	xz = qx*qz;
	yz = qy*qz;
	xw = qx*qw;
	yw = qy*qw;
	zw = qz*qw;

	m.mat[0] = 1.0f - 2.0*( yy + zz );
	m.mat[1] = 2.0*( xy + zw );
	m.mat[2] = 2.0*( xz - yw );

	m.mat[4] = 2.0*( xy - zw );
	m.mat[5] = 1.0f - 2.0*( xx + zz );
	m.mat[6] = 2.0*( yz + xw );

	m.mat[8] = 2.0*( xz + yw );
	m.mat[9] = 2.0*( yz - xw );
	m.mat[10]= 1.0f - 2.0*( xx + yy );

	return m;
}
 
inline Matrix4D Matrix4D::homothety(float scal) {
	Matrix4D m;
	m.mat[0] = m.mat[5] = m.mat[10] = scal;
	return m;
}

inline Matrix4D Matrix4D::homothety(float scalx,float scaly, float scalz) {
	Matrix4D m;
	m.mat[0] = scalx;
	m.mat[5] = scaly;
	m.mat[10] = scalz;
	return m;
}


inline Matrix4D Matrix4D::perspective(float l,float r,float b,float t,float n,float f) {
	Matrix4D m;
	m.mat[0] = 2.0f*n/(r-l);
	m.mat[5] = 2.0f*n/(t-b);
	m.mat[8] = (r+l)/(r-l);
	m.mat[9] = (t+b)/(t-b);
	m.mat[10]= -(f+n)/(f-n);
	m.mat[11]= -1.0f;
	m.mat[14]= -2.0f*f*n/(f-n);
	m.mat[15]= 0.0;

	return m;
}

inline Matrix4D Matrix4D::perspective(float fovy,float ratio,float znear,float zfar) {
	Matrix4D m;
	double f = 1.0/tanf(double(fovy)*M_PI/(360.0f)); // 1/tan(fovy/2)
	m.mat[0] = f/ratio;
	m.mat[5] = f;
	m.mat[10]= -(zfar+znear)/(zfar-znear);
	m.mat[11]= -1.0f;
	m.mat[14]= -2.0f*zfar*znear/(zfar-znear);
	m.mat[15]= 0.0;

	return m;
}

inline Matrix4D Matrix4D::lookAt(const Vector3D& eye,const Vector3D& center,const Vector3D& up) {
	Vector3D z = (eye - center);
	z.normalize();
	Vector3D x = up^z;
	x.normalize();
	Vector3D y = z^x;
	y.normalize();

	Vector4D tab[4];
	tab[0] = Vector4D(x[0],y[0],z[0],0.0);
	tab[1] = Vector4D(x[1],y[1],z[1],0.0);
	tab[2] = Vector4D(x[2],y[2],z[2],0.0);
	tab[3] = Vector4D(0.0,0.0,0.0,1.0);
	Matrix4D rot(tab);
	tab[0] = Vector4D(1.0,0.0,0.0,0.0);
	tab[1] = Vector4D(0.0,1.0,0.0,0.0);
	tab[2] = Vector4D(0.0,0.0,1.0,0.0);
	tab[3] = Vector4D(-eye,1.0);
	Matrix4D t(tab);

	return rot*t;
}

inline Matrix4D Matrix4D::ortho2D(float left,float right,float bottom, float top) {
    const float zNear = -1.0f;
    const float zFar = 1.0f;
    const float inv_z = 1.0f / (zFar - zNear);
    const float inv_y = 1.0f / (top - bottom);
    const float inv_x = 1.0f / (right - left);

	// Creation null matrix
	Matrix4D res;
	// Fill matrix
    //first column
    res.mat[0] = (2.0f*inv_x);
    //second
    res.mat[5] = (2.0*inv_y);
    //third
    res.mat[10]= (-2.0f*inv_z);
    //fourth
    res.mat[12] = (-(right + left)*inv_x);
    res.mat[13] = (-(top + bottom)*inv_y);
    res.mat[14] = (-(zFar + zNear)*inv_z);
    res.mat[15] = (1.0f);

	return res;
}



/* *************************************************************************************
 * ********** UTILITY FUNCTIONS
 * ************************************************************************************* */

inline void Matrix4D::set(const float matc[]) {
	for(int i=0;i<16;i++) mat[i] = matc[i];
}

inline void Matrix4D::get(float matren[]) const {
	for(int i=0;i<16;i++) matren[i] = mat[i];
}

inline void Matrix4D::transpose() {
	Vector4D tmp[4];
	tmp[0] = Vector4D(mat[0],mat[4],mat[8] ,mat[12]);
	tmp[1] = Vector4D(mat[1],mat[5],mat[9] ,mat[13]);
	tmp[2] = Vector4D(mat[2],mat[6],mat[10],mat[14]);
	tmp[3] = Vector4D(mat[3],mat[7],mat[11],mat[15]);
	for(int i=0;i<16;i++) {mat[i] = (tmp[i/4])[i%4];}
}

inline void Matrix4D::normalFromModelview() {
	// Destroy the third line
	mat[12] = mat[13] = mat[14] = 0.0; mat[15] = 1.0;
	// Invert and transpose
	invert();
	transpose();
}


inline bool Matrix4D::invert() {
	float m[16];
	float invOut[16];
	this->get(m);
	double inv[16], det;
	int i;

	inv[0] = m[5]  * m[10] * m[15] - 
	         m[5]  * m[11] * m[14] - 
	         m[9]  * m[6]  * m[15] + 
	         m[9]  * m[7]  * m[14] +
	         m[13] * m[6]  * m[11] - 
	         m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
	          m[4]  * m[11] * m[14] + 
	          m[8]  * m[6]  * m[15] - 
	          m[8]  * m[7]  * m[14] - 
	          m[12] * m[6]  * m[11] + 
	          m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
	         m[4]  * m[11] * m[13] - 
	         m[8]  * m[5] * m[15] + 
	         m[8]  * m[7] * m[13] + 
	         m[12] * m[5] * m[11] - 
	         m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
	           m[4]  * m[10] * m[13] +
	           m[8]  * m[5] * m[14] - 
	           m[8]  * m[6] * m[13] - 
	           m[12] * m[5] * m[10] + 
	           m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
	          m[1]  * m[11] * m[14] + 
	          m[9]  * m[2] * m[15] - 
	          m[9]  * m[3] * m[14] - 
	          m[13] * m[2] * m[11] + 
	          m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
	         m[0]  * m[11] * m[14] - 
	         m[8]  * m[2] * m[15] + 
	         m[8]  * m[3] * m[14] + 
	         m[12] * m[2] * m[11] - 
	         m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
	          m[0]  * m[11] * m[13] + 
	          m[8]  * m[1] * m[15] - 
	          m[8]  * m[3] * m[13] - 
	          m[12] * m[1] * m[11] + 
	          m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
	          m[0]  * m[10] * m[13] - 
	          m[8]  * m[1] * m[14] + 
	          m[8]  * m[2] * m[13] + 
	          m[12] * m[1] * m[10] - 
	          m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
	         m[1]  * m[7] * m[14] - 
	         m[5]  * m[2] * m[15] + 
	         m[5]  * m[3] * m[14] + 
	         m[13] * m[2] * m[7] - 
	         m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
	          m[0]  * m[7] * m[14] + 
	          m[4]  * m[2] * m[15] - 
	          m[4]  * m[3] * m[14] - 
	          m[12] * m[2] * m[7] + 
	          m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
	          m[0]  * m[7] * m[13] - 
	          m[4]  * m[1] * m[15] + 
	          m[4]  * m[3] * m[13] + 
	          m[12] * m[1] * m[7] - 
	          m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
	           m[0]  * m[6] * m[13] + 
	           m[4]  * m[1] * m[14] - 
	           m[4]  * m[2] * m[13] - 
	           m[12] * m[1] * m[6] + 
	           m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
	          m[1] * m[7] * m[10] + 
	          m[5] * m[2] * m[11] - 
	          m[5] * m[3] * m[10] - 
	          m[9] * m[2] * m[7] + 
	          m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
	         m[0] * m[7] * m[10] - 
	         m[4] * m[2] * m[11] + 
	         m[4] * m[3] * m[10] + 
	         m[8] * m[2] * m[7] - 
	         m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
	           m[0] * m[7] * m[9] + 
	           m[4] * m[1] * m[11] - 
	           m[4] * m[3] * m[9] - 
	           m[8] * m[1] * m[7] + 
	           m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
	          m[0] * m[6] * m[9] - 
	          m[4] * m[1] * m[10] + 
	          m[4] * m[2] * m[9] + 
	          m[8] * m[1] * m[6] - 
	          m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (fabs(det) < STP3D_EPSILON)
	    return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
	    invOut[i] = inv[i] * det;
	this->set(invOut);
	return true;
}

inline void Matrix4D::set(unsigned int col,unsigned int lgn,float val) {
	if (lgn>3 || col>3) {
		STP3D::setError(std::string("Try to set a 4x4 matrix value out of bounds (lgn/col)")+STP3D::intToString(col)+"/"+STP3D::intToString(lgn));
	}
	mat[4*col+lgn] = val;
}

/// Fonction permettant l'application d'une matrice \a mt 4x4 stockée en column major (OpenGL)
/// sur un vecteur \a levec. Le résultat est stocké dans \a res
inline void appmat(const float mt[],float levec[],float res[]) {
	res[0] = mt[0]*levec[0]+mt[4]*levec[1]+mt[8]*levec[2]+mt[12]*levec[3];
	res[1] = mt[1]*levec[0]+mt[5]*levec[1]+mt[9]*levec[2]+mt[13]*levec[3];
	res[2] = mt[2]*levec[0]+mt[6]*levec[1]+mt[10]*levec[2]+mt[14]*levec[3];
	res[3] = mt[3]*levec[0]+mt[7]*levec[1]+mt[11]*levec[2]+mt[15]*levec[3];
}

/// Fonction permettant l'application d'une matrice \a mt 4x4 stockée en column major (OpenGL)
/// sur un vecteur \a levec. Le résultat est stocké dans \a res
inline void appmat(const float mt[],double *levec,double *res) {
	res[0] = mt[0]*levec[0]+mt[4]*levec[1]+mt[8]*levec[2]+mt[12]*levec[3];
	res[1] = mt[1]*levec[0]+mt[5]*levec[1]+mt[9]*levec[2]+mt[13]*levec[3];
	res[2] = mt[2]*levec[0]+mt[6]*levec[1]+mt[10]*levec[2]+mt[14]*levec[3];
	res[3] = mt[3]*levec[0]+mt[7]*levec[1]+mt[11]*levec[2]+mt[15]*levec[3];
}

/// Fonction permettant l'application d'une matrice \a mt 4x4 stockée en column major (OpenGL)
/// sur un vecteur \a levec. Le résultat est stocké dans \a res
inline void appmat(const double *mt,double *levec,double *res) {
	res[0] = mt[0]*levec[0]+mt[4]*levec[1]+mt[8]*levec[2]+mt[12]*levec[3];
	res[1] = mt[1]*levec[0]+mt[5]*levec[1]+mt[9]*levec[2]+mt[13]*levec[3];
	res[2] = mt[2]*levec[0]+mt[6]*levec[1]+mt[10]*levec[2]+mt[14]*levec[3];
	res[3] = mt[3]*levec[0]+mt[7]*levec[1]+mt[11]*levec[2]+mt[15]*levec[3];
}

/// Affichage d'une matrice \a mt 4x4 stockée en column major (OpenGL)
inline void afficheMatrice(float mt[]) {
	std::cerr<<mt[0]<<"\t"<<mt[4]<<"\t"<<mt[8]<<"\t"<<mt[12]<<std::endl;
	std::cerr<<mt[1]<<"\t"<<mt[5]<<"\t"<<mt[9]<<"\t"<<mt[13]<<std::endl;
	std::cerr<<mt[2]<<"\t"<<mt[6]<<"\t"<<mt[10]<<"\t"<<mt[14]<<std::endl;
	std::cerr<<mt[3]<<"\t"<<mt[7]<<"\t"<<mt[11]<<"\t"<<mt[15]<<std::endl;
}

/// Affichage d'une matrice \a mt 4x4 stockée en column major (OpenGL)
inline void afficheMatrice(double mt[]) {
	std::cerr<<mt[0]<<"\t"<<mt[4]<<"\t"<<mt[8]<<"\t"<<mt[12]<<std::endl;
	std::cerr<<mt[1]<<"\t"<<mt[5]<<"\t"<<mt[9]<<"\t"<<mt[13]<<std::endl;
	std::cerr<<mt[2]<<"\t"<<mt[6]<<"\t"<<mt[10]<<"\t"<<mt[14]<<std::endl;
	std::cerr<<mt[3]<<"\t"<<mt[7]<<"\t"<<mt[11]<<"\t"<<mt[15]<<std::endl;
}

inline std::ostream& operator<<(std::ostream& os,const Matrix4D& src) {
	for(int j=0;j<4;j++) {
		for(int i=0;i<4;i++) {
			os<<src.mat[4*i+j]<<"\t";
		}
		os << std::endl;
	}
	return os;
}

} // Namespace end

#endif
