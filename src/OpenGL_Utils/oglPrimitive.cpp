#include "oglPrimitive.h"



oglPrimitive::oglPrimitive()
{
}


oglPrimitive::~oglPrimitive()
{
}


oglUnitQuad::oglUnitQuad()
	: vao(0), vbo(0)
{
}

oglUnitQuad::~oglUnitQuad()
{
}

void oglUnitQuad::create()
{
	GLfloat quad_pos[] = {
		-1.f, -1.f,
		1.f, -1.f,
		1.f,  1.f,
		1.f,  1.f,
		-1.f,  1.f,
		-1.f, -1.f
	};
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_pos), quad_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void oglUnitQuad::destroy()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void oglUnitQuad::bind()
{
	glBindVertexArray(vao);
}

void oglUnitQuad::unbind()
{
	glBindVertexArray(0);
}

void oglUnitQuad::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
