#pragma once

#include <iostream>
#include <cassert>
#include "tools/mesh.hpp"
#include "tools/vector3d.hpp"

using namespace STP3D;

namespace glbasimac {

struct GLBI_Convex_2D_Shape {
	GLBI_Convex_2D_Shape(unsigned int dim = 2)
		:nb_pts(0),dimension(dim),shape(0,GL_LINE_LOOP) {
		assert((dimension == 2) || (dimension ==3));
	};

	~GLBI_Convex_2D_Shape() {
		coord_pts.clear();
	};

	void initShape(const std::vector<float> in_coord);

	void changeNature(unsigned int new_gl_type);

	void drawShape();

	// Application and GL parameters
	unsigned int nb_pts;
	unsigned int dimension;
	StandardMesh shape;
	std::vector<float> coord_pts;
};

}
