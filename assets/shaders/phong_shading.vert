#version 410 core 

layout(location=0) in vec3 vx_pos; // Coordonnee du sommet
layout(location=1) in vec3 vx_nml; // Normale du sommet
layout(location=2) in vec2 vx_uvs; // Coordonnee de texture du sommet
layout(location=3) in vec3 vx_col; // Couleur du sommet (ou couleur de l'objet)

uniform mat4 projectionMat;
uniform mat4 modelviewMat;
uniform mat4 normalMat;

out vec3 color;
out vec2 uvs;
out vec3 nml;
out vec3 pos;

void main()
{
	gl_Position = projectionMat*modelviewMat*vec4(vx_pos,1.0);
	uvs = vx_uvs;
	color = vx_col;
	nml = vec3(normalMat*vec4(vx_nml,0.0));	
	vec4 pos_t = modelviewMat*vec4(vx_pos,1.0);
	pos = pos_t.xyz/pos_t.w;
}
