#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;

uniform mat4 view_matrix, proj_matrix;

out vec3 pos_eye, norm_eye;

void main()
{
	//mat4 model_matrix;
	pos_eye = vec3(view_matrix * vec4(vp, 1.0));
	norm_eye = normalize(vec3(view_matrix * vec4(vn, 0.0)));
	gl_Position = proj_matrix * view_matrix * vec4(vp, 1.0);
}