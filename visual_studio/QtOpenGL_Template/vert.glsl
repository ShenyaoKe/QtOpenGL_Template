#version 430
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vn;
/*uniform mat4 model_matrix; // ModelToWorld matrix
uniform mat4 view_matrix; // WorldToCamera matrix
uniform mat4 proj_matrix; // Projection matrix
*/
uniform mat4 model_matrix, view_matrix, proj_matrix;
out vec3 normal;

void main () {
	normal = vn;
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vp, 1.0);
}