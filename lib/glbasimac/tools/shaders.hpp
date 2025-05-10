/***************************************************************************
                      shaders.hpp  -  description
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

#ifndef _STP3D_SHADERS_HPP_
#define _STP3D_SHADERS_HPP_


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <sys/stat.h>
#include <filesystem>
#include <vector>
#include "globals.hpp"
#include "gl_tools.hpp"
#include <cassert>

namespace STP3D {

	enum ShaderType {Vertex,Fragment,Geometry,TesControl,TesEval};

	/**
	  \brief ShaderManager class allows to create and use shaders

	  Shaders are small programs written in GLSL <i>Graphic Library Shading Language</i> with syntax similar to C. 
	  These little programs are designed to design/overide stages of OpenGL <i>pipeline</i>:
	  <ul>
	  <li> Vertex processing: Vertex shader
	  <li> Fragment texturing and coloring: Fragment shader
	  <li> Geometry processing : Geometry shader
	  <li> Tesselation operations : Tesselation control and tesselation shader
	  </ul>
	  The class ShaderManager offers tools to load and use easily such shaders
	*/
	class ShaderManager {
	public:
		static void printLog(GLuint object, bool isShader, const char* str);
		static GLuint loadShader(std::filesystem::path const& vertexFile, std::filesystem::path const& fragmentFile, bool v = false);
		static GLuint loadShader(const std::vector<const char *> filenames, const std::vector<ShaderType> shaderTypes, bool v = false);
		static bool linkProgram(GLuint programObject, bool verbose);
		static bool compileShader(const char *filename, const ShaderType shaderType, GLuint& programObject, bool verbose);
		static void deleteProgram(GLuint programObject);
		static bool loadSource(const char* filename, char** source);
		static bool areShadersSupported(bool v);

		// SMALL TOOLS
		static std::string writeShaderType(ShaderType shdtype);
		static GLenum convertToGLShaderType(ShaderType shdtype);
	};

	inline void ShaderManager::printLog(GLuint object, bool isShader, const char* str) {
		GLsizei length = 0;

		if (isShader)
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
		else
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
		if(length > 1) {
			// If there is something to see
			GLchar *logStr = new GLchar[length];
			// Get the log
			if (isShader)
				glGetShaderInfoLog(object, length, 0, logStr);
			else
				glGetProgramInfoLog(object, length, 0, logStr);

			if(str) std::cout << str << " ";
			std::cout << logStr << std::endl;
			std::string ch = logStr;
			// To avoid a crash of the PC
			int i=ch.find("software");
			if(i>=0){
				std::cerr << "Unable to continue the program." << std::endl;
				exit(0);
			}
			delete[](logStr);
		}
	}

	inline GLuint ShaderManager::loadShader(std::filesystem::path const& vertexFile, std::filesystem::path const& fragmentFile, bool v) {
		GLuint programObject;
		if(v) std::cout << "Begin initializing shaders" << std::endl;
		CHECK_GL;

		// Create the shader program
		programObject = glCreateProgram();
		CHECK_GL;
		if(v) std::cout << "Initialization of the shader program : ";
		if(!programObject) {
			if(v) {
				std::cout << "[FAILED]" << std::endl;
				printLog(programObject,false,0);
				std::cout << "Shader will not be used" << std::endl;
			}
			return 0;
		}
		if(v) {
			std::cout << "[OK]" << std::endl;
			printLog(programObject,false,0);
		}
		CHECK_GL;

		// Compile the vertex shader
		if(!compileShader(vertexFile.string().c_str(), Vertex, programObject, v)) {
			if(v) std::cout << "Shader will not be used" << std::endl;
			return 0;
		}

		// Compile the fragment shader
		if(!compileShader(fragmentFile.string().c_str(), Fragment, programObject, v)) {
			if(v) std::cout << "Shader will not be used" << std::endl;
			return 0;
		}
		CHECK_GL;

		if(linkProgram(programObject, v)) {
			if(v) std::cout << "End of shader initialization" << std::endl;
			CHECK_GL;
			return programObject;
		}
		else {
			if(v) std::cout << "Shader will not be used" << std::endl;
			return 0;
		}
	}

	inline GLuint ShaderManager::loadShader(const std::vector<const char *> filenames, const std::vector<ShaderType> shaderTypes, bool v) {
		GLuint programObject;
		if(v) std::cout << "Begin initializing shaders" << std::endl;
		CHECK_GL;

		// Create the shader program
		programObject = glCreateProgram();
		CHECK_GL;
		if(v) std::cout << "Initialization of the shader program ";
		if(!programObject) {
			if(v) {
				std::cout << "[FAILED]" << std::endl;
				printLog(programObject,false,0);
				std::cout << "Shader will not be used" << std::endl;
			}
			return 0;
		}
		if(v) {
			std::cout << "[OK]" << std::endl;
			printLog(programObject,false,0);
		}

		// Compile all shaders
		for(unsigned int i=0;i<shaderTypes.size();i++) {
			if (!compileShader(filenames[i], shaderTypes[i], programObject, v)) {
				if(v) std::cout << "Shader will not be used" << std::endl;
				return 0;
			}
		}

		// Link program
		if(linkProgram(programObject, v)) {
			if(v) std::cout << "End of shader initialization" << std::endl;
			return programObject;
		}
		else {
			if(v) std::cout << "Shader will not be used" << std::endl;
			return 0;
		}
		CHECK_GL;
	}

	inline bool ShaderManager::linkProgram(GLuint programObject, bool verbose) {
		if(verbose) std::cout << "Program linkage ";

		int linked = 0; // Link status

		// Link the program
		CHECK_GL;
		glLinkProgram(programObject);
		// Get link status
		CHECK_GL;
		glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
		CHECK_GL;

		if(linked) {
			if(verbose){
				std::cout << "[OK]" << std::endl;
				printLog(programObject, false, 0);
			}
			return true;
		}

		if(verbose){
			std::cout << "[FAILED]" << std::endl;
			printLog(programObject, false, 0);
		}
		return false;
	}

	inline bool ShaderManager::compileShader(const char *shaderFile, const ShaderType shaderType, GLuint& programObject, bool verbose) {
		// Vertex shader
		GLchar *shaderSource;
		GLenum glShaderType = convertToGLShaderType(shaderType);
		if(shaderFile) {
			if(verbose) std::cout << writeShaderType(shaderType) << " shader : " << std::endl;
			if(!loadSource(shaderFile, &shaderSource)){
				if(verbose) std::cout << "Loading shader '" << shaderFile << "' [FAILED]" << std::endl;
				return false;
			}
			if(verbose) std::cout << "Loading shader '" << shaderFile << "' [OK]" << std::endl;

			// Compile shader

			// Create an object that will contain source
			GLuint shaderObject = glCreateShader(glShaderType);
			// Associate the source
			glShaderSource(shaderObject, 1, (const GLchar**)&shaderSource, 0);
			// Compile the source
			glCompileShader(shaderObject);

			// Get compilation status
			int compiled = 0;
			glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
			if(verbose) std::cout << "Compilation of shader ";

			if(compiled) {
				// Attach the compiled object to the program
				glAttachShader(programObject, shaderObject);
				if(verbose){
					std::cout << "[OK]" << std::endl;
					printLog(shaderObject,true,0);
				}
			}
			else {
				if(verbose){
					std::cout << "[FAILED]" << std::endl;
					printLog(shaderObject,true,0);
					delete shaderSource;
					return false;
				}
			}
			glDeleteShader(shaderObject);
			delete[](shaderSource);
		}
		return true;
	}

	inline bool ShaderManager::loadSource(const char* filename, char** source) {
		std::ifstream file(filename);
		if(!file){
			std::cout << "Unable to open file '" << filename << "'" << std::endl;
			assert(false && "Unable to open file");
			return false;
		}

		std::stringstream buffer; // Variable containing entire file
		// Copy entire file into the buffer
		buffer << file.rdbuf();
		// We no longer need the file!
		file.close();

		std::string s = buffer.str();

		*source = new char[ buffer.str().size() + 1];
		unsigned int i;
		for(i =0; i < buffer.str().size(); ++i){
			(*source)[i] = s[i];
		}
		(*source)[i] = '\0';
		return true;
	}

	inline void ShaderManager::deleteProgram(GLuint programObject) {
		// S'il existe on supprime le programme GLSL
		if(programObject) glDeleteShader(programObject);
	}

	inline bool ShaderManager::areShadersSupported(bool v = false) {
		if (v) {
			std::cout << "Check if shader extensions are supported:" << std::endl;
			std::cout << "Graphics card: " << (char *)glGetString(GL_RENDERER) << std::endl;
			std::cout << "Manufacturer: " << (char *)glGetString(GL_VENDOR) << std::endl;
			std::cout << "OpenGL version: " <<(char *)glGetString(GL_VERSION) << std::endl;
		}
		if(true/*GLEW_ARB_shading_language_100 || GLEW_ARB_shader_objects ||
		   GLEW_ARB_fragment_shader || GLEW_ARB_vertex_shader*/)
		{
			if (v) std::cout << "Shaders are supported" << std::endl;
			return true;
		}
		if (v) std::cout << "Shaders are not supported" << std::endl;
		return false;
	}

	inline std::string ShaderManager::writeShaderType(ShaderType shdtype) {
		switch(shdtype) {
			case(Vertex) : 
				return "Vertex";
			case(Fragment) : 
				return "Fragment";
			case(Geometry) : 
				return "Geometry";
			case(TesControl) : 
				return "Tesselation Control";
			case(TesEval) : 
				return "Tesselation Evaluation";
			default:
				return "UNKNOWN";
		}
	}

	inline GLenum ShaderManager::convertToGLShaderType(ShaderType shdtype) {
		switch(shdtype) {
			case(Vertex) : 
				return GL_VERTEX_SHADER;
			case(Fragment) : 
				return GL_FRAGMENT_SHADER;
			case(Geometry) : 
				return GL_GEOMETRY_SHADER;
#ifdef GL_VERSION_4_0
			case(TesControl) : 
				return GL_TESS_CONTROL_SHADER;
			case(TesEval) : 
				return GL_TESS_EVALUATION_SHADER;
#endif
			default:
				return 0;
		}
	}

};

#endif


