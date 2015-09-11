#pragma once
#ifndef __OGLVIEWER__
#define __OGLVIEWER__

#include "GL/glew.h"

#include <QGLWidget>
//#include <QGLFunctions>

#include "OpenGL_Utils/GLSLProgram.h"
#include "Geometry/Mesh.h"

//vector<Shape*> objectList;
static Mesh *disp_geo;
// OpenGL Window in Qt
class OGLViewer : public QGLWidget
{
public:
	OGLViewer();
	~OGLViewer();
protected:
	void initializeGL();
	void paintGL();
};

#endif // __OGLVIEWER__