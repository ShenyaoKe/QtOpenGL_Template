#pragma once
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class oglPrimitive
{
public:
	oglPrimitive();
	virtual~oglPrimitive() = 0;
	virtual void create() = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void destroy() = 0;
	virtual void draw() = 0;
};

class oglUnitQuad
{
public:
	oglUnitQuad();
	~oglUnitQuad();

	void create();
	void destroy();
	void bind();
	void unbind();

	void draw();
private:
	GLuint vao, vbo;
};