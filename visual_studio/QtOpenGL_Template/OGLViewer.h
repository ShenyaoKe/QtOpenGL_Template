#pragma once

#include "GL/glew.h"
#include "common.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QFileDialog>

#include "OpenGL_Utils/GLSLProgram.h"

#include "Math/MathUtil.h"
#include "Image/ImageData.h"
#include "Geometry/TriangleMesh.h"
#include "Camera/PerspectiveCamera.h"

// OpenGL Window in Qt
class OGLViewer : public QOpenGLWidget
{
	Q_OBJECT
public:
	enum Select_Mode
	{
		OBJECT_SELECT,
		COMPONENT_SELECT,
		FACE_COMPONENT_SELECT
	};
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

	void resetCamera();
protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void keyPressEvent(QKeyEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
private:
    GLuint createRenderObject(const Kaguya::RenderBufferTrait &trait);
	void saveFrameBuffer();
public:
protected:
private:
    float mProcessFps;
	int mFps;
	int mTimeCount;

	QTime mProcTime;
	int mLastMousePos[2];
	int mSelectMode;
    
    // OpenGL variables
	int mDisplayMode = 0;

	shared_ptr<Kaguya::TriangleMesh> box_mesh;// Display object
    Kaguya::RenderBufferTrait boxTrait;
	GLuint box_vao;

    shared_ptr<Kaguya::TriangleMesh> model_mesh;
    Kaguya::RenderBufferTrait modelTrait;
	GLuint model_vao;

	vector<GLfloat> filmgate, resgate;

    Kaguya::Matrix4x4 cam = Kaguya::Matrix4x4::lookAt(
        Kaguya::Point3f(0,0,-5), Kaguya::Point3f(0,0,0), Kaguya::Vector3f(0,1,0));
    Kaguya::Matrix4x4 ortho = Kaguya::Matrix4x4::orthography(-10,10,-10,10,-10,50);

	unique_ptr<GLSLProgram> model_shader;// OpenGL shader program

	unique_ptr<Kaguya::PerspectiveCamera> mViewCamera;

	friend class MainWindow;
};
