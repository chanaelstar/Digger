#version 410 core

layout(location=0) in vec2 vx_pos; // Indice 0
layout(location=2) in vec2 vx_uvs; // Indice 2
layout(location=3) in vec3 vx_col; // Indice 3

uniform mat4 projectionMat;
uniform mat4 modelviewMat;

out vec3 color;
out vec2 uvs;

void main()
{
	gl_Position = projectionMat*modelviewMat*vec4(vx_pos.xy,0.0,1.0);

	color = vx_col;
	uvs = vx_uvs;

}
