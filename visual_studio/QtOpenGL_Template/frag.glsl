#version 430
in vec3 normal;
out vec4 frag_color;
void main () {
	frag_color = vec4((normal+vec3(1,1,1))/2, 1.0);
	//frag_color = vec4(1.0,0,1.0,1.0);
}