#include "glbasimac/glbi_texture.hpp"

namespace glbasimac {
	void GLBI_Texture::createTexture() {
		glGenTextures(1,&id_in_GL);
		if (id_in_GL == 0) {
			std::cerr<<"Unable to create GL Texture. Exiting"<<std::endl;
			exit(1);
		}
	}

	void GLBI_Texture::attachTexture() {
		if (!id_in_GL) {
			std::cerr<<"Unable to attach an uncreated Texture"<<std::endl;
			exit(1);
		}
		glBindTexture(GL_TEXTURE_2D,id_in_GL);
	}

	void GLBI_Texture::loadImage(unsigned int w,unsigned int h,unsigned int n_chan,unsigned char* pixels) {
		if (!id_in_GL) {
			std::cerr<<"Unable to attach an uncreated Texture"<<std::endl;
			exit(1);
		}
		width = w;
		height = h;
		channels = n_chan;
		if (channels == 3) glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,pixels);
		if (channels == 4) glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
	}

	void GLBI_Texture::detachTexture() {
		glBindTexture(GL_TEXTURE_2D,0);
	}

	void GLBI_Texture::setParameters(unsigned int param,unsigned int value) {
		if (!id_in_GL) {
			std::cerr<<"Unable to set parameters of an uncreated texture"<<std::endl;
			exit(1);
		}
		glTexParameteri(GL_TEXTURE_2D,param,value);
	}

	
}
