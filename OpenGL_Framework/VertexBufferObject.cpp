#include "VertexBufferObject.h"
#include "MathLib/MathLibCore.h"

VertexArrayObject::VertexArrayObject()
{
	vaoHandle = 0;
	primitiveType = GL_TRIANGLES;
}

VertexArrayObject::~VertexArrayObject()
{
	destroy();
}

int VertexArrayObject::addVBO(VertexBufferData descriptor)
{
	vboData.push_back(descriptor);
	return 1;
}

int VertexArrayObject::addIBO(IndexBufferData descriptor)
{
	iboData = descriptor;
	isIndexed = true;
	return 1;
}

VertexBufferData * VertexArrayObject::getVboData(AttributeLocations loc)
{
	for (size_t i = 0; i < vboData.size(); ++i)
	{
		if (vboData[i].attributeType == loc)
		{
			return &vboData[i];
		}
	}
	return nullptr;
}

GLuint VertexArrayObject::getVaoHandle() const
{
	return vaoHandle;
}

GLenum VertexArrayObject::getPrimitiveType() const
{
	return primitiveType;
}

void VertexArrayObject::setPrimitiveType(GLenum type)
{
	primitiveType = type;
}

GLuint VertexArrayObject::getVboHandle(AttributeLocations loc) const
{
	for (size_t i = 0; i < vboData.size(); ++i)
	{
		if (vboData[i].attributeType == loc)
		{
			return vboHandles[i];
		}
	}
	return 0;
}

void VertexArrayObject::createVAO(GLenum vboUsage, GLenum iboUsage)
{
	if (vaoHandle)
	{
		destroy();
	}

	vboUsageType = vboUsage;
	iboUsageType = iboUsage;

	// Create and bind VAO handle.
	glGenVertexArrays(1, &vaoHandle);
	this->bind();


	if (!isIndexed)
	{
		// Create VBO handles
		auto numberOfBuffers = vboData.size();
		vboHandles.resize(numberOfBuffers);

		glGenBuffers(numberOfBuffers, &vboHandles[0]);

		for (size_t i = 0; i < numberOfBuffers; ++i)
		{
			VertexBufferData* attrib = &vboData[i];

			attrib->numVertices = attrib->numElements / attrib->numElementsPerAttribute;

			glEnableVertexAttribArray(attrib->attributeType);
			glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
			glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->sizeOfElement, attrib->data, vboUsage);

			glVertexAttribPointer(attrib->attributeType, attrib->numElementsPerAttribute, attrib->elementType, GL_FALSE, 0, reinterpret_cast<void*>(0));
		}
	}


	if (isIndexed)
	{
		// Create VBO handles
		auto numberOfBuffers = vboData.size();
		vboHandles.resize(numberOfBuffers);

		// Create IBO handle
		glGenBuffers(numberOfBuffers, &vboHandles[0]);
		glGenBuffers(1, &iboHandle);

		IndexBufferData* iboAttrib = &iboData;

		for (size_t i = 0; i < numberOfBuffers; ++i)
		{
			VertexBufferData* attrib = &vboData[i];

			attrib->numVertices = attrib->numElements / attrib->numElementsPerAttribute;

			glEnableVertexAttribArray(attrib->attributeType);
			glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
			glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->sizeOfElement, attrib->data, vboUsage);

			glVertexAttribPointer(attrib->attributeType, attrib->numElementsPerAttribute, attrib->elementType, GL_FALSE, 0, reinterpret_cast<void*>(0));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboAttrib->numIndices * iboAttrib->sizeOfIndex, iboAttrib->data, iboUsage);
		}
	}

	// Cleanup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	this->unbind();
}

void VertexArrayObject::createParticleInstancedVAO(GLenum vboUsage)
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

	for (size_t i = 0; i < numberOfBuffers; ++i)
	{
		VertexBufferData* attrib = &vboData[i];

		attrib->numVertices = attrib->numElements / attrib->numElementsPerAttribute;

		// Use next 4 attribute locations for particle instanced model matrices
		if (attrib->attributeType == INSTANCED_COL_0)
		{
			// For each particles model matrix
			size_t vec4Size = sizeof(vec4);
			glEnableVertexAttribArray(INSTANCED_COL_0);
			glVertexAttribPointer(INSTANCED_COL_0, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(INSTANCED_COL_1);
			glVertexAttribPointer(INSTANCED_COL_1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(vec4Size));
			glEnableVertexAttribArray(INSTANCED_COL_2);
			glVertexAttribPointer(INSTANCED_COL_2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(2 * vec4Size));
			glEnableVertexAttribArray(INSTANCED_COL_3);
			glVertexAttribPointer(INSTANCED_COL_3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, reinterpret_cast<void*>(3 * vec4Size));


			glVertexAttribDivisor(INSTANCED_COL_0, 1);
			glVertexAttribDivisor(INSTANCED_COL_1, 1);
			glVertexAttribDivisor(INSTANCED_COL_2, 1);
			glVertexAttribDivisor(INSTANCED_COL_3, 1);
		}

		// Use a single attribute location
		glEnableVertexAttribArray(attrib->attributeType);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
		glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->sizeOfElement, attrib->data, vboUsage);

		glVertexAttribPointer(attrib->attributeType, attrib->numElementsPerAttribute, attrib->elementType, GL_FALSE, 0, reinterpret_cast<void*>(0));
	}


	// Cleanup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	this->unbind();
}

void VertexArrayObject::reuploadVAO()
{
	if (vboUsageType == GL_DYNAMIC_DRAW)
	{
		this->bind();
		for (size_t i = 0; i < vboData.size(); ++i)
		{
			VertexBufferData* attrib = &vboData[i];

			glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, attrib->numElements * attrib->sizeOfElement, attrib->data);
		}
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
		this->unbind();
	}
	else
	{
		//SAT_DEBUG_LOG_WARNING("VAO #%d is not dynamic!", vaoHandle);
	}
}

void VertexArrayObject::draw() const
{
	if (vaoHandle)
	{
		this->bind();

		if (!isIndexed)
		{
			glDrawArrays(primitiveType, 0, vboData[0].numVertices);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
			glDrawElements(primitiveType, iboData.numIndices, iboData.elementType, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
		}

		this->unbind();
	}
}

void VertexArrayObject::drawParticles(size_t numParticles) const
{
	if (vaoHandle)
	{
		this->bind();

		glDrawArraysInstanced(primitiveType, 0, vboData[0].numVertices, numParticles);

		this->unbind();
	}
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(vaoHandle);
}

void VertexArrayObject::unbind() const
{
	glBindVertexArray(GL_NONE);
}

void VertexArrayObject::destroy()
{
	if(vaoHandle)
	{//Delete the data on the GPU
		glDeleteVertexArrays(1, &vaoHandle);
		glDeleteBuffers((GLsizei)vboHandles.size(), &vboHandles[0]);
		vaoHandle = 0;
	}

	vboHandles.clear();
	vboData.clear();
}
