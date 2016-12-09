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

class GLSLShader
{
public:
    GLSLShader(GLenum shader_t);
	GLSLShader(const string &filename, GLenum shader_t);
	~GLSLShader();
protected:
	
private:
};
class GLSLProgram
{
public:
	//GLSLProgram();
	GLSLProgram(const char* vert,
                const char* frag = nullptr,
		        const char* geom = nullptr,
		        const char* tcs = nullptr,
                const char* tes = nullptr,
                bool isXformFeedback = false,
                size_t xformFdbVaryingCount = 0,
                const char* xformVaryings[] = nullptr);
	~GLSLProgram();

	bool create_shaders(const char* vert,
                        const char* frag = nullptr,
		                const char* geom = nullptr,
		                const char* tcs  = nullptr,
                        const char* tes  = nullptr);
	void del_program();
	bool use_program() const;
    bool unuse() const;
    // Refined workflow
    bool attachShader(GLuint shader_handle, GLenum shader_t);
    bool attachShaderFromFile(const char* vert, GLenum shader_t);
    bool attachShaderFromString(const char* vert, GLenum shader_t);
    bool attachShaders(const char* vert = nullptr,
                       const char* frag = nullptr,
                       const char* geom = nullptr,
                       const char* tcs  = nullptr,
                       const char* tes  = nullptr);
    bool createProgram();
    bool linkProgram();

	// Uniform
	GLuint getUniformLocation(const char *name) const;
	void add_uniformv(const string &uniform);
	GLuint operator () (const string &uniform);
	GLuint operator [] (const string &uniform);
	GLuint get(GLenum type = 0);

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