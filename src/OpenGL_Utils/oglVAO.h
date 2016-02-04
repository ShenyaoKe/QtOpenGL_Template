#pragma once
#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

class oglVAO
{
	GLuint handle;
	vector<GLuint> vboHandles;
public:
	oglVAO();
	~oglVAO();

	void clear();
};

