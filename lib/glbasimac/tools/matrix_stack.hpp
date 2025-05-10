/***************************************************************************
                          matrix.hpp  -  description
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

#ifndef _STP3D_MATRIX_STACK_HPP_
#define _STP3D_MATRIX_STACK_HPP_


#include <iostream>
#include <vector>
#include "globals.hpp"
#include "matrix4d.hpp"


namespace STP3D {

	/**
	  * \brief The Matrix Stack class allows to store matrix in a simple stack.
	  * Matrix Stack allows to store several matrix in a stack order. This is
	  * usefull especially when these matrix store different frame, allowing the
	  * application to recall, thanks to the stack, previous frame.
	  */
	class MatrixStack {
	public:
		/// Standard construtor. Creates a stack containing one identity matrix.
		MatrixStack() {
			stack.clear();
			stack.push_back(Matrix4D());
		};
		~MatrixStack() {};

		/// The stack of matrix
		std::vector<Matrix4D> stack;

		/// Push. Copy the current top matrix. Create a new layer and store the copied matrix
		void pushMatrix() {stack.push_back(stack.back());};
		/// Pop Matrix.
		void popMatrix() {if (stack.size()>0) stack.pop_back();};

		/// Get the number of matrix in the matrix stack
		size_t getNbElt() {return stack.size();};
		/// Retrieve the GL matrix from the top level matrix
		void getTopGLMatrix(float mat[]) const {stack.back().get(mat);};
		float* getTopGLMatrix() {return (float*)stack.back();};
		Matrix4D getTopGLMatrix() const {return stack.back();};

		/// Erasing all previous transformations and store identity transformation
		void loadIdentity();
		/// Erasing previous transformation and store a particular transformation
		void loadTransformation(const Matrix4D& transfo);
		/// Compose top level matrix with a new transformation
		void addTransformation(const Matrix4D& transfo);
		/// Compose top level matrix with a new translation
		void addTranslation(const Vector3D& trans);
		/// Compose top level matrix with a new rotation
		void addRotation(float angle,const Vector3D& axe);
		/// Compose top level matrix with a new homothety (same on 3 axes)
		void addHomothety(float scale);
		/// Compose top level matrix with a new homothety varying on the 3 axis
		void addHomothety(const Vector3D& scale);
	};

	inline void MatrixStack::loadIdentity() {
		stack.back() = Matrix4D();
	}

	inline void MatrixStack::loadTransformation(const Matrix4D& transfo) {
		stack.back() = transfo;
	}

	inline void MatrixStack::addTransformation(const Matrix4D& transfo) {
		stack.back() *= transfo;
	}

	inline void MatrixStack::addTranslation(const Vector3D& trans) {
		stack.back() *= Matrix4D::translation(trans);
	}

	inline void MatrixStack::addRotation(float angle,const Vector3D& axe) {
		stack.back() *= Matrix4D::rotation(angle,axe);
	}

	inline void MatrixStack::addHomothety(float scale) {
		stack.back() *= Matrix4D::homothety(scale,scale,scale);
	}

	inline void MatrixStack::addHomothety(const Vector3D& scale) {
		stack.back() *= Matrix4D::homothety(scale.x,scale.y,scale.z);
	}

};

#endif


