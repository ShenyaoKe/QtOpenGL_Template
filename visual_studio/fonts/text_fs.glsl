#version 410

// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
uniform sampler2D tex;

// output fragment color RGBA
out vec4 frag_color;

void main () {
	// invert color of right-hand side
	float alpha = float(texture(tex, st).r) / 255.0f;
	if (st.s >= 0.5f) {
		alpha = 1.0f - alpha;
	}
	frag_color = vec4(alpha, 0, 0, alpha);//*/
	//frag_color = vec4 (1.f, 0.f, 0.f, 1.0);
}
