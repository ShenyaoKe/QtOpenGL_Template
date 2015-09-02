#pragma once

#include "GL/glew.h"

#include <QGLWidget>
//#include <QGLFunctions>

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

