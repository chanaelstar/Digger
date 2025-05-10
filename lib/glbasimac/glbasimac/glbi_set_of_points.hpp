#pragma once

#include <iostream>
#include <cassert>
#include "tools/mesh.hpp"

using namespace STP3D;

namespace glbasimac {

#define MAX_NB_POINTS_SET_OF_POINTS 100

struct GLBI_Set_Of_Points {
	// Constructor. Initially we render points
	GLBI_Set_Of_Points(unsigned int dim = 2)
		:nb_pts(0),pts(0,GL_POINTS),dimension(dim) {
		assert((dimension == 2) || (dimension ==3));
	};

	~GLBI_Set_Of_Points() {
		coord_pts.clear();
		color_pts.clear();
	};

	// Initialize the set of points to render
	void initSet(const std::vector<float> in_coord,float c_r,float c_v,float c_b);
	void initSet(const std::vector<float> in_coord,const std::vector<float> in_color);

	// Allow to add a point. Not an efficient way for rendering...
	void addAPoint(float* new_coord,float* new_color);

	// Allow to switch between points (GL_POINTS) and a line (GL_LINE_STRIP)
	void changeNature(unsigned int new_gl_type);

	void drawSet();

	// Application and GL parameters
	unsigned int nb_pts;
	StandardMesh pts;
	unsigned int dimension;
	std::vector<float> coord_pts;
	std::vector<float> color_pts;
};

}
