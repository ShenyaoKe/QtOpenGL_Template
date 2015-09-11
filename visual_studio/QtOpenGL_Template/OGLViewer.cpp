#include "OGLViewer.h"

GLdouble* verts;// = new GLdouble[objectList.size() * 9];
GLdouble* uvs;// = new GLdouble[objectList.size() * 6];
GLdouble* norms;// = new GLdouble[objectList.size() * 9];
int vbo_size;
GLSLProgram* shader;

OGLViewer::OGLViewer()
{
}


OGLViewer::~OGLViewer()
{
}

void OGLViewer::initializeGL()
{
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	//gl_log("renderer: %s\nversion: %s\n", renderer, version);

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

	
	shader = new GLSLProgram("vert.glsl", "frag.glsl");

	disp_geo = new Mesh("D:/Learning/OpenGL/monkey2.obj");
	//obj.refine(objectList);
	
	exportVBO(disp_geo, vbo_size, verts, uvs, norms);
	
}

void OGLViewer::paintGL()
{
	glViewport(0, 0, width(), height());
	//glEnable(GL_DEPTH_TEST);
	//
	glClearColor(0.5, 0.8, 1.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*GLdouble* verts;// = new GLdouble[objectList.size() * 9];
	GLdouble* uvs;// = new GLdouble[objectList.size() * 6];
	GLdouble* norms;// = new GLdouble[objectList.size() * 9];
	int vbo_size;
	exportVBO(disp_geo, vbo_size, verts, uvs, norms);*/

	
	
	//pts vbo
	GLuint pts_vbo;
	glGenBuffers(1, &pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9 * vbo_size, verts, GL_STATIC_DRAW);

	GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9 * vbo_size, norms, GL_STATIC_DRAW);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);
//////////////////////////////////////////////////////////////////////////
	/*GLSLProgram shader("vert.vert", "frag.frag");

	GLdouble verts[] = {
		0.443178, 0.390707, -1.336396,
		-0.056822, -1.014719, -1.493799,
		0.943178, 0.233305, 0.069031,
		-0.620477, 0.500000, 0.431426,
		-1.120477, -0.500000, -0.568574,
		-0.120477, -0.500000, 0.831426
	};
	GLdouble *colors = new GLdouble[12];
	GLdouble colors2[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
	};
	for (int i = 0; i < 12; i++)
	{
		colors[i] = colors2[i];
	}
	//pts vbo
	GLuint pts_vbo;
	glGenBuffers(1, &pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 18, verts, GL_STATIC_DRAW);

	/ *GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 18, colors, GL_STATIC_DRAW);
* /

	delete colors;


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);*/
	/*GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);*/

	shader->use_program();
	glDrawArrays(GL_TRIANGLES, 0, vbo_size * 3);
}
