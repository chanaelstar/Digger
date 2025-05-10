/***************************************************************************
                          mesh.hpp  -  description
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

#ifndef _STP3D_MESH_HPP_
#define _STP3D_MESH_HPP_

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#define GL4_PROTOTYPES 1


#include <iostream>
#include <string>
#include <vector>
#include "gl_tools.hpp"

namespace STP3D {

	/**
	  * \brief Mesh class allows to store generic informations about a mesh 
	  * that has no indirect order.
	  * Mesh class allows to store several float buffer to use with a GL shaders.
	  * Such buffers are not interleaved and each has a semantic on his own. 
	  * Note that a mesh MUST have at least one buffer of coordinates.
	  * This class allows also the creation of the corresponding VBO.
	  * This class may or may not store the data.
	  */
	class StandardMesh {
	public:
		/// Standard construtor. Creates an empty mesh withouh any information.
		StandardMesh(unsigned int elts = 0,unsigned int new_gl_type = GL_TRIANGLES) 
			: nb_elts(elts),gl_type_mesh(new_gl_type),id_vao(0) {
			buffers.clear();
			size_one_elt.clear();
			attr_id.clear();
			attr_semantic.clear();
			vbo_id.clear();
		};
		~StandardMesh();

		/// Set the number of elements in each buffers
		void setNbElt(unsigned int elts) {nb_elts = elts;};
		void addOneBuffer(unsigned int id_attribute,unsigned int one_elt_size,
		                  float* data,std::string semantic,bool copy=false);
		void releaseCPUMemory();
		void reInit();
		/*****************************************************************
		 *                      GL RELATED FUNCTIONS
		 *****************************************************************/
		void changeType(unsigned int new_gl_type) {gl_type_mesh = new_gl_type;};
		bool createVAO();
		unsigned int getIdVAO();
		void draw() const;
private:
		//  User defined members
		/// All the data in CPU buffers
		std::vector<float*> buffers;
		/// Number of elements in each buffer : must be common !!
		unsigned int nb_elts;
		/// Size of one element in each buffer
		std::vector<unsigned int> size_one_elt;
		/// Attribute id corresponding to each buffer
		std::vector<unsigned int> attr_id;
		/// Attribute semantic corresponding to each buffer
		std::vector<std::string> attr_semantic;
		/// Attribute semantic corresponding to each buffer
		std::vector<bool> copied;
		/// Attribute semantic corresponding to each buffer
		unsigned int gl_type_mesh;

		//  GL defined members
		/// Id of all VBO. Created by the GL API
		std::vector<unsigned int> vbo_id;
		/// Id of the corresponding VAO
		unsigned int id_vao;

	};

	inline StandardMesh::~StandardMesh() {
 		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			if (copied[i]) delete[](buffers[i]);
		}
 		size_one_elt.clear();
		attr_id.clear();
		attr_semantic.clear();
		glDeleteBuffers(vbo_id.size(),&(vbo_id[0]));
		vbo_id.clear();
		glDeleteVertexArrays(1,&id_vao);
	}

	inline bool StandardMesh::createVAO() {
		// Create and use the VAO
		glGenVertexArrays(1,&id_vao);
		if (id_vao == 0) {
			STP3D::setError("Unable to find a value for a VAO");
			return false;
		}
		glBindVertexArray(id_vao);

		// Check if mesh is defined
		if (buffers.size()==0) {
			STP3D::setError("Impossible to create VBO from empty buffers. This mesh has not been initialized");
			return false;
		}

		// Create all VBO (\TODO check every VBO is created)
		vbo_id.resize(buffers.size());

		glGenBuffers(buffers.size(),&(vbo_id[0]));

		// Transfer all data for all VBO from CPU to GPU
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			std::cerr<<"Id VBO for "<<attr_semantic[i]<<" : "<<vbo_id[i]<<std::endl;
			glBindBuffer(GL_ARRAY_BUFFER,vbo_id[i]);

			glBufferData(GL_ARRAY_BUFFER,nb_elts*size_one_elt[i]*sizeof(GLfloat),buffers[i],GL_STATIC_DRAW);

			glEnableVertexAttribArray(attr_id[i]);

			glVertexAttribPointer(attr_id[i], size_one_elt[i], GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
		
		glBindVertexArray(0);
		return true;
	}

	inline void StandardMesh::addOneBuffer(unsigned int id_attribute,unsigned int one_elt_size,
	                                       float* data,std::string semantic,bool copy) {
		if (copy) {
			float* tab = new float[one_elt_size*nb_elts];
			memcpy(tab,data,one_elt_size*nb_elts*sizeof(float));
			buffers.push_back(tab); 
			copied.push_back(true);
		}
		else {
			buffers.push_back(data);
			copied.push_back(false);
		}
		attr_id.push_back(id_attribute);
		size_one_elt.push_back(one_elt_size);
		attr_semantic.push_back(semantic);
	}

	inline void StandardMesh::draw() const {
		glBindVertexArray(id_vao);

		glDrawArrays(gl_type_mesh,0,nb_elts);

		glBindVertexArray(0);
	}

	inline void StandardMesh::reInit() {
 		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			if (copied[i]) delete[](buffers[i]);
		}
		buffers.clear();
		copied.clear();
 		size_one_elt.clear();
		attr_id.clear();
		attr_semantic.clear();
		glDeleteBuffers(vbo_id.size(),&(vbo_id[0]));
		vbo_id.clear();
		glDeleteVertexArrays(1,&id_vao);
	}

	inline void StandardMesh::releaseCPUMemory() {
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			if (copied[i] && buffers[i]) delete[](buffers[i]);
			buffers[i] = NULL;
		}
	}
};

#endif


