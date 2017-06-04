#include "OGLViewer.h"
#include <QMatrix4x4>
#include <Kaguya/IO/ObjLoader.h>

using namespace Kaguya;

OGLViewer::OGLViewer(QWidget *parent)
	: QOpenGLWidget(parent), mTimeCount(0), mFps(30)
	, mSelectMode(OBJECT_SELECT)
	, mViewCamera(new Kaguya::PerspectiveCamera(
		Kaguya::Point3f(10, 6, 11),
		Kaguya::Point3f(0, 0, 0),
		Kaguya::Vector3f(0, 1, 0),
		width() / float(height())))
	, box_mesh(ObjLoader::loadTriangleMesh("../../scene/obj/cube_large.obj"))
	, model_mesh(ObjLoader::loadTriangleMesh("../../scene/obj/monkey.obj"))
{
	// Set surface format for current widget
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	format.setSamples(4);
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);

	// Link timer trigger
	/*process_time.start();
	QTimer *timer = new QTimer(this);
	//timer->setSingleShot(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(0);*/
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
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front

	glClearColor(0.6, 0.6, 0.6, 0.0);

	//////////////////////////////////////////////////////////////////////////

	// Create shader files
	model_shader.reset(new GLSLProgram("mesh_vs.glsl", "mesh_fs.glsl"));

	// Export vbo for shaders

	model_mesh->getRenderBuffer(&modelTrait);
	model_vao = createRenderObject(modelTrait);

	box_mesh->getRenderBuffer(&boxTrait);
	box_vao = createRenderObject(boxTrait);
}

GLuint OGLViewer::createRenderObject(const RenderBufferTrait &trait)
{
	GLuint vbo, ibo, vao;

	// VBO
	glCreateBuffers(1, &vbo);
	glNamedBufferData(vbo, trait.vertex.size, trait.vertex.data, GL_STATIC_DRAW);
	// IBO
	glCreateBuffers(1, &ibo);
	glNamedBufferData(ibo, trait.index.size, trait.index.data, GL_STATIC_DRAW);
	//indexCount = trait.index.count;

	// Bind VAO
	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, 0);

	// Attach VBO and IBO to VAO
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, trait.vertex.offset, trait.vertex.stride);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayElementBuffer(vao, ibo);

	return vao;
}

void OGLViewer::paintGL()
{
	// Make curent window
	makeCurrent();

	glDisable(GL_MULTISAMPLE);
	// Clear background and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mDisplayMode == 0)// Wireframe
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT); // cull back face
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// Bind Box VAOs
	glBindVertexArray(box_vao);
	// Use shader program
	model_shader->use_program();
	// Apply uniform matrix
	//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat);
	glUniformMatrix4fv((*model_shader)["view_matrix"], 1, GL_FALSE, cam.data());// mViewCamera->world_to_cam());
	glUniformMatrix4fv((*model_shader)["proj_matrix"], 1, GL_FALSE, ortho.data());//mViewCamera->cam_to_screen());
	glDrawElements(GL_TRIANGLES, boxTrait.index.count, GL_UNSIGNED_INT, 0);

	//////////////////////////////////////////////////////////////////////////
	// Model
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(model_vao);
	model_shader->use_program();

	// Apply uniform matrix
	//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat);
	glUniformMatrix4fv((*model_shader)["view_matrix"], 1, GL_FALSE, cam.data());//mViewCamera->world_to_cam());
	glUniformMatrix4fv((*model_shader)["proj_matrix"], 1, GL_FALSE, ortho.data());//mViewCamera->cam_to_screen());
	glDrawElements(GL_TRIANGLES, modelTrait.index.count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
// Redraw function

// Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	mViewCamera->resizeViewport(width() / Float(height()));
}
/************************************************************************/
/* Qt User Operation Functions                                          */
/************************************************************************/
void OGLViewer::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_W)
	{
		mViewCamera->zoom(0.0, 0.0, 0.10);
	}
	else if (e->key() == Qt::Key_S)
	{
		mViewCamera->zoom(0.0, 0.0, -0.10);
	}
	else if (e->key() == Qt::Key_Q)
	{
		mViewCamera->zoom(0.10, 0.0, 0.0);
	}
	else if (e->key() == Qt::Key_A)
	{
		mViewCamera->zoom(-0.10, 0.0, 0.0);
	}
	else if (e->key() == Qt::Key_E)
	{
		mViewCamera->zoom(0.0, 0.10, 0.0);
	}
	else if (e->key() == Qt::Key_D)
	{
		mViewCamera->zoom(0.0, -0.10, 0.0);
	}
	// Save frame buffer
	else if (e->key() == Qt::Key_P && e->modifiers() == Qt::ControlModifier)
	{
		saveFrameBuffer();
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
	mLastMousePos[0] = e->x();
	mLastMousePos[1] = e->y();
	if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
	{
		// Do something here
	}
};

void OGLViewer::mouseReleaseEvent(QMouseEvent *e)
{
	mLastMousePos[0] = e->x();
	mLastMousePos[1] = e->y();
}

void OGLViewer::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->x() - mLastMousePos[0];
	int dy = e->y() - mLastMousePos[1];

	//printf("dx: %d, dy: %d\n", dx, dy);

	if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
	{
		mViewCamera->rotate(dy * 0.25, -dx * 0.25, 0.0);
		update();
	}
	else if ((e->buttons() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			mViewCamera->zoom(0.0, 0.0, dx * 0.05);
			update();
		}
	}
	else if ((e->buttons() == Qt::MidButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			mViewCamera->zoom(-dx * 0.05, dy * 0.05, 0.0);
			update();
		}
	}
	else
	{
		QOpenGLWidget::mouseMoveEvent(e);
	}

	mLastMousePos[0] = e->x();
	mLastMousePos[1] = e->y();
}
/************************************************************************/
/* Application Functions                                                */
/************************************************************************/
void OGLViewer::resetCamera()
{
	mViewCamera.reset(new PerspectiveCamera(Point3f(10, 6, 10),
											Point3f(0, 0, 0),
											Vector3f(0, 1, 0),
											width() / Float(height())));
}

void OGLViewer::saveFrameBuffer()
{
	QString filename = QFileDialog::getSaveFileName(
		this, "Save Screenshot file...", "default", tr("PNG(*.png)"));
	grab().save(filename);
}