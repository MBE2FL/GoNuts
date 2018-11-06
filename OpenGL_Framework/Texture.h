#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include "SOIL/SOIL.h"

using std::string;
using std::cout;
using std::endl;

class Texture
{
public:
	Texture() = default;
	~Texture();

	bool Load(const string& file);
	void Unload();

	void Bind();
	void UnBind();

	//- The handle to the texture object
	GLuint texObj = GL_NONE;
};