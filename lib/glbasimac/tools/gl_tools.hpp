/***************************************************************************
                          matrix4d.hpp  -  description
                             -------------------
    copyright            : (C) 2012 by Biri Venceslas
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

#ifndef _STP3D_GL_TOOLS_HPP_
#define _STP3D_GL_TOOLS_HPP_

#include "globals.hpp"
#include <string>

#ifndef GL_STP3D_DEFINED
#define GL_STP3D_DEFINED

#define GL_GLEXT_PROTOTYPES 1

#include "glad/glad.h"

#ifndef M_PI
#define M_PI 3.1415926535
#endif

/*
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glxext.h>
*/
namespace STP3D {

// ///////////////////////////////////////////////////////////////////////////
// GL check tool
// ///////////////////////////////////////////////////////////////////////////
inline std::string GetGLErrorString(GLenum error)
{
	switch (error)
	{
		case GL_INVALID_ENUM:
			return "INVALID_ENUM (An unacceptable value is specified for an enumerated argument)";
		case GL_INVALID_VALUE:
			return "INVALID_VALUE (A numeric argument is out of range)";
		case GL_INVALID_OPERATION:
			return "INVALID_OPERATION (The specified operation is not allowed in the current state)";
		case GL_STACK_OVERFLOW:
			return "STACK_OVERFLOW (An attempt has been made to perform an operation that would cause an internal stack to overflow)";
		case GL_STACK_UNDERFLOW:
			return "STACK_UNDERFLOW (An attempt has been made to perform an operation that would cause an internal stack to underflow)";
		case GL_OUT_OF_MEMORY:
			return "OUT_OF_MEMORY (There is not enough memory left to execute the command)";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "INVALID_FRAMEBUFFER_OPERATION (The framebuffer object is not complete)";
		default:
			{
				std::stringstream ss;
				ss << "Unknown OpenGL error (" << error << ")";
				return ss.str();
			}
	}
}
inline void checkGL(int line, const char *filename) {
	int err = glGetError();
	if(err != GL_NO_ERROR) {
		std::cerr << "ERROR GL : erreur dans le fichier " << filename << " à la ligne " ;
		std::cerr << line << " : " << GetGLErrorString(err) << std::endl;
		exit(0);
	}
}
#define CHECK_GL STP3D::checkGL(__LINE__, __FILE__);

#endif // GL_DEFINED

/** GL Tools.
  * This class is a set of GL tools functions
  * @author Venceslas BIRI (biri@univ-mlv.fr)
  */
class GLTools {
public: 
	static void takeSnapshot(int w,int h,unsigned char *image_data,unsigned int nb_channel = 3);
};

inline void GLTools::takeSnapshot(int w,int h,unsigned char *image_data,unsigned int nb_channel) {
	glPixelStorei(GL_PACK_ALIGNMENT,1);
	if(nb_channel == 1) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RED,GL_UNSIGNED_BYTE,image_data);
	if(nb_channel == 3) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RGB,GL_UNSIGNED_BYTE,image_data);
	if(nb_channel == 4) glReadPixels(0,0,(GLint)w,(GLint)h,GL_RGBA,GL_UNSIGNED_BYTE,image_data);
}

// Utility function
inline float deg2rad(float deg) {
	return M_PI*deg/180.;
}


} // End namespace
#endif
