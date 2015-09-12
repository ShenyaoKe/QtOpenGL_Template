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
//vector<Shape*> objectList;
static Mesh *disp_geo;
static GLfloat* verts;
static GLfloat* uvs;
static GLfloat* norms;
static int vbo_size;
static GLSLProgram* shader;
static Matrix4D matrix = setTranslation(0.5, 0, 0);
static int mat_loc;
static GLfloat mtx[12];

// OpenGL Window in Qt
class OGLViewer : public QOpenGLWidget
{
public:
	//OGLViewer();
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

	//void update();
protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void paintEvent(QPaintEvent *e);
private:

private slots:
	void movement();
private:
	int fps;
	int tcount;
	QTime process_time;
};

#endif // __OGLVIEWER__