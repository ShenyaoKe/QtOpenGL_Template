#include "GLSLProgram.h"


GLSLProgram::GLSLProgram()
	: program(0), shaders()
{
}


GLSLProgram::GLSLProgram(char *vert, char* frag,
	char* geom, char* tcs, char* tes) : program(0), shaders()
{
	create(vert, frag, geom, tcs, tes);
}

GLSLProgram::~GLSLProgram()
{
	//delete[]shaders;
}

bool GLSLProgram::create(char *vert, char* frag,
	char* geom, char* tcs, char* tes)
{
	if (vert == nullptr)
	{
		cout << "ERROR: Vertex shader required!\n";
		return false;
	}
	// Vertex Shader
	if (!create_shader(vert, shaders[0], GL_VERTEX_SHADER))
	{
		return false;
	}
	// Fragment Shader
	if (frag != nullptr)
	{
		if (!create_shader(frag, shaders[1], GL_FRAGMENT_SHADER))
		{
			return false;
		}
	}
	// Geometry Shader
	if (geom != nullptr)
	{
		if (!create_shader(geom, shaders[2], GL_GEOMETRY_SHADER))
		{
			return false;
		}
	}
	// Tessallation Control Shader
	if (tcs != nullptr)
	{
		if (!create_shader(tcs, shaders[3], GL_TESS_CONTROL_SHADER))
		{
			return false;
		}
	}
	// Tessallation Evaluation Shader
	if (tes != nullptr)
	{
		if (!create_shader(tes, shaders[4], GL_TESS_EVALUATION_SHADER))
		{
			return false;
		}
	}
	create_program();
	return true;
}

void GLSLProgram::del_program()
{
	glDeleteProgram(program);
	program = -1;
}

bool GLSLProgram::use_program() const
{
	glUseProgram(program);
	return true;
}

bool GLSLProgram::unuse() const
{
	glUseProgram(0);
	return true;
}

int GLSLProgram::getUniformLocation(const char *name) const
{
	return glGetUniformLocation(program, name);
}

void GLSLProgram::add_uniformv(const string &uniform)
{
	uniform_locs.insert(make_pair(uniform, glGetUniformLocation(program, uniform.c_str())));
}

bool GLSLProgram::read_shader_file(const char *file_name, char* &shader_str) const
{
	shader_str = nullptr; // reset string
	std::ifstream shader_file(file_name);
	if (!shader_file.is_open())
	{
		cout << "ERROR: Cannot open shader file " << file_name << endl;
		return false;
	}
	string contents((std::istreambuf_iterator<char>(shader_file)),
		std::istreambuf_iterator<char>());
	if (!contents.size())
	{
		cout << "ERROR: The shader file contains nothing in " << file_name << endl;
		return false;
	}
	delete shader_str;
	shader_str = new char[contents.size() + 1];
	memcpy(shader_str, contents.c_str(), contents.size() + 1);
	return true;
}
bool GLSLProgram::create_shader(const char *file_name, GLuint &shader, GLenum type)
{
	char *shader_str;//read(file_name)
	if (!read_shader_file(file_name, shader_str))
	{
		return false;
	}
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_str, nullptr);
	glCompileShader(shader);
	int params = -1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
	if (params == GL_FALSE)
	{
		cout << "ERROR: GL shader " << file_name << " did not compile successfully!\n";

#ifdef _DEBUG
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
		cout	<< "*************************************************************\n"
				<< "*                       Link log                            *\n"
				<< infoLog
				<< "*************************************************************\n";
		delete[] infoLog;
#endif
		return false;
	}
	return true;
}

bool GLSLProgram::create_program()
{
	program = glCreateProgram();
	for (int i = 0; i < 5; i++)
	{
		if (shaders[i] != 0)
		{
			glAttachShader(program, shaders[i]);
			//glDeleteShader(*shaders[i]);
		}
	}
	glLinkProgram(program);

	GLint params = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		cout<<"ERROR: could not link shader programme GL index %u\n";
		
#ifdef _DEBUG
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *infoLog = new GLchar[infoLogLength];
		glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
		cout	<< "*************************************************************\n"
				<< "*                       Link log                            *\n"
				<< infoLog
				<< "*************************************************************\n";
		delete[] infoLog;
#endif
		return false;
	}
	return true;
}
