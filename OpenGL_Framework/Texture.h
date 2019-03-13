//#pragma once
//
//#include <GL/glew.h>
//#include <string>
#include <iostream>
#include "SOIL/SOIL.h"
#include <fstream>
//
//using std::string;
//using std::cout;
//using std::endl;
//
//class Texture
//{
//public:
//	Texture() = default;
//	~Texture();
//
//	bool load(const string& file);
//	void unload();
//
//	void bind();
//	void unBind();
//	void bind(const int textureSlot);
//	void unBind(const int textureSlot);
//
//	//- The handle to the texture object
//	GLuint texObj = GL_NONE;
//};
#pragma once
#include "GL/glew.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

struct RGB
{
	float r = 0;
	float g = 0;
	float b = 0;
};
class Texture
{
public:
	Texture() = default;
	Texture(const std::string &file, const bool LUT = false);
	~Texture();

	// Loads the texture
	bool load(const std::string &file);
	bool load3D(const std::string &file);
	// Unloads the texture
	bool unload();

	// Binds the texture to the last active slot
	void bind() const;
	// Binds the texture to a specific slot
	void bind(int textureSlot) const;
	// Unbinds the last active texture slot
	void unBind() const;
	// Unbinds a specific texture slot
	void unBind(int textureSlot) const;

	string getName() const;
	void setName(const string& name);
	int getSizeX() const;
	int getSizeY() const;

	std::string filename;
	GLuint _Handle;
	GLenum _Target;

	std::vector<RGB> LUTVec;

private:


	GLenum _InternalFormat;
	GLenum _FilterMag = GL_LINEAR; // Filter parameter
	GLenum _FilterMin = GL_LINEAR;
	GLenum _WrapU = GL_REPEAT; // Per axis Wrap parameter
	GLenum _WrapV = GL_REPEAT;

	int sizeX;
	int sizeY;
	int channels;

	string _name;
};