#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
layout(location = 2) in vec2 uv;

uniform mat4 view_matrix, proj_matrix;
out vec3 normal;
out vec2 texST;

out vec3 pos_view, normal_eye;

void main()
{
	normal = vn;
	texST = vec2(uv.x, 1-uv.y);
	pos_view = (view_matrix * vec4(vp, 1.0)).xyz;
	normal_eye = normalize(vec3(view_matrix * vec4(vn, 0.0)));
	gl_Position = proj_matrix * view_matrix * vec4(vp, 1.0);
}