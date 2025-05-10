#version 410 core 

#define M_PI 3.1415926535897932384626433832795
const int Nmax = 255;

in vec3 color; // Couleur flat du point de l'objet (si existe)
in vec2 uvs;   // Coordonnees de texture du point de l'object (dans le repere camera)
in vec3 nml;   // Normale du point de l'object (dans le repere camera)
in vec3 pos;   // Position dans le repere camera

uniform sampler2D tex0;
uniform int use_texture; // 0 if not. 1 else

uniform vec3 c_spec;
uniform float shininess;

uniform vec3 attenuationFactor;
uniform int numOfLight;
uniform vec4 lightPos[6];
uniform vec3 lightIntensity[6];
uniform mat4 viewMatrix;

layout(location = 0) out vec4 final_col;


vec4 objTexture() {
	return texture(tex0,uvs);
}

float saturate(float val) {
	if (val<0.0) return 0.0;
	return val;
}

vec4 lambert(int idLight) {
	// Normal normalization
	vec3 nml_cam = normalize(nml);

	// Computing vector PL and setting lambert term
	vec3 dir_illu;
	if (lightPos[idLight].w > 0.0) {
		vec3 ptlight = vec3(viewMatrix*vec4(lightPos[idLight].xyz,1.0f));
		dir_illu = ptlight - pos;
	}
	else {
		dir_illu = vec3(viewMatrix*lightPos[idLight]);
	}
	float dist = length(dir_illu);
	vec3 dir_illu_nml = normalize(dir_illu);
	float cos_illu = saturate(dot(dir_illu_nml,nml_cam));

	vec3 L = lightIntensity[idLight];
	float attenuation;
	if (lightPos[idLight].w > 0.0) {
		attenuation = 1.0f/(attenuationFactor.x+attenuationFactor.y*dist+attenuationFactor.z*dist*dist);
	}
	else {
		attenuation = attenuationFactor.x;
	}

	// Shininess
	vec3 view_dir = normalize(-pos);
	vec3 halfVector = normalize(view_dir + dir_illu_nml);
	float spec_intensity = 0.0;
	if (shininess>0.0) {
		spec_intensity = pow(saturate(dot(nml_cam,halfVector)),shininess);
	}
	//spec_intensity = saturate(dot(nml,halfVector));

	// Final color computation
	vec4 c_dif;
	if (use_texture == 1) {
		c_dif  = texture(tex0,uvs);
	}
	else {
		c_dif = vec4(color,1.0f);
	}
	return vec4(c_dif.rgb*(L*attenuation)*cos_illu + spec_intensity*(L*attenuation)*c_spec,1.0);
}

void main()
{
	final_col = vec4(0.0,0.0,0.0,1.0);
	for(int i=0;i<numOfLight;i++) {
		final_col += lambert(i);
	}
}
