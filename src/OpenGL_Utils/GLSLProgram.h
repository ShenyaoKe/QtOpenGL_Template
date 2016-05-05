#pragma once
#ifndef __GLSLPROGRAM__
#define __GLSLPROGRAM__

#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using std::cout;
using std::cin;
using std::endl;

using std::string;
using std::vector;
using std::unordered_map;

class GLSLProgram
{
public:
	GLSLProgram();
	GLSLProgram(
		const char *vert, const char* frag = nullptr,
		const char* geom = nullptr,
		const char* tcs = nullptr, const char* tes = nullptr);
	~GLSLProgram();

	bool create(const char *vert, const char* frag = nullptr,
		const char* geom = nullptr,
		const char* tcs = nullptr, const char* tes = nullptr);
	void del_program();
	bool use_program() const;
	bool unuse() const;

	// Uniform
	GLuint getUniformLocation(const char *name) const;
	void add_uniformv(const string &uniform);
	GLuint operator ()(const string &uniform);
	GLuint operator [](const string &uniform);
	GLuint get(GLenum type = 0);

	//GLuint operator[](const string &uniform);
private:
	bool read_shader_file(const char *file_name, char* &shader_str) const;
	bool create_shader(const char *file_name, GLuint &shader, GLenum type);
	bool create_program();

	vector<GLuint> shaders;
	GLuint program;
	unordered_map<string, GLuint> uniform_locs;
};

#endif // __GLSLPROGRAM__