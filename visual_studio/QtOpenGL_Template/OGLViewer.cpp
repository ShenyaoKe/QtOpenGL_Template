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
	timer->start(1);

	// Window Title
	setWindowTitle(tr("Bouncing Ball"));

	// Initialize transform matrix
	matrix = setRotation(tcount * 10, 0, 0);
	exportVBO(matrix, mtx);
}

OGLViewer::~OGLViewer()
{
}

// Redraw function
void OGLViewer::paintEvent(QPaintEvent *e)
{
	// Draw current frame
	paintGL();
	// Update status
	movement();

	char titlename[100];
	// Time delay
	while (1000.0 / process_time.elapsed() > fps)
	{
	}
	// Update fps at window title
	int err_s = sprintf_s(titlename, "Time Lapse: %.2ffps", 1000.0 / process_time.elapsed());
	if (err_s)
	{
		setWindowTitle(tr(titlename));
	}
	process_time.start();
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
	//gl_log("renderer: %s\nversion: %s\n", renderer, version);

	// Enable OpenGL features
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_BLEND);
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front
	

	//////////////////////////////////////////////////////////////////////////

	// Create shader files
	shader = new GLSLProgram("vert.glsl", "frag.glsl");

	// Read obj file
	disp_geo = new Mesh("D:/Learning/OpenGL/monkey2.obj");
	
	// Export vbo for shaders
	exportVBO(disp_geo, vbo_size, verts, uvs, norms);

	// Get uniform variable location
	mat_loc = shader->getUniformLocation("matrix");
	cout << "Uniform matrix location: " << mat_loc << endl;
}

void OGLViewer::paintGL()
{
	// Make curent window
	makeCurrent();
	// Clear background and color buffer
	glClearColor(0.5, 0.8, 1.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind VBOs
	//pts vbo
	GLuint pts_vbo;
	glGenBuffers(1, &pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * vbo_size, verts, GL_STATIC_DRAW);

	// Bind normal value as color
	GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * vbo_size, norms, GL_STATIC_DRAW);
	
	// Bind VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	// Use shader program
	shader->use_program();

	// Apply uniform matrix
	glUniformMatrix4fv(mat_loc, 1, GL_FALSE, mtx);
	glDrawArrays(GL_TRIANGLES, 0, vbo_size * 3);
}

void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void OGLViewer::movement()
{
	// Change current status for render
	tcount++;
	matrix = setRotation(tcount * 10, 0, 0);
	exportVBO(matrix, mtx);
}
