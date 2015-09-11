#version 450
in vec4 color;
out vec4 frag_color;
void main () {
	frag_color = color;
	//frag_color = vec4(1.0,0,1.0,1.0);
}