#include "glbasimac/glbi_engine.hpp"
#include "tools/shaders.hpp"
#include "exe_path/exe_path.h"
using namespace glbasimac;
using namespace STP3D;

namespace glbasimac {

	void GLBI_Engine::initGL() {
		std::cout<<"Initialisation of GL Engine"<<std::endl;

		if (mode2D) {
			std::cerr<<"Flat 2D"<<std::endl;
			idShader[0] = ShaderManager::loadShader(exe_path::dir() / "assets/shaders/flat_shading_2D.vert",exe_path::dir() / "assets/shaders/flat_shading.frag",true);
		}
		else {
			std::cerr<<"Flat 3D"<<std::endl;
			idShader[0] = ShaderManager::loadShader(exe_path::dir() / "assets/shaders/flat_shading_3D.vert",exe_path::dir() / "assets/shaders/flat_shading.frag",true);
			std::cerr<<"Phong 3D"<<std::endl;
			idShader[1] = ShaderManager::loadShader(exe_path::dir() / "assets/shaders/phong_shading.vert",exe_path::dir() / "assets/shaders/phong_shading.frag",true);
		}
		mvMatrixStack.loadIdentity();
		glUseProgram(idShader[0]);
		if (!mode2D) {
			glUniform1i(glGetUniformLocation(idShader[0],"use_texture"),useTexture);
			glUseProgram(idShader[1]);
			glUniform3fv(glGetUniformLocation(idShader[1],"attenuationFactor"),1,attFactors);
			glUniform1f(glGetUniformLocation(idShader[1],"shininess"),0.0);
			glUniform1i(glGetUniformLocation(idShader[1],"numOfLight"),numberOfLight);
			glUniform1i(glGetUniformLocation(idShader[1],"use_texture"),useTexture);
			glUseProgram(idShader[0]);
		}
		else {
			glUniformMatrix4fv(glGetUniformLocation(idShader[currentShader],"modelviewMat"),1,GL_FALSE,mvMatrixStack.getTopGLMatrix());
		}
	}

	void GLBI_Engine::setFlatColor(float r,float g,float b) {
		glVertexAttrib3f(glGetAttribLocation(idShader[currentShader],"vx_col"),r,g,b);
	}

	void GLBI_Engine::updateMvMatrix() {
		glUniformMatrix4fv(glGetUniformLocation(idShader[currentShader],"modelviewMat"),1,GL_FALSE,mvMatrixStack.getTopGLMatrix());
		if (!mode2D) {
			Matrix4D nmlMatrix = mvMatrixStack.getTopGLMatrix();
			nmlMatrix.invert();
			nmlMatrix.transpose();
			glUniformMatrix4fv(glGetUniformLocation(idShader[currentShader],"normalMat"),1,GL_FALSE,nmlMatrix);
		}
	}

	void GLBI_Engine::set2DProjection(float xmin,float xmax,float ymin,float ymax) {
		Matrix4D proj = Matrix4D::ortho2D(xmin,xmax,ymin,ymax);
		glUniformMatrix4fv(glGetUniformLocation(idShader[currentShader],"projectionMat"),1,GL_FALSE,proj);
	}

	void GLBI_Engine::set3DProjection(float fov,float ratio,float z_near,float z_far) {
		Matrix4D proj = Matrix4D::perspective(fov,ratio,z_near,z_far);
		glUseProgram(idShader[0]);
		glUniformMatrix4fv(glGetUniformLocation(idShader[0],"projectionMat"),1,GL_FALSE,proj);
		if (!mode2D) {
			glUseProgram(idShader[1]);
			glUniformMatrix4fv(glGetUniformLocation(idShader[1],"projectionMat"),1,GL_FALSE,proj);
			glUseProgram(idShader[currentShader]);
		}
	}

	void GLBI_Engine::setViewMatrix(const Matrix4D& mat) {
		viewMatrix = mat;
		// Mettre le uniform dans le shader correspondant.
		if (!mode2D) {
			glUseProgram(idShader[1]);
			glUniformMatrix4fv(glGetUniformLocation(idShader[1],"viewMatrix"),1,GL_FALSE,viewMatrix);
			glUseProgram(idShader[currentShader]);
		}
		mvMatrixStack.addTransformation(mat);
	}

	void GLBI_Engine::activateTexturing(bool use_texture) {
		useTexture = use_texture;
		glActiveTexture(GL_TEXTURE0);
		if (!mode2D) {
			glUniform1i(glGetUniformLocation(idShader[currentShader],"tex0"),0);
			glUniform1i(glGetUniformLocation(idShader[currentShader],"use_texture"),useTexture);
		}
		else {
			std::cerr<<"Unable to use texturing in 2D mode"<<std::endl;
		}
	}

	void GLBI_Engine::switchToFlatShading() {
		currentShader = 0;
		glUseProgram(idShader[0]);
	}

	void GLBI_Engine::switchToPhongShading() {
		if (mode2D) {
			std::cerr<<"Unable to switch to Phong Shading in 2D mode"<<std::endl;
		}
		else {
			currentShader = 1;
			glUseProgram(idShader[1]);
		}
	}

	void GLBI_Engine::setLightPosition(const Vector4D& light_pos,int num_light) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set light position in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			if (num_light<numberOfLight) {
				lightPos[num_light] = light_pos;
				glUniform4fv(glGetUniformLocation(idShader[currentShader],"lightPos"),numberOfLight,&lightPos[0][0]);
			}
		}
	}

	void GLBI_Engine::setLightIntensity(const Vector3D& light_intensity,int num_light) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set light position in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			if (num_light<numberOfLight) {
				lightIntensity[num_light] = light_intensity;
				glUniform3fv(glGetUniformLocation(idShader[currentShader],"lightIntensity"),numberOfLight,&lightIntensity[0][0]);
			}
		}
	}

	void GLBI_Engine::setNormalForConvex2DShape(const Vector3D& nml) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set light position in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			glVertexAttrib3f(glGetAttribLocation(idShader[currentShader],"vx_nml"),nml.x,nml.y,nml.z);
		}
	}

	void GLBI_Engine::setAttenuationFactor(const Vector3D& factors) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set light position in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			attFactors = factors;
			glUniform3fv(glGetUniformLocation(idShader[1],"attenuationFactor"),1,attFactors);
		}
	}

	void GLBI_Engine::addALight(const Vector4D& light_pos,const Vector3D& light_intensity) {
		if (mode2D) {
			std::cerr<<"Unable to add light in 2D mode"<<std::endl;
		}
		else {
			numberOfLight++;
			lightPos.push_back(light_pos);
			lightIntensity.push_back(light_intensity);
			glUseProgram(idShader[1]);
			glUniform1i(glGetUniformLocation(idShader[1],"numOfLight"),numberOfLight);
			glUseProgram(idShader[currentShader]);
		}
	}

	void GLBI_Engine::setShininess(float new_shininess) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set shininess in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			glUniform1f(glGetUniformLocation(idShader[1],"shininess"),new_shininess);
		}
	}

	void GLBI_Engine::setSpecularColor(const Vector3D& c_spec) {
		if (mode2D || currentShader == 0) {
			std::cerr<<"Unable to set shininess in 2D mode or in Flat shading"<<std::endl;
		}
		else {
			glUniform3fv(glGetUniformLocation(idShader[1],"c_spec"),1,c_spec.val);
		}
	}


}
