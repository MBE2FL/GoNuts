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

	bool load(const string& file);
	void unload();

	void bind();
	void unBind();
	void bind(const int textureSlot);
	void unBind(const int textureSlot);

	//- The handle to the texture object
	GLuint texObj = GL_NONE;
};