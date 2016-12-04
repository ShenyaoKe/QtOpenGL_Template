#include "OGLViewer.h"
#include <QMatrix4x4>

OGLViewer::OGLViewer(QWidget *parent)
    : QOpenGLWidget(parent)
    , view_cam(new perspCamera(
        Point3f(10, 6, 10), Point3f(0.0, 0.0, 0.0), Vector3f(0, 1, 0),
        width() / Float(height())))
    , model_mesh(new TriangleMesh("../../scene/obj/unitCube.obj"))
{
    auto ctx = this->context();
    cout << "default fbo:\t" << defaultFramebufferObject() << endl;
    // Set surface format for current widget
    QSurfaceFormat format;
    format.setDepthBufferSize(32);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(format);
}

OGLViewer::~OGLViewer()
{
}
/************************************************************************/
/* OpenGL Rendering Modules                                             */
/************************************************************************/
void OGLViewer::initializeGL()
{
    // OpenGL extention initialization
    glewInit();
    // Print OpenGL vertion
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version supported " << glGetString(GL_VERSION) << endl;

    // Enable OpenGL features
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front
    glClearColor(0.6, 0.6, 0.6, 0.0);

    //////////////////////////////////////////////////////////////////////////
    // Create shader files
    model_shader.reset(new GLSLProgram("mesh_vs.glsl", "mesh_fs.glsl",
        nullptr, nullptr, nullptr, true, 1, feedbackVaryings.data()));

    // Export vbo for shaders
    model_mesh->exportVBO(&model_verts, &model_uvs, &model_norms);
    feedbackOutput.resize(model_verts.size());

    bindMesh();
}

void OGLViewer::bindMesh()
{
    glDeleteBuffers(1, &model_vert_vbo);
    glDeleteBuffers(1, &model_norm_vbo);
    glDeleteVertexArrays(1, &model_vao);

    // Bind VAO
    glGenVertexArrays(1, &model_vao);
    glBindVertexArray(model_vao);

    glGenBuffers(1, &model_vert_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model_vert_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * model_verts.size(),
                 &model_verts[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Bind normal value as color
    glGenBuffers(1, &model_norm_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model_norm_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * model_norms.size(),
                 &model_norms[0],
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * model_verts.size(),
                 nullptr,
                 GL_STATIC_READ);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLViewer::paintGL()
{
    // Make curent window
    makeCurrent();

    glDisable(GL_MULTISAMPLE);
    // Clear background and color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //////////////////////////////////////////////////////////////////////////
    // Model
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // cull back face
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glBindVertexArray(model_vao);
    model_shader->use_program();

    // Apply uniform matrix
    glUniformMatrix4fv((*model_shader)["view_matrix"], 1, GL_FALSE, view_cam->world_to_cam());
    glUniformMatrix4fv((*model_shader)["proj_matrix"], 1, GL_FALSE, view_cam->cam_to_screen());

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    glBeginTransformFeedback(GL_TRIANGLES);
    glDrawArrays(GL_TRIANGLES, 0, model_verts.size() / 3);
    glEndTransformFeedback();

    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER,
                       0,
                       sizeof(float) * feedbackOutput.size(),
                       feedbackOutput.data());
    
    FILE* fp = fopen("newObj.obj", "w");
    for (int i = 0; i < feedbackOutput.size(); i+=3)
    {
        fprintf(fp, "v %f %f %f\n", feedbackOutput[i], feedbackOutput[i+1], feedbackOutput[i+2]);
    }
    for (int i = 0; i < feedbackOutput.size() / 3; i += 3)
    {
        fprintf(fp, "f %d %d %d\n", i + 1, i + 2, i + 3);
    }
    fclose(fp);

    glBindVertexArray(0);
}

// Resize function
void OGLViewer::resizeGL(int w, int h)
{
    // Widget resize operations
    view_cam->resizeViewport(width() / static_cast<double>(height()));
}
/************************************************************************/
/* Qt User Operation Functions                                          */
/************************************************************************/
void OGLViewer::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_W)
    {
        view_cam->zoom(0.0, 0.0, 0.10);
    }
    else if (e->key() == Qt::Key_S)
    {
        view_cam->zoom(0.0, 0.0, -0.10);
    }
    else if (e->key() == Qt::Key_Q)
    {
        view_cam->zoom(0.10, 0.0, 0.0);
    }
    else if (e->key() == Qt::Key_A)
    {
        view_cam->zoom(-0.10, 0.0, 0.0);
    }
    else if (e->key() == Qt::Key_E)
    {
        view_cam->zoom(0.0, 0.10, 0.0);
    }
    else if (e->key() == Qt::Key_D)
    {
        view_cam->zoom(0.0, -0.10, 0.0);
    }
    else if (e->key() == Qt::Key_Home)
    {
        initParas();
    }
    // Save frame buffer
    else if (e->key() == Qt::Key_P && e->modifiers() == Qt::ControlModifier)
    {
        this->saveFrameBuffer();
    }
    //////////////////////////////////////////////////////////////////////////
    else
    {
        QOpenGLWidget::keyPressEvent(e);
    }
    update();
}

void OGLViewer::mousePressEvent(QMouseEvent *e)
{
    m_lastMousePos[0] = e->x();
    m_lastMousePos[1] = e->y();
    if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
    {
        // Do something here
    }
};

void OGLViewer::mouseReleaseEvent(QMouseEvent *e)
{
    m_lastMousePos[0] = e->x();
    m_lastMousePos[1] = e->y();
}

void OGLViewer::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->x() - m_lastMousePos[0];
    int dy = e->y() - m_lastMousePos[1];

    //printf("dx: %d, dy: %d\n", dx, dy);

    if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
    {
        view_cam->rotate(dy * 0.25, -dx * 0.25, 0.0);
        update();
    }
    else if ((e->buttons() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier))
    {
        if (dx != e->x() && dy != e->y())
        {
            view_cam->zoom(0.0, 0.0, dx * 0.05);
            update();
        }
    }
    else if ((e->buttons() == Qt::MidButton) && (e->modifiers() == Qt::AltModifier))
    {
        if (dx != e->x() && dy != e->y())
        {
            view_cam->zoom(-dx * 0.05, dy * 0.05, 0.0);
            update();
        }
    }
    else
    {
        QOpenGLWidget::mouseMoveEvent(e);
    }

    m_lastMousePos[0] = e->x();
    m_lastMousePos[1] = e->y();
}
/************************************************************************/
/* Application Functions                                                */
/************************************************************************/
void OGLViewer::resetCamera()
{
    view_cam.reset(new perspCamera(
        Point3f(10, 6, 10), Point3f(0, 0, 0), Vector3f(0, 1, 0),
        width() / static_cast<Float>(height())));
}
void OGLViewer::initParas()
{
    update();
}

void OGLViewer::saveFrameBuffer()
{
    QString filename = QFileDialog::getSaveFileName(
        this, "Save Screenshot file...", "default", tr("PNG(*.png)"));
    this->grab().save(filename);
}