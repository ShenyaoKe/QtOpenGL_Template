#version 430
//uniform sampler2D modelTex;
in vec3 pos_eye, norm_eye;

uniform vec3 Kd = vec3(0.6, 0.8, 1);
uniform vec3 La = vec3(0, 0, 0); // grey ambient colour
// fixed point light properties
uniform vec3 light_world = vec3(0, 0, 0);

out vec4 frag_color; // final colour of surface

void main()
{
	if (gl_FrontFacing)
	{
		// diffuse intensity
		// raise light position to eye space
		vec3 dist_to_light = light_world - pos_eye;
		vec3 dir_to_light = normalize(dist_to_light);
		float dot_prod = dot(dir_to_light, norm_eye);
		dot_prod = (dot_prod + 1.0) / 2.0;
		vec3 Id = mix(La, Kd, dot_prod); // final diffuse intensity

		// final colour
		frag_color = vec4(Id, 1.0);
	}
	else
	{
		frag_color = vec4(0, 0, 1, 1.0);
	}
}

