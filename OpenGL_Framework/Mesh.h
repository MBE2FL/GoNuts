#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
//#include <MiniMath/Core.h>	// Have to replace with own math library
#include "MathLib/MathLibCore.h"

#include "MeshBounds.h"

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char*)0 + (i))

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::strstr;
using std::sscanf;

class Mesh
{
public:
	Mesh();
	~Mesh();

	// Load a mesh, and send it to OpenGL
	bool loadFromFile(const string& file);
	// Release data from OpenGL (VRAM)
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int getNumVertices() const;

	GLuint getVBO_Verts();

	MeshBounds getMeshBounds() const;

	string getFilename() const;
	void setFilename(const string& filename);

	// OpenGL buffers and objects
	GLuint VBO_Vertices = GL_NONE;
	GLuint VBO_UVs = GL_NONE;
	GLuint VBO_Normals = GL_NONE;
	GLuint VAO = GL_NONE;

private:
	unsigned int _numFaces = 0;
	unsigned int _numVertices = 0;

	string _filename;

	MeshBounds _meshBounds;
	Vector3 minPoint;
	Vector3 maxPoint;

	void computeMinMax(const Vector3& vertex);
};