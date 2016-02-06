#include "OGLViewer.h"
#include <QMatrix4x4>

OGLViewer::OGLViewer(QWidget *parent)
	: QOpenGLWidget(parent), tcount(0), fps(30)
	, m_selectMode(OBJECT_SELECT)
	, view_cam(new perspCamera(
		Point3D(10, 6, 10), Point3D(0.0, 0.0, 0.0), Point3D(0, 1, 0),
		width() / static_cast<double>(height())))
	, box_mesh(nullptr), model_mesh(nullptr)
{
	// Set surface format for current widget
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	//format.setSamples(16);
	format.setVersion(4, 5);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);

	// Link timer trigger
	/*process_time.start();
	QTimer *timer = new QTimer(this);
	//timer->setSingleShot(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(0);*/

	// Read obj file
	box_mesh = new Mesh("../../scene/obj/cube_large.obj");
	model_mesh = new Mesh("../../scene/obj/monkey.obj");

	view_cam->exportVBO(view_mat, proj_mat, nullptr);
	/*proj = Matrix4D::Perspective(60, width() / static_cast<float>(height()), 0.1, 100);
	//proj.printInfo("Default perspective matrix");
	view = Matrix4D::LookAt(Point3D(3, 1, 3), Point3D(0, 0, 0));
	view.printInfo("Default LookAt matrix");
	view = view.Inverse();
	view.exportVBO(view_mat);
	proj.exportVBO(proj_mat);*/
	//QMatrix4x4 qproj;
	//qproj.perspective(60, 1, 0.1, 100);
	//qproj.inverted();
	//resetCamera();
}

OGLViewer::~OGLViewer()
{
	delete box_mesh;
	box_mesh = nullptr;
	delete model_mesh;
	model_mesh = nullptr;
	delete model_shader;
	model_shader = nullptr;
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


	//////////////////////////////////////////////////////////////////////////

	// Create shader files
	model_shader = new GLSLProgram("vert.glsl", "frag.glsl");

	// Export vbo for shaders
	box_mesh->exportVBO(&box_verts, &box_uvs, &box_norms);
	model_mesh->exportVBO(&model_verts, &model_uvs, &model_norms);

	bindBox();
	bindMesh();

	// Get uniform variable location
	model_mat_loc = model_shader->getUniformLocation("model_matrix");
	view_mat_loc = model_shader->getUniformLocation("view_matrix");
	proj_mat_loc = model_shader->getUniformLocation("proj_matrix");; // WorldToCamera matrix
}

void OGLViewer::bindBox()
{
	glDeleteBuffers(1, &box_vert_vbo);
	glDeleteBuffers(1, &box_norm_vbo);
	glDeleteVertexArrays(1, &box_vao);
	// Bind VAO
	glGenVertexArrays(1, &box_vao);
	glBindVertexArray(box_vao);

	//GLuint box_vert_vbo;
	glGenBuffers(1, &box_vert_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * box_verts.size(), &box_verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Bind normal value as color
	//GLuint box_norm_vbo;
	glGenBuffers(1, &box_norm_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_norm_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * box_norms.size(), &box_norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void OGLViewer::bindMesh()
{
	glDeleteBuffers(1, &model_vert_vbo);
	glDeleteBuffers(1, &model_norm_vbo);
	glDeleteBuffers(1, &model_uv_vbo);
	glDeleteVertexArrays(1, &model_vao);

	// Bind VAO
	glGenVertexArrays(1, &model_vao);
	glBindVertexArray(model_vao);
	
	glGenBuffers(1, &model_vert_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model_verts.size(), &model_verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Bind normal value as color
	glGenBuffers(1, &model_norm_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_norm_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * model_norms.size(), &model_norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// Bind UV values
	/*glGenBuffers(1, &model_uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * model_vbo_size, model_uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);*/
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLViewer::paintGL()
{
	// Make curent window
	makeCurrent();
	// Clear background and color buffer
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (display_mode == 0)// Wireframe
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
	vector<GLfloat> projmatvec(proj_mat, proj_mat + 16);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat);
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, &projmatvec[0]);
	glDrawArrays(GL_TRIANGLES, 0, box_verts.size() / 3);

	//////////////////////////////////////////////////////////////////////////
	// Model
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(model_vao);
	model_shader->use_program();

	// Apply uniform matrix
	//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat);
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, proj_mat);
	glDrawArrays(GL_TRIANGLES, 0, model_verts.size() / 3);
	glBindVertexArray(0);
}
// Redraw function
void OGLViewer::paintEvent(QPaintEvent *e)
{
	// Draw current frame
	paintGL();
}
// Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	view_cam->resizeViewport(width() / static_cast<double>(height()));
	//view_cam->setResolution(width(), height());
	view_cam->exportVBO(nullptr, proj_mat, nullptr);
}
/************************************************************************/
/* Qt User Operation Functions                                          */
/************************************************************************/
void OGLViewer::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_W)
	{
		view_cam->zoom(0.0, 0.0, 0.10);
		view_cam->exportVBO(view_mat, nullptr, nullptr);

	}
	else if (e->key() == Qt::Key_S)
	{
		view_cam->zoom(0.0, 0.0, -0.10);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if (e->key() == Qt::Key_Q)
	{
		view_cam->zoom(0.10, 0.0, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if (e->key() == Qt::Key_A)
	{
		view_cam->zoom(-0.10, 0.0, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if (e->key() == Qt::Key_E)
	{
		view_cam->zoom(0.0, 0.10, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if (e->key() == Qt::Key_D)
	{
		view_cam->zoom(0.0, -0.10, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
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
		view_cam->exportVBO(view_mat, nullptr, nullptr);
		update();
	}
	else if ((e->buttons() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(0.0, 0.0, dx * 0.05);
			view_cam->exportVBO(view_mat, nullptr, nullptr);
			update();
		}
	}
	else if ((e->buttons() == Qt::MidButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(dx * 0.05, dy * 0.05, 0.0);
			view_cam->exportVBO(view_mat, nullptr, nullptr);
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
	delete view_cam;
	//Matrix4D cam2w = Matrix4D::LookAt(Point3D(10, 6, 10), Point3D(0.0, 0.0, 0.0), Point3D(0, 1, 0));

	//view_cam->setCamToWorld(Matrix4D::LookAt(Point3D(10, 6, 10), Point3D(0.0, 0.0, 0.0), Point3D(0, 1, 0)));
	//Transform pers = Transform(Matrix4D::Perspective(67,
	//	width() / static_cast<double>(height()), 0.1, 100));
	view_cam = new perspCamera(
		Point3D(10, 6, 10), Point3D(0.0, 0.0, 0.0), Point3D(0, 1, 0),
		width() / static_cast<double>(height()));
	//view_cam->exportVBO(view_mat, proj_mat, nullptr);
	//update();
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