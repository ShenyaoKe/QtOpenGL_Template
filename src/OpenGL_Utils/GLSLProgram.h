#pragma once
#ifndef __GLSLPROGRAM__
#define __GLSLPROGRAM__

#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::unordered_map;
/*
const GLenum shader_type[5] = {
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER
};*/

class GLSLProgram
{
public:
	GLSLProgram();
	GLSLProgram(char *vert, char* frag = nullptr, char* geom = nullptr,
		char* tcs = nullptr, char* tes = nullptr);
	~GLSLProgram();

	bool create(char *vert, char* frag = nullptr, char* geom = nullptr,
		char* tcs = nullptr, char* tes = nullptr);
	void del_program();
	bool use_program() const;
	bool unuse() const;

	// Uniform
	GLuint getUniformLocation(const char *name) const;
	void add_uniformv(const string &uniform);
	GLuint operator ()(const string &uniform);

	//GLuint operator[](const string &uniform);
private:
	bool read_shader_file(const char *file_name, char* &shader_str) const;
	bool create_shader(const char *file_name, GLuint &shader, GLenum type);
	bool create_program();

	GLuint shaders[5];
	GLuint program;
	unordered_map<string, GLuint> uniform_locs;
};

#endif // __GLSLPROGRAM__