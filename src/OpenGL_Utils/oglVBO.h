#pragma once
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>

class oglVBO
{
public:
	oglVBO();
	~oglVBO();

	//void CollapseCommonVertices(bool collapse);
	void Draw();
	void Begin(GLenum type);
	void Color4fv(GLfloat *rgba);
	void End();
	void Normal3fv(GLfloat *nxyz);
	void TexCoord2fv(GLfloat *st);
	void Vertex2fv(GLfloat *xy);
	void Vertex3fv(GLfloat *xyz);
	//void Print(char *str = "", FILE *out = stderr);
	//void RestartPrimitive();
	void SetTol(float tol);
private:
	GLfloat *pts;
	GLfloat *normal;
	GLfloat *vcolor;
	GLfloat *texcoord;
	GLuint num;
};