#pragma once
#ifndef __OGLVIEWER__
#define __OGLVIEWER__

#include "GL/glew.h"
#include "common.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QTime>
#include <QString>
#include <QFileDialog>
#include <QOpenGLVertexArrayObject>
//#include <QGLFunctions>

#include "OpenGL_Utils/GLSLProgram.h"

#include "Math/MathUtil.h"
#include "Image/ImageData.h"
#include "Geometry/TriangleMesh.h"
#include "Camera/perspCamera.h"

// OpenGL Window in Qt
class OGLViewer : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLViewer(QWidget *parent = nullptr);
    ~OGLViewer();

    void resetCamera();
    void initParas();
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private:
    void bindMesh();
    void saveFrameBuffer();
public:
protected:
    unique_ptr<perspCamera> view_cam;
private:
    int m_lastMousePos[2];
private: // OpenGL variables

    unique_ptr<TriangleMesh> model_mesh;
    vector<GLfloat> model_verts;// vertices vbo
    vector<GLfloat> model_uvs;// Texture coordinates vbo
    vector<GLfloat> model_norms;// Normal coordinates vbo
    GLuint model_vert_vbo, model_norm_vbo, model_vao;


    GLuint tbo;
    vector<float> feedbackOutput;
    vector<const char*> feedbackVaryings = { "pos_eye" };

    unique_ptr<GLSLProgram> model_shader;// OpenGL shader program

    friend class MainWindow;
};

#endif // __OGLVIEWER__