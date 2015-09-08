#include "OGLViewer.h"


OGLViewer::OGLViewer()
{
}


OGLViewer::~OGLViewer()
{
}

void OGLViewer::initializeGL()
{
	glewInit();

	GLfloat verts[] = {
		+0.0f, +1.0f,
		-1.0f, -1.0f,
		+1.0f, -1.0f
	};

	GLuint bufferidx;
	glGenBuffers(1, &bufferidx);
	glBindBuffer(GL_ARRAY_BUFFER, bufferidx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

void OGLViewer::paintGL()
{
	glViewport(0, 0, width(), height());
	//glClearColor(0.2, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
