#include "oglFBO.h"



oglFBO::oglFBO()
{
}


oglFBO::~oglFBO()
{
}

void oglFBO::resize(int32_t w, int32_t h)
{
	width = w;
	height = h;
}

bool oglFBO::init()
{
	glGenFramebuffers(1, &fb);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Attach texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	// Create a renderbuffer, which allows depth-testing in framebuffer
	glGenRenderbuffers(1, &rb);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// Attach renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);

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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void oglFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void oglFBO::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void oglFBO::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, tex);
}
