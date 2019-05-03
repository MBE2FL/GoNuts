#pragma once

#include "GL/glew.h"
#include <vector>
#include "IO.h"
/*
  ////////////////////////////////
 // Vertex Array/Buffer Object //
////////////////////////////////

Vertex Buffer Objects are normal Buffer Objects used as a source for vertex array data.
Vertex Array Objects link together VBOs store all of the states needed to supply vertex data, 
along with what formats those data are in. They are used for drawing triangles to a framebuffer.

More info about VAOs and VBOs can be read here
https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object

Limitations of VAOs and VBOs can be found using glGetIntegerv(GL_MACRO, &integer)
	-VAOs have a maximum number of vertex attributes
		GL_MAX_VERTEX_ATTRIBS

*/

// Vertex Buffer Locations
enum AttributeLocations
{
	VERTEX = 0,
	TEXCOORD = 1,
	NORMAL = 2,
	JOINT_IDS = 3,
	JOINT_WEIGHTS = 4,
	COLOR = 5,
	INSTANCED_COL_0 = 12,
	INSTANCED_COL_1 = 13,
	INSTANCED_COL_2 = 14,
	INSTANCED_COL_3 = 15
};


struct VertexBufferData
{
	VertexBufferData()
	{
		numVertices = 0;
		numElements = 0;
		elementType = GL_FLOAT;
		attributeType = AttributeLocations::VERTEX;
	}


	AttributeLocations attributeType;
	GLuint numVertices;
	GLuint numElements;
	GLuint numElementsPerAttribute;
	GLuint sizeOfElement;
	GLenum elementType;

	void* data;

	// What does a VBO require?
	// Each VBO has a location AKA which VBO slot for the VAO
	// We can also keep track of what kind of data it is too
		// Along with the size of the elements: sizeof(float)
	// How many elements per attribute? 1/2/3/4?
	// Total number of vertices
	// Total number of elements?
	// Maybe we can store the type of attribute as a string too, why not?
	// the data itself

};

struct IndexBufferData
{
	IndexBufferData()
	{
		numIndices = 0;
		elementType = GL_UNSIGNED_INT;
	}


	GLuint numIndices;
	GLuint sizeOfIndex;
	GLenum elementType;

	void* data;
};

class VertexArrayObject
{
public:
	VertexArrayObject();
	virtual ~VertexArrayObject();

	int addVBO(VertexBufferData descriptor);
	int addIBO(IndexBufferData descriptor);

	VertexBufferData* getVboData(AttributeLocations loc);

	GLuint getVaoHandle() const;
	GLuint getVboHandle(AttributeLocations loc) const;
	GLenum getPrimitiveType() const; 
	void setPrimitiveType(GLenum type);

	virtual void createVAO(GLenum vboUsage = GL_STATIC_DRAW, GLenum iboUsage = GL_STATIC_DRAW);
	void reuploadVAO();

	virtual void draw() const;

	void bind() const;
	void unbind() const;

	void destroy();

protected:
	GLuint vaoHandle; // Handle for the VAO itself
	GLenum primitiveType; // How the primitive is drawn Ex GL_TRIANGLE/GL_LINE/GL_POINT
	GLenum vboUsageType;
	GLenum iboUsageType;
	// https://www.khronos.org/opengl/wiki/Primitive	//GL_TRIANGLE_STRIP/GL_LINE_STRIP
	std::vector<VertexBufferData> vboData;	// Vector for the vbo data and their respective handles
	IndexBufferData iboData;
	std::vector<GLuint> vboHandles;
	GLuint iboHandle;
	// We separate the handles from the data itself so that you can reuse the same data on the CPU
	// and send it to 2 separate VAO's for instance, morpth targets with multiple keyframes

	bool isIndexed = false;
};