#include "OGLViewer.h"

OGLViewer::OGLViewer(QWidget *parent)
	: QOpenGLWidget(parent), tcount(0), fps(30)
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

}

OGLViewer::~OGLViewer()
{
}
void OGLViewer::resetCamera()
{
	Transform cam2w = lookAt(Point3D(9, 12.0, -10.0), Point3D(0.0, 0, 0.0), Point3D(0, 1, 0));
	Transform pers = Transform(setPerspective(67,
		width() / static_cast<double>(height()), 0.1, 100));
	view_cam = new perspCamera(cam2w, pers);
	view_cam->exportVBO(view_mat, proj_mat, nullptr);
	//update();
}
void OGLViewer::initializeGL()
{
	// OpenGL extention initialization
	glewInit();

	// Print OpenGL vertion
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	shader = new GLSLProgram("vert.glsl", "frag.glsl");

}
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

void OGLViewer::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->x() - m_lastMousePos[0];
	int dy = e->y() - m_lastMousePos[1];

	printf("dx: %d, dy: %d\n", dx, dy);
	if ((e->buttons() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
	{
		view_cam->rotate(-dy * 0.25, dx * 0.25, 0.0);
		view_cam->exportVBO(view_mat, nullptr, nullptr);
	}
	else if ((e->buttons() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier))
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(0.0, 0.0, dx * 0.10);
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



void OGLViewer::paintGL()
{
	// Make curent window
	makeCurrent();
	// Clear background and color buffer
	glClearColor(0.25, 0.4, 0.5, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	//////////////////////////////////////////////////////////////////////////
	
	GLfloat tri[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	glEnable(GL_DEPTH_TEST); /* enable depth-testing */
	/* with LESS depth-testing interprets a smaller value as "closer" */
	glDepthFunc(GL_LESS);

	shader->use_program();

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), tri,
		GL_STATIC_DRAW);

	GLfloat offsets[] = { 0.1f, 0.2f, 0.3f };
	GLuint off_vbo;
	glGenBuffers(1, &off_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, off_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(offsets), offsets,
		GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	// Apply uniform matrix
	glDrawArrays(GL_TRIANGLES, 0,  3);
	//glBindVertexArray(vao);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, off_vbo);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	//glVertexAttribDivisor(1, 1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindVertexArray(sphere_vao);
	//glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, 3);
	//glDrawArraysInstanced(GL_TRIANGLES, 0, 3, sphere_vbo_size);
}
// Redraw function
void OGLViewer::paintEvent(QPaintEvent *e)
{
	// Draw current frame
	paintGL();
	
	process_fps = 1000.0 / process_time.elapsed();

	process_time.start();
}
//Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
/*
	view_cam->resizeViewport(width() / static_cast<double>(height()));
	view_cam->exportVBO(nullptr, proj_mat, nullptr);*/
	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OGLViewer::initParas()
{
}