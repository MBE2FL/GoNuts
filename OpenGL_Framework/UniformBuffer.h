#pragma once
#include <GL/glew.h>
//#include <gmtk/gmtk.h>
#include "MathLib/MathLibCore.h"

/*
  ////////////////////
 // Uniform Buffer //
////////////////////

Uniform Buffer are used to hold data on the GPU. Similar to Vertex Buffer Object it's
inconvenient (and slow) to send the same data over and over when the frame is drawn.
Instead, anything that requires the data can simply bind the uniform buffer and utilize
it. The data only needs to be sent to the GPU when the values are changed.
They can also be extremely convenient for uniforms that are shared across multiple 
shader programs, as the values don't need to be shared across 'n' number of shaders.

Data can either be sent as one value at a time, or can be sent in one go if the data is
aligned in memory. Important to note, the layout for memory in uniform buffers are not 
aligned the exact same way as C++. std140 is similar to how memory is allocated in C++, 
but with small differences. Memory is still laid out based on declaration order, but arrays 
are automatically aligned to 16 bytes per element (the size of a vec4), a vec3 will be 
aligned the same way as a vec4

You can read more about the std140 specifications here
https://www.khronos.org/registry/OpenGL/specs/gl/glspec45.core.pdf#page=159

Here is an example from https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
layout (std140) uniform ExampleBlock
{
					 // base alignment  // aligned offset
	float value;     // 4               // 0
	vec3 vector;     // 16              // 16  (must be multiple of 16 so 4->16)
	mat4 matrix;     // 16              // 32  (column 0)
					 // 16              // 48  (column 1)
					 // 16              // 64  (column 2)
					 // 16              // 80  (column 3)
	float values[3]; // 16              // 96  (values[0])
					 // 16              // 112 (values[1])
					 // 16              // 128 (values[2])
	bool boolean;    // 4               // 144
	int integer;     // 4               // 148
};

Limitations of Uniform Buffers can be found using glGetIntegerv(GL_MACRO, &integer)
	-Each shader stage has a limit on the number of separate uniform buffer binding locations
		GL_MAX_VERTEX_UNIFORM_BLOCKS
		GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		GL_MAX_FRAGMENT_UNIFORM_BLOCKS
	
	-Max size of an individual UBO	
		GL_MAX_UNIFORM_BLOCK_SIZE

*/

class UniformBuffer
{
public:
	UniformBuffer();
	UniformBuffer(unsigned int bytes);
	~UniformBuffer();
	void unload();

	void allocateMemory(unsigned int bytes); // Initializes the Uniform Buffer Object
	void sendMatrix(const mat4 &matrix, int offset) const;
	void sendVector(const vec3 &vector, const int offset) const;
	void sendVector(const vec4 &vector, int offset) const;
	void sendFloat(float scalar, int offset) const;
	void sendInt(int scalar, int offset) const;
	void sendUInt(unsigned int scalar, int offset) const;
	void sendBool(bool boolean, int offset) const;
	void sendData(void* data, int size, int offset = 0) const;
	void bind(GLuint slot);	// Binds the uniform buffer to an active slot (similar to active texture slots)
	bool isInit();
	void setName(std::string name);

private:
	void bind() const;	// Binds the uniform buffer for sending data (similar to VBOs)
	void unbind() const; // Unbinds the uniform buffer for safety purposes, can help in preventing obscure bugs
	bool _IsInit = false;
	GLuint _Handle = 0;
	GLuint _BytesAllocated = 0;
	GLuint _BindLocation = 0;
	std::string _Label;
};