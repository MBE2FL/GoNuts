#include "VertexBufferObject.h"

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

void VertexArrayObject::createVAO(GLenum vboUsage)
{
	if (vaoHandle)
	{
		destroy();
	}

	vboUsageType = vboUsage;
	glGenVertexArrays(1, &vaoHandle);
	this->bind();
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
		glDrawArrays(primitiveType, 0, vboData[0].numVertices);
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
