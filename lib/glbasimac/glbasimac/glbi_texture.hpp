#pragma once

#include <iostream>
#include <cassert>
#include "tools/gl_tools.hpp"

using namespace STP3D;

namespace glbasimac {

struct GLBI_Texture {
	GLBI_Texture() : id_in_GL(0),width(0),height(0),channels(0) {
	};

	~GLBI_Texture() {
		glDeleteTextures(1,&id_in_GL);
	};

	void createTexture();
	void attachTexture();
	void detachTexture();
	void loadImage(unsigned int w,unsigned int h,unsigned int n_chan,unsigned char* pixels);
	void setParameters(unsigned int param,unsigned int value);

	// Texture parameters
	unsigned int id_in_GL;
	unsigned int width,height;
	unsigned int channels;
};

}
