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
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

}

void OGLViewer::paintGL()
{
	glViewport(0, 0, width(), height());
	//glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
//////////////////////////////////////////////////////////////////////////
	// Read shader files
	const char* vertex_shader =
		"#version 440\n"
		"in vec3 vp;"
		"void main () {"
		"	gl_Position = vec4 (vp, 1.0);"
		"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char* fragment_shader =
		"#version 440\n"
		"out vec4 frag_colour;"
		"void main () {"
		"	frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";
	const char* fragment_shader2 =
		"#version 440\n"
		"out vec4 frag_colour;"
		"void main () {"
		"	frag_colour = vec4 (0.1, 0.5, 0.5, 1.0);"
		"}";
	/* GL shader objects for vertex and fragment shader [components] */
	GLuint vs, fs, fs2;
	/* GL shader programme object [combined, to link] */
	GLuint shader_programme, shader_programme2;

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, vs);
	glAttachShader(shader_programme, fs);
	glLinkProgram(shader_programme);


	fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs2, 1, &fragment_shader2, NULL);
	glCompileShader(fs2);
	shader_programme2 = glCreateProgram();
	glAttachShader(shader_programme2, vs);
	glAttachShader(shader_programme2, fs2);
	glLinkProgram(shader_programme2);
//////////////////////////////////////////////////////////////////////////

	GLuint myBufferID;
	glGenBuffers(1, &myBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID);
	
	GLdouble verts[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9, verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glUseProgram(shader_programme);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	
	GLdouble verts2[] = {
		0.0f, 0.0f, 0.0f,
		0.2f, -0.2f, -1.0f,
		-0.2f, -0.2f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * 9, verts2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glUseProgram(shader_programme2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
