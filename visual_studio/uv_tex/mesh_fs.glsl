#version 430
//uniform sampler2D modelTex;
uniform mat4 view_matrix;
uniform sampler2D myTextureSampler;
in vec3 normal;
in vec2 texST;  // From the geometry shader
in vec3 pos_view, normal_eye;

out vec4 frag_color; // final colour of surface

void main()
{
	if (gl_FrontFacing)
	{
        vec3 Kd = texture(myTextureSampler, texST).xyz;//vec3(0.6, 0.8, 1);
        vec3 La = vec3(0, 0, 0); // grey ambient colour
        // fixed point light properties
        vec3 light_position_world = vec3(0.0, 100.0, 2.0);
        
        // ambient intensity
		// diffuse intensity
		// raise light position to eye space
		vec3 light_pos_view = light_position_world;// vec3(view_matrix * vec4(light_position_world, 1.0));
		vec3 distance_to_light_eye = light_pos_view - pos_view;
		vec3 direction_to_light_eye = normalize(distance_to_light_eye);
		float dot_prod = dot(direction_to_light_eye, normal_eye);
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

