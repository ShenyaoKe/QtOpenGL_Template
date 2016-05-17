#include "oglFBO.h"

GLuint oglFBO::defaultFrameBuffer = 0;

oglFBO::oglFBO()
{
}


oglFBO::~oglFBO()
{
}

bool oglFBO::create(int32_t width, int32_t height, GLenum attachment,
	GLenum target, GLenum internalFormat)
{
	glGenFramebuffers(1, &fb);
	glGenTextures(1, &tex);
	glBindTexture(target, tex);
	glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, tex, 0);

	// Create a renderbuffer, which allows depth-testing in framebuffer
	glGenRenderbuffers(1, &rb);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// Attach renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rb);
	
	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufs);

	// Check framebuffer
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		fprintf(stderr, "ERROR: incomplete framebuffer\n");
		if (GL_FRAMEBUFFER_UNDEFINED == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_UNDEFINED\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		}
		else if (GL_FRAMEBUFFER_UNSUPPORTED == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_UNSUPPORTED\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
		}
		else if (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS == status) {
			fprintf(stderr, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
		}
		else {
			fprintf(stderr, "unspecified error\n");
		}
		return false;
	}
	// Unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
	return true;
}

void oglFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void oglFBO::unbind(GLuint default_fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, default_fbo);
}

void oglFBO::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, tex);
}

GLuint oglFBO::texture() const
{
	return tex;
}

bool oglFBO::setDefault(GLint fbo)
{
	if (fbo < 0)
	{
		return false;
	}
	defaultFrameBuffer = fbo;
	return true;
}

bool oglFBO::bindDefault()
{
	if (defaultFrameBuffer < 0)
	{
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
	return true;
}
