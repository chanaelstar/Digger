#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "tools/matrix4d.hpp"
#include "tools/matrix_stack.hpp"

using namespace STP3D;

namespace glbasimac {

struct GLBI_Engine {
	GLBI_Engine():mode2D(true),useTexture(0),currentShader(0),attFactors({1.0,0.0,1.0}),numberOfLight(1) {
		lightPos.push_back({0.0,0.0,0.0,0.0});
		lightIntensity.push_back({0.0,0.0,0.0});
	}

	~GLBI_Engine() {}

	/// Set the OpenGL Engine
	void initGL();
	/// Set 2D orthographic projection. Resulting virtual screen size is [xmin,ymin][xmax,ymax]
	void set2DProjection(float xmin,float xmax,float ymin,float ymax);
	/// Set 3D perspective projection with a \param fov and \param z_near / \param \z_far depth range
	void set3DProjection(float fov,float ratio,float z_near,float z_far);
	/// Set the current flat color to r,g,b. This color will remains until changed
	void setFlatColor(float r,float g,float b);
	/// Set the current flat color to r,g,b. This color will remains until changed
	void setViewMatrix(const Matrix4D& mat);
	/// Send current transformation to GL Engine. ids is the id of the shader to set.
	void updateMvMatrix();
	
	/// In 3D configuration, activate or desactivate texturing.
	void activateTexturing(bool use_texture);
	/// Switch shader to "flat shading".
	void switchToFlatShading();
	/// Switch shader to "phong shading".
	void switchToPhongShading();
	/// Setting light position for light number num_light
	void setLightPosition(const Vector4D& light_pos,int num_light=0);
	/// Setting light intensity for light number num_light
	void setLightIntensity(const Vector3D& light_intensity,int num_light=0);
	/// Setting light intensity for light number num_light
	void addALight(const Vector4D& light_pos,const Vector3D& light_intensity);
	/// Set the normal of future convex 2D shape to draw
	void setNormalForConvex2DShape(const Vector3D& nml);
	/// Set attenuation factor (for all lights)
	void setAttenuationFactor(const Vector3D& factors);
	/// Set shininess (for future rendered object)
	void setShininess(float new_shininess);
	/// Set specular coefficient (for future rendered object)
	void setSpecularColor(const Vector3D& c_spec);

	/// GL parameters
	unsigned int idShader[3];
	MatrixStack mvMatrixStack;
	Matrix4D viewMatrix;
	bool mode2D;
	int useTexture; // 0 do not use texture. Else number of texture to use (TODO, 1 for the moment)
	int currentShader;

	/// Light parameters
	Vector3D attFactors;
	std::vector<Vector4D> lightPos;
	std::vector<Vector3D> lightIntensity;
	int numberOfLight;
};

}
