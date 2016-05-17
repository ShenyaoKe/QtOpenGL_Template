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

	bool create(int32_t width, int32_t height, GLenum attachment,
		GLenum target = GL_TEXTURE_2D, GLenum internalFormat = 0);
	void bind();
	void unbind(GLuint default_fbo = 0);
	void bindTexture();

	GLuint texture() const;

	static bool setDefault(GLint fbo);
	static bool bindDefault();
private:
	GLuint fb, tex, rb;

	static GLuint defaultFrameBuffer;
};
