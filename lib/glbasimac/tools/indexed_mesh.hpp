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

#ifndef _STP3D_INDEXED_MESH_HPP_
#define _STP3D_INDEXED_MESH_HPP_


#include <iostream>
#include <vector>
#include "globals.hpp"


namespace STP3D {

	/**
	  * \class IndexedMesh allows to store generic informations about an indexed mesh.
	  * IndexedMesh class allows to store several float buffer to use with a GL shaders in
	  * an indexed way. Such buffers are not interleaved and each has a semantic on his own. 
	  * Note that an indexed mesh MUST have at least one buffer of coordinates.
	  * This class allows also the creation of the corresponding VBO.
	  * This class may or may not store the data.
	  */
	class IndexedMesh {
	public:
		/// Standard construtor. Creates an empty mesh withouh any information.
		IndexedMesh(unsigned int n_prim = 0,unsigned int elts = 0,unsigned int new_gl_type = GL_TRIANGLES) : id_vao(0) {
			buffers.clear();
			size_one_elt.clear();
			attr_id.clear();
			attr_semantic.clear();
			vbo_id.clear();
			gl_type_mesh = new_gl_type;
			nb_primitive = n_prim;
			index_buffer = NULL;
			nb_idx_per_primitive = getNbIdxPerPrimitive();
			if (nb_primitive>0) {
				index_buffer = new unsigned int[nb_primitive*nb_idx_per_primitive];
			}
			nb_elts = elts;
			id_index = 0;
		};
		~IndexedMesh();
		
		//  User defined members
		/// The index buffer
		unsigned int* index_buffer;
		/// The number of primitive buffer
		unsigned int nb_primitive;
		/// The id of index buffer
		unsigned int id_index;
		/// All the data in CPU buffers
		std::vector<float*> buffers;
		/// Number of elements (vertex) in each buffer : must be common !!
		unsigned int nb_elts;
		/// Size of one element in each buffer
		std::vector<unsigned int> size_one_elt;
		/// Attribute id corresponding to each buffer
		std::vector<unsigned int> attr_id;
		/// Attribute semantic corresponding to each buffer
		std::vector<std::string> attr_semantic;
		/// Attribute semantic corresponding to each buffer
		unsigned int gl_type_mesh;

		//  GL defined members
		/// Id of all VBO. Created by the GL API
		std::vector<unsigned int> vbo_id;
		/// Id of the corresponding VAO
		unsigned int id_vao;

		/// Set the number of elements in each buffers
		void setNbElt(unsigned int elts) {nb_elts = elts;};
		void setNbIndex(unsigned int idx) {nb_primitive = idx;if (index_buffer) delete[](index_buffer);};
		void addIndexBuffer(unsigned int* data,bool copy = false);
		void addOneBuffer(unsigned int id_attribute,unsigned int one_elt_size,
		                  float* data,std::string semantic="",bool copy=false);
		void releaseCPUMemory();
		/*****************************************************************
		 *                      GL RELATED FUNCTIONS
		 *****************************************************************/
		void changeType(unsigned int new_gl_type) {gl_type_mesh = new_gl_type;};
		bool createVAO();
		void draw();

	private:
		unsigned int nb_idx_per_primitive;
		unsigned int getNbIdxPerPrimitive();

	};

	inline IndexedMesh::~IndexedMesh() {
		if (buffers.size()>0) {
			for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
				if (buffers[i]) delete[](buffers[i]);
			}
		}
		if (index_buffer) delete[](index_buffer);
	}

	inline unsigned int IndexedMesh::getNbIdxPerPrimitive() {
		if(gl_type_mesh == GL_POINTS) return 1;
		if(gl_type_mesh == GL_LINES) return 2;
		if(gl_type_mesh == GL_TRIANGLES) return 3;
		STP3D::setError("Wrong type of type in getNbIdxPerPrimitive");
		return 0;
	}


	inline bool IndexedMesh::createVAO() {
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
		}

		// Create all VBO (and check)
		vbo_id.resize(buffers.size());
		unsigned int* new_id = new unsigned int[buffers.size()];
		glGenBuffers(buffers.size(),new_id);
		for(unsigned int i=0;i<buffers.size();++i) {
			if (new_id[i]==0) {STP3D::setError("Unable to find an empty VBO");return false;}
			vbo_id[i]=new_id[i];
		}
		delete[](new_id);

		// Create the index VBO (and check)
		glGenBuffers(1,&id_index);
		if (id_index==0) {STP3D::setError("Unable to find an empty VBO for index buffer");return false;}


		// Transfer all data for all VBO from CPU to GPU
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			glEnableVertexAttribArray(attr_id[i]);

			glBindBuffer(GL_ARRAY_BUFFER,vbo_id[i]);

			glBufferData(GL_ARRAY_BUFFER,nb_elts*size_one_elt[i]*sizeof(GLfloat),buffers[i],GL_STATIC_DRAW);

			glVertexAttribPointer(attr_id[i], size_one_elt[i], GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
		// Transfer index data VBO from CPU to GPU
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,nb_idx_per_primitive*nb_primitive*sizeof(unsigned int),index_buffer,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		glBindVertexArray(0);
		return true;
	}

	inline void IndexedMesh::addIndexBuffer(unsigned int* data,bool copy) {
		if (copy) {
			memcpy(index_buffer,data,nb_idx_per_primitive*nb_primitive*sizeof(unsigned int));
		}
		else {
			if (index_buffer) delete[](index_buffer);
			index_buffer = data;
		}
	}

	inline void IndexedMesh::addOneBuffer(unsigned int id_attribute,unsigned int one_elt_size,
	                                       float* data,std::string semantic,bool copy) {
		if (copy) {
			float* tab = new float[one_elt_size*nb_elts];
			memcpy(tab,data,one_elt_size*nb_elts*sizeof(float));
			buffers.push_back(tab); 
		}
		else buffers.push_back(data);
		attr_id.push_back(id_attribute);
		size_one_elt.push_back(one_elt_size);
		attr_semantic.push_back(semantic);
	}

	inline void IndexedMesh::draw() {
		glBindVertexArray(id_vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,id_index);
		glDrawElements(gl_type_mesh,nb_primitive*nb_idx_per_primitive,GL_UNSIGNED_INT,0);

		glBindVertexArray(0);
	}


	inline void IndexedMesh::releaseCPUMemory() {
		for(std::vector<int>::size_type i = 0; i < buffers.size(); ++i) {
			if (buffers[i]) delete[](buffers[i]);
			buffers[i] = NULL;
		}
		if (index_buffer) {
			delete[](index_buffer);
			index_buffer = NULL;
		}
	}



};

#endif


