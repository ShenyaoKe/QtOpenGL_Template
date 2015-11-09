#include "OGLViewer.h"

OGLViewer::OGLViewer(QWidget *parent)
	: QOpenGLWidget(parent), tcount(0), fps(30)
	, m_selectMode(OBJECT_SELECT)
{
	// Set surface format for current widget
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);

	// Link timer trigger
	process_time.start();
	QTimer *timer = new QTimer(this);
	/*timer->setSingleShot(false);*/
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(0);


	// Read obj file
	box_mesh = new Mesh("../../scene/obj/cube_large.obj");
	model_mesh = new Mesh("../../scene/obj/monkey.obj");


	resetCamera();
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
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// Enable OpenGL features
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front


	//////////////////////////////////////////////////////////////////////////

	// Create shader files
	shader = new GLSLProgram("vert.glsl", "frag.glsl");

	// Export vbo for shaders
	box_mesh->exportVBO(box_vbo_size, box_verts, box_uvs, box_norms);
	model_mesh->exportVBO(model_vbo_size, model_verts, model_uvs, model_norms);

	bindBox();
	bindMesh();

	// Get uniform variable location
	model_mat_loc = shader->getUniformLocation("model_matrix");
	view_mat_loc = shader->getUniformLocation("view_matrix");
	proj_mat_loc = shader->getUniformLocation("proj_matrix");; // WorldToCamera matrix

	cout << "Model matrix location: " << model_mat_loc << endl;
	cout << "View matrix location: " << view_mat_loc << endl;
	cout << "Projection matrix location: " << proj_mat_loc << endl;
	cout << "Selection ID loc:" << sel_id_loc << endl;

}

void OGLViewer::bindBox()
{
	GLuint box_pts_vbo;
	glGenBuffers(1, &box_pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * box_vbo_size, box_verts, GL_STATIC_DRAW);

	// Bind normal value as color
	GLuint box_normal_vbo;
	glGenBuffers(1, &box_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * box_vbo_size, box_norms, GL_STATIC_DRAW);

	// Bind VAO
	GLuint box_vao;
	glGenVertexArrays(1, &box_vao);
	glBindVertexArray(box_vao);
	glBindBuffer(GL_ARRAY_BUFFER, box_pts_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, box_normal_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	vao_handles.push_back(box_vao);
}

void OGLViewer::bindMesh()
{
	GLuint model_pts_vbo;
	glGenBuffers(1, &model_pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * model_vbo_size, model_verts, GL_STATIC_DRAW);

	// Bind normal value as color
	GLuint model_normal_vbo;
	glGenBuffers(1, &model_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * model_vbo_size, model_norms, GL_STATIC_DRAW);

	// Bind normal value as color
	GLuint model_uv_vbo;
	glGenBuffers(1, &model_uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * model_vbo_size, model_uvs, GL_STATIC_DRAW);

	// Bind VAO
	GLuint model_vao;
	glGenVertexArrays(1, &model_vao);
	glBindVertexArray(model_vao);
	glBindBuffer(GL_ARRAY_BUFFER, model_pts_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, model_normal_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, model_uv_vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);

	vao_handles.push_back(model_vao);
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

		//glEnable(GL_BLEND);
	}
	// Bind Box VAOs
	glBindVertexArray(vao_handles[0]);
	// Use shader program
	shader->use_program();
	// Apply uniform matrix
	//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat);
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, proj_mat);
	glDrawArrays(GL_TRIANGLES, 0, box_vbo_size * 3);
	//////////////////////////////////////////////////////////////////////////
	// Model
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(vao_handles[1]);
	shader->use_program();

	// Apply uniform matrix
	//glUniformMatrix4fv(model_mat_loc, 1, GL_FALSE, model_mat);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat);
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, proj_mat);
	glDrawArrays(GL_TRIANGLES, 0, model_vbo_size * 3);


}
// Redraw function
void OGLViewer::paintEvent(QPaintEvent *e)
{
	// Draw current frame
	paintGL();

	process_fps = 1000.0 / process_time.elapsed();

	process_time.start();
}
// Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	view_cam->resizeViewport(width() / static_cast<double>(height()));
	view_cam->setResolution(width(), height());
	view_cam->exportVBO(nullptr, proj_mat, nullptr);
	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	// Selection mode switch
	else if (e->key() == Qt::Key_F8)
	{
		m_selectMode = OBJECT_SELECT;
	}
	else if (e->key() == Qt::Key_F11)
	{
		m_selectMode = FACE_COMPONENT_SELECT;
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

	printf("dx: %d, dy: %d\n", dx, dy);

	if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
	{
		view_cam->rotate(dy * 0.25, -dx * 0.25, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if ((e->buttons() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(0.0, 0.0, dx * 0.05);
			view_cam->exportVBO(view_mat, nullptr, nullptr);
		}
	}
	else if ((e->buttons() == Qt::MidButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(dx * 0.05, dy * 0.05, 0.0);
			view_cam->exportVBO(view_mat, nullptr, nullptr);
		}
	}
	/*else
	{
	QOpenGLWidget::mouseMoveEvent(e);
	}*/

	m_lastMousePos[0] = e->x();
	m_lastMousePos[1] = e->y();
	update();
}
/************************************************************************/
/* Application Functions                                                */
/************************************************************************/
void OGLViewer::resetCamera()
{
	Transform cam2w = lookAt(Point3D(10, 6, 10), Point3D(0.0, 0.0, 0.0), Point3D(0, 1, 0));
	Transform pers = Transform(setPerspective(67,
		width() / static_cast<double>(height()), 0.1, 100));
	view_cam = new perspCamera(cam2w, pers);
	view_cam->exportVBO(view_mat, proj_mat, nullptr);
	//update();
}
void OGLViewer::initParas()
{
	update();
}

void OGLViewer::saveFrameBuffer()
{
	this->grab().save("../../scene/texture/framebuffer.png");

}