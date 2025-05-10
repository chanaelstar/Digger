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

#ifndef _STP3D_MESH_MANAGER_HPP_
#define _STP3D_MESH_MANAGER_HPP_


#include <iostream>
#include <vector>
#include <map>
#include "globals.hpp"


namespace STP3D {

	/**
	  * \brief Mesh manager store efficiently meshes using symbolic names.
	  * Mesh manager can be used to store, once, each mesh of a scene. It
	  * can also be used for memory management. Mesh manager can handle only
	  * generic mesh type which are StandardMesh and IndexedMesh
	  */
	class MeshManager {
	public:
		/// Standard construtor. Creates an empty mesh withouh any information.
		MeshManager() {
			indexed_meshes.clear();
			standard_meshes.clear();
		};
		~MeshManager();

		/// All indexed meshes store in a hash table indexed by names
		typedef std::map<std::string,const IndexedMesh*> map_idx_mesh;
		map_idx_mesh indexed_meshes;
		/// All standard meshes store in a hash table indexed by names
		typedef std::map<std::string,const StandardMesh*> map_mesh;
		map_mesh standard_meshes;

		/// Get the total number of meshes
		unsigned int getNbMeshes() {return indexed_meshes.size()+standard_meshes.size();}
		/** Add an indexed mesh to the manager.
		  * \param name the id/name of the mesh. Must be unique for each particular mesh
		  * \param addedMesh the mesh to be added
		  */
		void addIdxMesh(const std::string name,const IndexedMesh* addedMesh);
		/** Add a standard mesh to the manager.
		  * \param name the id/name of the mesh. Must be unique for each particular mesh
		  * \param addedMesh the mesh to be added
		  */
		void addMesh(const std::string name,const StandardMesh* addedMesh);
		/** Get a standard mesh from the stored list.
		  * \param name the id/name of the mesh. Must be unique for each particular mesh
		  * \return The found mesh. If it does not exist, return NULL
		  */
		const StandardMesh* getMesh(const std::string name);
		/** Get an indexed mesh from the stored list.
		  * \param name the id/name of the mesh. Must be unique for each particular mesh
		  * \return The found mesh. If it does not exist, return NULL
		  */
		const IndexedMesh* getIdxMesh(const std::string name);
	};

	inline MeshManager::~MeshManager() {
		for(map_mesh::iterator it = standard_meshes.begin();it!=standard_meshes.end();it++) {
			std::cerr<<"Name "<<it->first<<" is deleted"<<std::endl;
			if (it->second) delete(it->second);
		}
		for(map_idx_mesh::iterator it = indexed_meshes.begin();it!=indexed_meshes.end();it++) {
			std::cerr<<"Name "<<it->first<<" is deleted"<<std::endl;
			if (it->second) delete(it->second);
		}
	}

	inline void MeshManager::addIdxMesh(const std::string name,const IndexedMesh* addedMesh) {
		if (indexed_meshes.find(name) == indexed_meshes.end()) {
			indexed_meshes.insert(std::pair<std::string,const IndexedMesh*>(name,addedMesh));
		}
		else {
			std::cerr<<"Mesh "<<name<<"already loader in manager"<<std::endl;
		}
	}

	inline void MeshManager::addMesh(const std::string name,const StandardMesh* addedMesh) {
		if (standard_meshes.find(name) == standard_meshes.end()) {
			standard_meshes.insert(std::pair<std::string,const StandardMesh*>(name,addedMesh));
		}
		else {
			std::cerr<<"Mesh "<<name<<"already loader in manager"<<std::endl;
		}
	}

	inline const StandardMesh* MeshManager::getMesh(const std::string name) {
		map_mesh::iterator res = standard_meshes.find(name);
		return (res == standard_meshes.end()) ? NULL : res->second;
	}

	inline const IndexedMesh* MeshManager::getIdxMesh(const std::string name) {
		map_idx_mesh::iterator res = indexed_meshes.find(name);
		return (res == indexed_meshes.end()) ? NULL : res->second;
	}

};

#endif


