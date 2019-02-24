#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
//#include <MiniMath/Core.h>	// Have to replace with own math library
#include "MathLib/MathLibCore.h"

#include "Collider.h"
#include "VertexBufferObject.h"


#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char*)0 + (i))

using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::strstr;
using std::sscanf;


struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	// Load a mesh, and send it to OpenGL
	virtual bool loadFromFile(const string& path);
	virtual void uploadToGPU();
	// Release data from OpenGL (VRAM)
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int getNumVertices() const;

	GLuint getVBO_Verts();

	Bounds getMeshBounds() const;

	string getFilename() const;
	void setFilename(const string& filename);
	string getName() const;
	void setName(const string& name);

	// OpenGL buffers and objects
	GLuint VBO_Vertices = GL_NONE;
	GLuint VBO_UVs = GL_NONE;
	GLuint VBO_Normals = GL_NONE;
	GLuint VAO = GL_NONE;

	VertexArrayObject vao;

	// TEST VArialbe
	bool _isSkeletal = false;

protected:
	unsigned int _numFaces = 0;
	unsigned int _numVertices = 0;

	string _filename;
	string _name;

	Bounds _meshBounds;
	vec3 minPoint = vec3(0.0f);
	vec3 maxPoint = vec3(0.0f);

	std::vector<vec4> dataVertex;
	std::vector<vec4> dataTexture;
	std::vector<vec4> dataNormal;
	std::vector<vec4> dataColor;

	bool _IsLoaded = false;

	void computeMinMax(const vec3& vertex);
};