/***************************************************************************
                        basic_mesh.hpp  -  description
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

#ifndef _STP3D_BASIC_MESH_HPP_
#define _STP3D_BASIC_MESH_HPP_


#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "mesh.hpp"
#include "indexed_mesh.hpp"


namespace STP3D {

	/** BASIC MESH CREATION TOOLS
	  * All these functions create mesh
	  * Data are stored in buffers but VBO must be build by the application. 
	  * Index chosen for each buffer follow this convention.
	  * Index 0 : coordinates (mandatory)
	  * Index 1 : normals
	  * Index 2 : texture coordinates
	  * Index 3 : colors
	  */

	/** Frame creation
	  * This is a basic frame visualization. It shows the 3 axis with red,green,blue colors. 
	  * \param lg is the length of each axis
	  */
	StandardMesh* createRepere(float lg=1.0f);

	/** Create a basic rectangle on the xz plane
	  * This function create a mesh (2 triangles) for a basic quad set on the (xz)-plane.
	  * The mesh stores only the coordinates, leaving all material properties to the application
	  * The position and size of the rectangle is (0,0) to (sizex,sizez).
	  * \param sizex,sizez size of the rectangle along x and z planes
	  */
	StandardMesh* basicRect(float sizex,float sizez);

	/** Create a basic cone on the y axis
	  * This function create a mesh for a basic cone along the y axis.
	  * The mesh stores only the coordinates, leaving all material properties to the application
	  * The top of the cone is at height \a h.
	  * The problem reside in the fact that each triangle is dependant of the following one 
	  * (due to the use of GL_TRIANGLE_STRIP) therefore we see some inconsistency in the illumination
	  * \param h height of the cone
	  * \param radius radius of the cone
	  * \param radius_up to create a truncated cone with a up radius of radius_up
	  * \param nb_div number of division (in triangles) for the cone
	  */
	StandardMesh* basicCone(float h,float radius,float radius_up = 0.0,unsigned int nb_div = 64);

	/** Create a basic cylinder on the y axis
	  * This function create a mesh for a basic cylinder along the y axis without top and bottom closure.
	  * The mesh stores only the coordinates, leaving all material properties to the application
	  * The height of the cylinder is \a h.
	  * \param h height of the cylinder
	  * \param radius radius of the cylinder
	  * \param div_round number of division for the cylinder
	  * \param div_height number of division for the cylinder
	  */
	IndexedMesh* basicCylinder(float h,float radius,unsigned int div_round = 64,unsigned int div_height = 1);

	/** Create a basic cube surounding the origine
	  * This function create a cube centered at the origine with a specific size
	  * The mesh stores only the coordinates, leaving all material properties to the application
	  * The size of one edge is \a width.
	  * \param width size of one edge, therefore along an axe, the cube is located at -size/2 to size/2
	  */
	IndexedMesh* basicCube(float width = 1.0f);

	/** Create a basic sphere surounding the origine
	  * This function create a sphere centered at the origine with a specific radius
	  * The mesh stores only the coordinates, leaving all material properties to the application
	  * \param radius radius of the sphere
	  */
	IndexedMesh* basicSphere(float radius = 1.0f,unsigned int nb_div_h=64, unsigned int nb_div_circle=64);

	inline StandardMesh* createRepere(float lg) {
		StandardMesh* repere = new StandardMesh(6,GL_LINES);
		float coord[18] = {0.0,0.0,0.0,lg,0.0,0.0,
		                   0.0,0.0,0.0,0.0,lg,0.0,
		                   0.0,0.0,0.0,0.0,0.0,lg};
		float color[18] = {1.0,0.0,0.0,1.0,0.0,0.0,
		                   0.0,1.0,0.0,0.0,1.0,0.0,
		                   0.0,0.0,1.0,0.0,0.0,1.0};
		//std::string sem1 = "coordinates";
		//std::string sem2 = "colors";
		repere->addOneBuffer(0,3,coord,"coordinates",true);
		repere->addOneBuffer(3,3,color,"colors",true);
		return repere;
	}

	inline StandardMesh* basicRect(float sizex,float sizez) {
		StandardMesh* rect = new StandardMesh(4,GL_TRIANGLE_STRIP);
		float coord[12] = {0.0,0.0,0.0,
		                   0.0,0.0,sizez,
		                   sizex,0.0,0.0,
		                   sizex,0.0,sizez};
		float normals[12] = {0.0,1.0,0.0,0.0,1.0,0.0,
		                     0.0,1.0,0.0,0.0,1.0,0.0};
		float uvs_rect[8] = {0.0,0.0,0.0,1.0,1.0,0.0,1.0,1.0};
		rect->addOneBuffer(0,3,coord,"coordinates",true);
		rect->addOneBuffer(1,3,normals,"normals",true);
		rect->addOneBuffer(2,2,uvs_rect,"uvs",true);
		return rect;
	}

	inline StandardMesh* basicCone(float h,float radius,float r_up,unsigned int nb_div) {
		StandardMesh* cone = new StandardMesh((nb_div+1)*2,GL_TRIANGLE_STRIP);
		float* coord = new float[(nb_div+1)*2*3];
		float* normals = new float[(nb_div+1)*2*3];
		float* uv = new float[(nb_div+1)*2*2];
		double cos_pt,sin_pt;
		double angle=0;
		// 
		double angle_to_axis_y = atan(radius/h);
		double cos_to_xz = cos(angle_to_axis_y);
		double sin_to_xz = sin(angle_to_axis_y);
		//std::cerr<<"P : "<<angle_to_axis_y*180.0/M_PI<<" ("<<cos_to_xz<<") "<<std::endl;
		for(unsigned int i=0;i<=nb_div;i++,angle+=2*M_PI/nb_div) {
			// Head of the cone (repeated)
			cos_pt = cos(angle);
			sin_pt = sin(angle);
			coord[6*i  ] = radius*cos_pt; coord[6*i+1] = 0.0; coord[6*i+2] = radius*sin_pt;
			coord[6*i+3] = r_up*cos_pt;           coord[6*i+4] = h;   coord[6*i+5] =  r_up*sin_pt;
			normals[6*i  ] = cos_pt*cos_to_xz; normals[6*i+1] = sin_to_xz; normals[6*i+2] = sin_pt*cos_to_xz;
			normals[6*i+3] = cos_pt*cos_to_xz; normals[6*i+4] = sin_to_xz; normals[6*i+5] = sin_pt*cos_to_xz;
			uv[4*i  ] = (float)i/nb_div; uv[4*i+1] = 0.0;
			uv[4*i+2] = (float)i/nb_div; uv[4*i+3] = 1.0;
		}
		cone->addOneBuffer(0,3,coord,"coordinates",false);
		cone->addOneBuffer(1,3,normals,"normals",false);
		cone->addOneBuffer(2,2,uv,"uvs",false);
		return cone;
	}

	inline IndexedMesh* basicCylinder(float h,float radius,unsigned int div_round,unsigned int div_height) {
		unsigned int nb_points = (div_round+1)*(div_height+1);
		unsigned int nb_prim = 2*div_round*div_height;
		IndexedMesh* cyl = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
		float* coord = new float[nb_points*3];
		float* normals = new float[nb_points*3];
		float* uv = new float[nb_points*2];
		unsigned int* indexes = new unsigned int[3*nb_prim];
		double cos_pt,sin_pt;
		double angle = 0.0;
		double height = 0.0;
		double incr_angle = 2*M_PI/div_round;
		// Calcul des points
		for(unsigned int j=0;j<=div_height;j++,height+=h/div_height) {
			for(unsigned int i=0;i<=div_round;i++,angle+=incr_angle) {
				cos_pt = cos(angle);
				sin_pt = sin(angle);
				coord[3*j*(div_round+1)+3*i  ] = radius*cos_pt;
				coord[3*j*(div_round+1)+3*i+1] = height;
				coord[3*j*(div_round+1)+3*i+2] = radius*sin_pt;
				normals[3*j*(div_round+1)+3*i  ] = cos_pt;
				normals[3*j*(div_round+1)+3*i+1] = 0.0;
				normals[3*j*(div_round+1)+3*i+2] = sin_pt;
				uv[2*j*(div_round+1)+2*i  ] = i/(float)div_round;
				uv[2*j*(div_round+1)+2*i+1] = j/(float)div_height;
			}
		}
		
		// Computation of indexes
		for(unsigned int  j=0;j<div_height;j++) {
			for(unsigned int i=0;i<div_round;i++) {
				indexes[j*div_round*6+6*i+0] = j*(div_round+1)+i;         // A
				indexes[j*div_round*6+6*i+1] = j*(div_round+1)+(i+1);     // B
				indexes[j*div_round*6+6*i+2] = (j+1)*(div_round+1)+i;     // C
				indexes[j*div_round*6+6*i+3] = (j+1)*(div_round+1)+i;     // C
				indexes[j*div_round*6+6*i+4] = j*(div_round+1)+(i+1);     // B
				indexes[j*div_round*6+6*i+5] = (j+1)*(div_round+1)+(i+1); // D
			}
		}

		cyl->addOneBuffer(0,3,coord,"coordinates",false);
		cyl->addOneBuffer(1,3,normals,"normals",false);
		cyl->addOneBuffer(2,2,uv,"uvs",false);
		cyl->addIndexBuffer(indexes,false);
		return cyl;
	}

	inline IndexedMesh* basicCube(float width) {
		unsigned int nb_points = 24;
		unsigned int nb_prim = 12;
		IndexedMesh* cube = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
		float coord[3*24] = {
			// Face front (z = width/2) (A,B,C,D)
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			width/2.0f,-width/2.0f,width/2.0f,          // B
			width/2.0f,width/2.0f,width/2.0f,           // C
			-width/2.0f,width/2.0f,width/2.0f,          // D
			// Face back (z = -width/2) (E,F,G,H)
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			width/2.0f,width/2.0f,-width/2.0f,          // G
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			// Face left (x = -width/2) (E,A,D,H)
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			-width/2.0f,width/2.0f,width/2.0f,          // D
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			// Face right (x = +width/2) (B,F,G,C)
			width/2.0f,-width/2.0f,width/2.0f,          // B
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			width/2.0f,width/2.0f,-width/2.0f,			// G
			width/2.0f,width/2.0f,width/2.0f,			// C
			// Face top (y = +width/2) (D,C,G,H)
			-width/2.0f,width/2.0f,width/2.0f,          // D
			width/2.0f,width/2.0f,width/2.0f,           // C
			width/2.0f,width/2.0f,-width/2.0f,          // G
			-width/2.0f,width/2.0f,-width/2.0f,         // H
			// Face down (y = -width/2) (A,B,F,E)
			-width/2.0f,-width/2.0f,width/2.0f,         // A
			-width/2.0f,-width/2.0f,-width/2.0f,        // E
			width/2.0f,-width/2.0f,-width/2.0f,         // F
			width/2.0f,-width/2.0f,width/2.0f           // B
		};
		float normals[3*24] = {
			// Face front (z = width/2) (A,B,C,D)
			0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,
			// Face back (z = -width/2) (E,H,G,F)
			0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,	0.0f,0.0f,-1.0f,
			// Face left (x = -width/2) (E,A,D,H)
			-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,		-1.0,0.0,0.0,
			// Face right (x = +width/2) (B,F,G,C)
			1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,		1.0,0.0,0.0,
			// Face top (y = +width/2) (D,C,G,H)
			0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,		0.0,1.0,0.0,
			// Face down (y = -width/2) (A,B,F,E)
			0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0,		0.0,-1.0,0.0
		};
		float uv[2*24] = {
			// Face front (z = width/2) (A,B,C,D)
			0.0,0.0,		1.0,0.0,		1.0,1.0,		0.0,1.0,
			// Face back (z = -width/2) (E,F,G,H)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face left (x = -width/2) (E,A,D,H)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face right (x = +width/2) (B,F,G,C)
			1.0,0.0,		0.0,0.0,		0.0,1.0,		1.0,1.0,
			// Face top (y = +width/2) (D,C,G,H)
			0.0,1.0,		1.0,1.0,		1.0,0.0,		0.0,0.0,
			// Face down (y = -width/2) (A,E,F,B)
			0.0,0.0,		1.0,0.0,		1.0,1.0,		0.0,1.0
		};
		unsigned int indexes[3*12] = {
			// Face front (z = width/2) (A,B,C,D)
			0,1,2,		0,2,3,
			// Face back (z = -width/2) (E,F,G,H)
			4,5,6,		4,6,7,
			// Face left (x = -width/2) (E,A,D,H)
			8,9,10,		8,10,11,
			// Face right (x = +width/2) (B,F,G,C)
			12,13,14,		12,14,15,
			// Face top (y = +width/2) (D,C,G,H)
			16,17,18,		16,18,19,
			// Face down (y = -width/2) (A,E,F,B)
			20,21,22,		20,22,23
		};


		cube->addOneBuffer(0,3,coord,"coordinates",true);
		cube->addOneBuffer(1,3,normals,"normals",true);
		cube->addOneBuffer(2,2,uv,"uvs",true);
		cube->addIndexBuffer(indexes,true);
		return cube;
	}

	inline IndexedMesh* basicSphere(float radius,unsigned int nb_div_h, unsigned int nb_div_circle) {
		assert(nb_div_h>1);
		unsigned int nb_points = 2+((nb_div_h-1)*(nb_div_circle+1));
		
		unsigned int nb_prim = nb_div_circle*2 + (nb_div_h-2)*2*nb_div_circle;
		IndexedMesh* sphere = new IndexedMesh(nb_prim,nb_points,GL_TRIANGLES);
		float* coord = new float[nb_points*3];
		float* normals = new float[nb_points*3];
		float* uv = new float[nb_points*2];
		float* pt_coord = coord;
		float* pt_nml = normals;
		float* pt_uv = uv;
		
		// Bottom of the sphere
		pt_coord[0] = pt_coord[2] = 0.0;
		pt_coord[1] = -radius;
		
		pt_nml[0] = pt_nml[2] = 0.0;
		pt_nml[1] = -1.0;
		
		pt_uv[0] = 0.0;
		pt_uv[1] = 0.0;
		
		pt_coord += 3;
		pt_nml += 3;
		pt_uv += 2;

		// All other slices
		float inner_radius,y_value;
		float step_y_angle = M_PI/(float)nb_div_h;
		float step_circle_angle = 2*M_PI/(float)nb_div_circle;
		for(unsigned int i = 1; i <= (nb_div_h-1) ; i++) {
			y_value = sin(-(M_PI/2.0)+i*step_y_angle);
			inner_radius = cos(-M_PI/2.0+i*step_y_angle);
			for(unsigned int j=0; j <= nb_div_circle ; j++) {
				pt_coord[0] = radius*inner_radius*cos(j*step_circle_angle);
				pt_coord[2] = -radius*inner_radius*sin(j*step_circle_angle);
				pt_coord[1] = radius*y_value;
				pt_coord+=3;

				pt_nml[0] = inner_radius*cos(j*step_circle_angle);
				pt_nml[2] = -inner_radius*sin(j*step_circle_angle);
				pt_nml[1] = y_value;
				pt_nml+=3;
				
				pt_uv[0] = (float)j/(float)nb_div_circle;
				pt_uv[1] = (float)i/(float)nb_div_h;
				pt_uv+=2;
			}
		}
		// Top of the sphere 
		pt_coord[0] = pt_coord[2] = 0.0;
		pt_coord[1] = radius; 
		pt_coord+=3;
		
		pt_nml[0] = pt_nml[2] = 0.0;
		pt_nml[1] = 1.0;
		pt_nml+=2;
		
		pt_uv[0] = 0.0;
		pt_uv[1] = 1.0;
		pt_uv+=2;
		
		
		// LES INDICES
		unsigned int* indexes = new unsigned int[3*nb_prim];
		unsigned int* pt_indx = indexes;
		
		// South pole
		for(unsigned int i=0;i<nb_div_circle;i++) {
			pt_indx[0] = 0;
			pt_indx[1] = 1+i;
			pt_indx[2] = 1+((i+1)%nb_div_circle);
			pt_indx+=3;
		}
		// All quads
		for(unsigned int i = 1; i < (nb_div_h-1) ; i++) {
			for(unsigned int j=0; j < nb_div_circle ; j++) {
				pt_indx[0] = 1 +  j    + (i-1)  * (nb_div_circle+1);
				pt_indx[1] = 1 + (j+1) +   i    * (nb_div_circle+1);
				pt_indx[2] = 1 +  j    +   i    * (nb_div_circle+1);
				pt_indx[3] = 1 +  j    + (i-1)  * (nb_div_circle+1);
				pt_indx[4] = 1 + (j+1) + (i-1)  * (nb_div_circle+1);
				pt_indx[5] = 1 + (j+1) +   i    * (nb_div_circle+1);
				pt_indx += 6;
			}
		}
		// North pole
		int k = (nb_div_h-2);
		for(unsigned int j=0;j<nb_div_circle;j++) {
			pt_indx[0] = 1 +  j    + k * (nb_div_circle+1);
			pt_indx[1] = 1 + (j+1) + k * (nb_div_circle+1);
			pt_indx[2] = 1+((nb_div_h-1)*(nb_div_circle+1));
			pt_indx += 3;
		}
		
		// Affichage indice
		/*
		for(unsigned int i=0;i<nb_prim;i++) {
			std::cerr<<"Primitive "<<i<<" : "<<indexes[3*i]<<" / "<<indexes[3*i+1]<<" / "<<indexes[3*i+2]<<std::endl;
			
		}
		*/

		sphere->addOneBuffer(0,3,coord,"coordinates",false);
		sphere->addOneBuffer(1,3,normals,(char*)"normals",false);
		sphere->addOneBuffer(2,2,uv,(char*)"uvs",false);
		sphere->addIndexBuffer(indexes,false);
		
		//delete[](coord);
		//delete[](indexes);
		return sphere;
	}

};

#endif


