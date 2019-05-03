#pragma once

#include "VertexBufferObject.h"
#include "MathLib/mat4.h"
#include "MathLib/vec4.h"

using std::vector;

class InstVertexArrayObject : public VertexArrayObject
{
public:
	InstVertexArrayObject();
	~InstVertexArrayObject();

	void createVAO(GLenum vboUsage = GL_STATIC_DRAW);
	void draw(const size_t numInstances, vector<mat4>& transforms) const;

private:
	GLuint _instVboHandle;
};