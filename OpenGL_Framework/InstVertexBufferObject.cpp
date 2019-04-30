#include "InstVertexBufferObject.h"

InstVertexArrayObject::InstVertexArrayObject()
	: VertexArrayObject()
{
}

InstVertexArrayObject::~InstVertexArrayObject()
{
	destroy();
}

void InstVertexArrayObject::createVAO(GLenum vboUsage)
{
	if (vaoHandle)
	{
		destroy();
	}

	vboUsageType = vboUsage;

	// Create and bind VAO handle.
	glGenVertexArrays(1, &vaoHandle);
	this->bind();


	// Create VBO handles
	auto numberOfBuffers = vboData.size();
	vboHandles.resize(numberOfBuffers);

	glGenBuffers(numberOfBuffers, &vboHandles[0]);

	// Setup VBOs
	for (size_t i = 0; i < numberOfBuffers; ++i)
	{
		VertexBufferData* attrib = &vboData[i];

		attrib->numVertices = attrib->numElements / attrib->numElementsPerAttribute;

		glEnableVertexAttribArray(attrib->attributeType);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
		glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->sizeOfElement, attrib->data, vboUsage);

		glVertexAttribPointer(attrib->attributeType, attrib->numElementsPerAttribute, attrib->elementType, GL_FALSE, 0, reinterpret_cast<void*>(0));
	}

	
	// Create instanced VBO handle
	glGenBuffers(1, &_instVboHandle);

	// Setup instanced VBO
	glBindBuffer(GL_ARRAY_BUFFER, _instVboHandle);

	// Consume as many attribute locations as needed
	for (size_t j = 0; j < 4; ++j)
	{
		glEnableVertexAttribArray(INSTANCED_COL_0 + j);

		glVertexAttribPointer(INSTANCED_COL_0 + j, 4, GL_FLOAT, GL_FALSE, sizeof(vec4) * 4, reinterpret_cast<void*>(sizeof(vec4) * j));

		glVertexAttribDivisor(INSTANCED_COL_0 + j, 1);
	}
	
	
	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	this->unbind();
}

void InstVertexArrayObject::draw(const size_t numInstances, vector<mat4> & transforms) const
{
	if (vaoHandle && transforms.size() > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _instVboHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * numInstances, transforms[0].data, GL_DYNAMIC_DRAW);


		this->bind();

		glDrawArraysInstanced(primitiveType, 0, vboData[0].numVertices, numInstances);

		this->unbind();
	}
}
