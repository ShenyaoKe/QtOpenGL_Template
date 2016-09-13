#version 410

// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
uniform sampler2D tex;

// output fragment color RGBA
out vec4 frag_color;

void main () {
	// invert color of right-hand side
	float alpha = texture(tex, st).r;
	if (st.s >= 0.5f) {
		alpha = 1.0f - alpha;
	}
	frag_color = vec4(alpha);
}
