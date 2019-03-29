#pragma once


#include "GL/glew.h"
#include "MathLib/MathLibCore.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::istreambuf_iterator;
using std::vector;


class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	// Loads a vertex shader and a fragment shader, and places then in a program
	bool load(const string& vertFile, const string& fragFile);
	bool isLoaded() const;
	// Clear all data from OpenGL
	void unLoad();
	bool linkProgram();

	// Use / detach the shader from use
	void bind() const;
	void unBind();

	// Requires a re-link before OpenGL will register the change
	void addAttribute(unsigned int index, const string& attribName);

	// Returns -1 if the attribute does not exist
	int getAttribLocation(const string& attribName);
	// Returns -1 if uniform does not exist
	int getUniformLocation(const string& unifromName);

	// Send data to shaders
	void sendUniform(const string& name, int integer);
	void sendUniform(const string& name, unsigned int unsigned_integer);
	void sendUniform(const string& name, float scalar);
	void sendUniform(const string& name, const vec2& vector);
	void sendUniform(const string& name, const vec3& vector);
	void sendUniformVec3Array(const string& name, const unsigned int count, const float* vector);
	void sendUniform(const string& name, const vec4& vector);
	void sendUniformVec4Array(const string& name, const unsigned int count, const float* vector);
	void sendUniformMat3(const string& name, float* matrix, bool transpose);
	void sendUniformMat4(const string& name, float* matrix, bool transpose);
	void sendUniformMat4Array(const string& name, const unsigned int numOfMat, float* matrix, bool transpose);

	string getVertFilename() const;
	void setVertFilename(const string& vertFilename);
	string getFragFilename() const;
	void setFragFilename(const string& fragFilename);
	string getProgramName() const;
	void setProgramName(const string& programName);

	void reload();

private:
	bool _isInit = false;
	GLuint _vertexShader = GL_NONE;
	GLuint _fragShader = GL_NONE;
	GLuint _program = GL_NONE;

	string readFile(const string& fileName) const;
	bool compileShader(GLuint shader) const;
	void outputShaderLog(GLuint shader) const;
	void outputProgramLog() const;

	string _vertFilename;
	string _fragFilename;
	string _programName;
};