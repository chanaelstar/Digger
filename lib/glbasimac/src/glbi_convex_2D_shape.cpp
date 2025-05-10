#include "glbasimac/glbi_convex_2D_shape.hpp"
#include <array>

namespace glbasimac {

	void GLBI_Convex_2D_Shape::initShape(const std::vector<float> in_coord) {
		coord_pts.clear();
		if (dimension == 2) {
			assert(in_coord.size()%2 == 0);
			nb_pts = in_coord.size()/2;
		}
		else {
			assert(in_coord.size()%3 == 0);
			nb_pts = in_coord.size()/3;
			std::cerr<<"NB POINT : "<<nb_pts<<std::endl;
		}

		shape.setNbElt(nb_pts);
		coord_pts = in_coord;
		shape.addOneBuffer(0,dimension,coord_pts.data(),"Coordinates",false);

		// for(size_t i{0};i<in_coord.size()/2;i++) {
		// 	color_pts.push_back(c_r);
		// 	color_pts.push_back(c_v);
		// 	color_pts.push_back(c_b);
		// }
		// shape.addOneBuffer(3,3,color_pts.data(),"Color",false);

		if(!shape.createVAO()) {
			std::cerr<<"Unable to create VAO for Set of Points"<<std::endl;
			exit(1);
		}
	}

	void GLBI_Convex_2D_Shape::changeNature(unsigned int new_gl_type) {
		shape.changeType(new_gl_type);
	}

	void GLBI_Convex_2D_Shape::drawShape() {
		shape.draw();
	}

}
