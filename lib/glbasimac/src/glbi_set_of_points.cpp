#include "glbasimac/glbi_set_of_points.hpp"
#include <array>

namespace glbasimac {

	void GLBI_Set_Of_Points::initSet(const std::vector<float> in_coord,float c_r,float c_v,float c_b) {
		coord_pts.clear();
		color_pts.clear();
		if (dimension == 2) {
			assert(in_coord.size()%2 == 0);
			nb_pts = in_coord.size()/2;
			std::cerr<<"Nb points "<<nb_pts<<std::endl;
		}
		if (dimension == 3) {
			assert(in_coord.size()%3 == 0);
			nb_pts = in_coord.size()/3;
			std::cerr<<"Nb points "<<nb_pts<<std::endl;
		}

		pts.setNbElt(nb_pts);
		coord_pts = in_coord;
		pts.addOneBuffer(0,dimension,coord_pts.data(),"Coordinates",false);

		for(size_t i{0};i<in_coord.size()/dimension;i++) {
			color_pts.push_back(c_r);
			color_pts.push_back(c_v);
			color_pts.push_back(c_b);
		}

		pts.addOneBuffer(3,3,color_pts.data(),"Color",false);

		if(!pts.createVAO()) {
			std::cerr<<"Unable to create VAO for Set of Points"<<std::endl;
			exit(1);
		}
	}

	void GLBI_Set_Of_Points::initSet(const std::vector<float> in_coord,const std::vector<float> in_color) {
		coord_pts.clear();
		color_pts.clear();
		if (dimension == 2) {
			assert(in_color.size()/3 == in_coord.size()/2);
			nb_pts = in_coord.size()/2;
			std::cerr<<"Nb ponts "<<nb_pts<<std::endl;

		}
		if (dimension == 3) {
			assert(in_color.size()/3 == in_coord.size()/3);
			nb_pts = in_coord.size()/3;
		}
		pts.setNbElt(nb_pts);

		coord_pts = in_coord;
		pts.addOneBuffer(0,dimension,coord_pts.data(),"Coordinates",false);

		color_pts = in_color;
		pts.addOneBuffer(3,3,color_pts.data(),"Color",false);

		if(!pts.createVAO()) {
			std::cerr<<"Unable to create VAO for Set of Points"<<std::endl;
			exit(1);
		}
	}

	void GLBI_Set_Of_Points::addAPoint(float* n_coord,float* n_col) {
		coord_pts.push_back(n_coord[0]);
		coord_pts.push_back(n_coord[1]);
		if (dimension == 3) coord_pts.push_back(n_coord[2]);
		color_pts.push_back(n_col[0]);
		color_pts.push_back(n_col[1]);
		color_pts.push_back(n_col[2]);

		pts.setNbElt(color_pts.size()/3);
		pts.reInit();

		pts.addOneBuffer(0,2,coord_pts.data(),"Coordinates",false);
		pts.addOneBuffer(3,3,color_pts.data(),"Color",false);
		if(!pts.createVAO()) {
			std::cerr<<"Unable to create VAO for Set of Points"<<std::endl;
			exit(1);
		}
	}

	void GLBI_Set_Of_Points::changeNature(unsigned int new_gl_type) {
		pts.changeType(new_gl_type);
	}

	void GLBI_Set_Of_Points::drawSet() {
		pts.draw();
	}

	
}
