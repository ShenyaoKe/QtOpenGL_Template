#pragma once
#ifndef __OGLVIEWER__
#define __OGLVIEWER__

#include "GL/glew.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
//#include <QGLFunctions>

#include "OpenGL_Utils/GLSLProgram.h"
#include "Math/MathUtil.h"
#include "Geometry/Mesh.h"
//#include "Math/Matrix4D.h"
#include "Camera/Camera.h"
#include "Accel/KdTreeAccel.h"

static Mesh *sphere_mesh;
static GLfloat* sphere_verts;// vertices vbo
static GLfloat* sphere_uvs;// Texture coordinates vbo
static GLfloat* sphere_norms;// Normal coordinates vbo
static int sphere_vbo_size;// Triangle face numbers
static Matrix4D sphere_matrix;
static GLfloat sphere_model_mat[16];

static Mesh *box_mesh;// Display object
static GLfloat* box_verts;// vertices vbo
static GLfloat* box_uvs;// Texture coordinates vbo
static GLfloat* box_norms;// Normal coordinates vbo
static int box_vbo_size;// Triangle face numbers
static GLSLProgram* shader;// OpenGL shader program
static GLSLProgram* shader_transparent;// OpenGL shader program
static Matrix4D matrix;// Transform matrix

static int model_mat_loc;// Uniform matrix location
static GLfloat model_mat[16];// Uniform matrix buffer
static int view_mat_loc;// Uniform matrix location
static GLfloat view_mat[16];
static int proj_mat_loc;// Porjection matrix location
static GLfloat proj_mat[16];
//////////////////////////////////////////////////////////////////////////
// Acceleration
//////////////////////////////////////////////////////////////////////////
static vector<Shape*> triangleList;
static KdTreeAccel *mytree;

//////////////////////////////////////////////////////////////////////////
// Sphere Attribute
//////////////////////////////////////////////////////////////////////////

const double eps = 5e-4;


// OpenGL Window in Qt
class OGLViewer : public QOpenGLWidget
{
	Q_OBJECT
public:
	//OGLViewer();
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

	//void update();
public slots:
	void resetCamera();
	void initParas();
protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
public:
	double process_fps;
protected:
	perspCamera *view_cam;
private:
	int fps;
	int tcount;

	QTime process_time;
	int m_lastMousePos[2];
private:
	int display_mode = 0;

	friend class MainWindow;
};

#endif // __OGLVIEWER__