#include "OGLViewer.h"


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

	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs, fs2;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme;
	

}

void OGLViewer::paintGL()
{
	glViewport(0, 0, width(), height());
	//glEnable(GL_DEPTH_TEST);
	//
	glClearColor(0.5, 0.8, 1.0, 0.5);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//////////////////////////////////////////////////////////////////////////
	GLSLProgram shader("vert.vert", "frag.frag");

	GLdouble verts[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, -1.0f,
		0.5f, -0.5f, 1.0f
	};
	GLdouble *colors = new GLdouble[12];
	GLdouble colors2[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f
	};
	for (int i = 0; i < 12; i++)
	{
		colors[i] = colors2[i];
	}
	//pts vbo
	GLuint pts_vbo;
	glGenBuffers(1, &pts_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9, verts, GL_STATIC_DRAW);

	GLuint color_vbo;
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 12, colors, GL_STATIC_DRAW);

	delete colors;

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, pts_vbo);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	//glUseProgram(*shader.program);
	shader.use_program();
	//glUseProgram(shader_programme);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	
	/*GLdouble verts2[] = {
		0.0f, 0.0f, 0.0f,
		0.2f, -0.2f, -1.0f,
		-0.2f, -0.2f, 1.0f
	};
	GLuint vbo2;
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9, verts2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glUseProgram(shader_programme2);
	glDrawArrays(GL_TRIANGLES, 0, 3);*/
}
