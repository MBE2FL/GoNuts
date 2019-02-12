#include "UniformBuffer.h"
#include "IO.h"

UniformBuffer::UniformBuffer()
{

}

UniformBuffer::UniformBuffer(unsigned int bytes)
{
	allocateMemory(bytes);
}

UniformBuffer::~UniformBuffer()
{
	unload();
}

void UniformBuffer::unload()
{
	if (_Handle)
	{
		glDeleteBuffers(1, &_Handle);
	}
}

bool UniformBuffer::isInit()
{
	return _IsInit;
}

void UniformBuffer::setName(std::string name)
{
	_Label = name;
	//setOpenGLName(GL_BUFFER, _Handle, _Label + "Uniform Buffer");
}

void UniformBuffer::allocateMemory(unsigned int bytes)
{
	if (_Handle)
	{
		unload();
	}
	glGenBuffers(1, &_Handle);
	_IsInit = true;
	_BytesAllocated = bytes;
	bind();
	// This allocates 'n' bytes on the GPU, after which, data can now be sent to the Buffer
	glBufferData(GL_UNIFORM_BUFFER, bytes, GL_NONE, GL_DYNAMIC_DRAW);
	unbind();
}

void UniformBuffer::sendMatrix(const mat4 &matrix, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 16 == 0, "mat4");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(mat4), &matrix);
	unbind();
}

void UniformBuffer::sendVector(const vec3 &vector, const int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 16 == 0, "vec3");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec3), &vector);
	unbind();
}

void UniformBuffer::sendVector(const vec4 &vector, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 16 == 0, "vec4");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec4), &vector);
	unbind();
}

void UniformBuffer::sendFloat(float scalar, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 4 == 0, "float");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &scalar);
	unbind();
}

void UniformBuffer::sendInt(int number, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 4 == 0, "int");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &number);
	unbind();
}

void UniformBuffer::sendUInt(unsigned int number, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 4 == 0, "uint");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(unsigned int), &number);
	unbind();
}

void UniformBuffer::sendBool(bool boolean, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 4 == 0, "bool");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(bool), &boolean);
	unbind();
}

void UniformBuffer::sendData(void * data, int size, int offset) const
{
	//SAT_ASSERT_ALIGN(offset SAT_OP_MOD 4 == 0, "data");
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	unbind();
}

void UniformBuffer::bind(GLuint slot)
{
	//SAT_ASSERT(_IsInit == true, "Uniform Buffer is not initialized!");

	glBindBufferBase(GL_UNIFORM_BUFFER, slot, _Handle);
	_BindLocation = slot;
}

void UniformBuffer::bind() const
{
	//SAT_ASSERT(_IsInit == true, "Uniform Buffer is not initialized!");

	glBindBuffer(GL_UNIFORM_BUFFER, _Handle);
}

void UniformBuffer::unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}
