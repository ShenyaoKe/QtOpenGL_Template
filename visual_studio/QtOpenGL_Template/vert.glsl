#version 450
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;
uniform mat4 matrix; // our matrix
out vec4 color;

void main () {
	color = vec4(vc,1.0);
	gl_Position = matrix * vec4(vp, 1.0);
}