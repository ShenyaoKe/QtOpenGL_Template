#pragma once
#ifndef __OGLVIEWER__
#define __OGLVIEWER__

#include "GL/glew.h"

#include <QOpenGLWidget>
#include <QTimer>
#include <QTime>
//#include <QGLFunctions>

#include "OpenGL_Utils/GLSLProgram.h"
#include "Geometry/Mesh.h"
#include "Math/Matrix4D.h"

static Mesh *disp_geo;// Display object
static GLfloat* verts;// vertices vbo
static GLfloat* uvs;// Texture coordinates vbo
static GLfloat* norms;// Normal coordinates vbo
static int vbo_size;// Triangle face numbers
static GLSLProgram* shader;// OpenGL shader program
static Matrix4D matrix;// Transform matrix
static int mat_loc;// Uniform matrix location
static GLfloat mtx[12];// Uniform matrix buffer

// OpenGL Window in Qt
class OGLViewer : public QOpenGLWidget
{
public:
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent *e);
};

#endif // __OGLVIEWER__