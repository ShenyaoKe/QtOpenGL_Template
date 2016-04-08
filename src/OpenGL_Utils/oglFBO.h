#pragma once

#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

class oglFBO
{
public:
	oglFBO();
	~oglFBO();

	void resize(int32_t w, int32_t h);
	bool init();
	void bind();
	void unbind();
	void bindTexture();
public:
	int32_t width, height;
	GLuint fb, tex, rb;
};

