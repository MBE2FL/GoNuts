#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
	if (_isInit)
	{
		unLoad();
	}
}

bool ShaderProgram::load(const string & vertFile, const string & fragFile)
{
	setVertFilename(vertFile);
	setFragFilename(fragFile);

	//Create shader and program objects
	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	_program = glCreateProgram();

	// Load our source code
	string source = readFile(vertFile);
	const GLchar* temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(_vertexShader, 1, &temp, nullptr);

	// Load our source code
	source = readFile(fragFile);
	temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(_fragShader, 1, &temp, nullptr);

	// Compile the code
	if (!compileShader(_vertexShader))
	{
		cout << "Vertex shader failed to compile." << endl;

		outputShaderLog(_vertexShader);
		unLoad();

		return false;
	}

	if (!compileShader(_fragShader))
	{
		cout << "Fragment shader failed to compile." << endl;

		outputShaderLog(_fragShader);
		unLoad();

		return false;
	}


	// Setup our program object
	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragShader);

	if (!linkProgram())
	{
		cout << "Shader program failed to link." << endl;

		outputProgramLog();
		unLoad();

		return false;
	}

	_isInit = true;
	return true;
}

bool ShaderProgram::isLoaded() const
{
	return _isInit;
}

void ShaderProgram::unLoad()
{
	if (_vertexShader != GL_NONE)
	{
		glDetachShader(_program, _vertexShader);
		glDeleteShader(_vertexShader);
		_vertexShader = GL_NONE;
	}
	if (_fragShader != GL_NONE)
	{
		glDetachShader(_program, _fragShader);
		glDeleteShader(_fragShader);
		_fragShader = GL_NONE;
	}
	if (_program != GL_NONE)
	{
		glDeleteProgram(_program);
		_program = GL_NONE;
	}

	_isInit = false;
}

bool ShaderProgram::linkProgram()
{
	glLinkProgram(_program);

	GLint success;

	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	return (success == GL_TRUE);
}

void ShaderProgram::bind() const
{
	glUseProgram(_program);
}

void ShaderProgram::unBind()
{
	glUseProgram(GL_NONE);
}

void ShaderProgram::addAttribute(unsigned int index, const string & attribName)
{
	// in vec3 vert		0
	// in vec2 uv		1
	// in vec3 normals	2

	glBindAttribLocation(_program, index, attribName.c_str());
}

// Location won't change. Slow to constantly call. Save all locations once. Map names to integer locations
int ShaderProgram::getAttribLocation(const string & attribName)
{
	return glGetAttribLocation(_program, attribName.c_str());
}

int ShaderProgram::getUniformLocation(const string & unifromName)
{
	return glGetUniformLocation(_program, unifromName.c_str());
}

void ShaderProgram::sendUniform(const string & name, int integer)
{
	GLint location = getUniformLocation(name);
	glUniform1i(location, integer);
}

void ShaderProgram::sendUniform(const string & name, unsigned int unsigned_integer)
{
	GLint location = getUniformLocation(name);
	glUniform1ui(location, unsigned_integer);
}

void ShaderProgram::sendUniform(const string & name, float scalar)
{
	GLint location = getUniformLocation(name);
	glUniform1f(location, scalar);
}

void ShaderProgram::sendUniform(const string & name, const vec2 & vector)
{
	GLint location = getUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void ShaderProgram::sendUniform(const string & name, const vec3 & vector)
{
	GLint location = getUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void ShaderProgram::sendUniformVec3Array(const string & name, const unsigned int count, const float * vector)
{
	GLint location = getUniformLocation(name);
	glUniform3fv(location, count, vector);
}

void ShaderProgram::sendUniform(const string & name, const vec4 & vector)
{
	GLint location = getUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::sendUniformVec4Array(const string & name, const unsigned int count, const float* vector)
{
	GLint location = getUniformLocation(name);
	glUniform4fv(location, count, vector);
}

void ShaderProgram::sendUniformMat3(const string & name, float * matrix, bool transpose)
{
	GLint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void ShaderProgram::sendUniformMat4(const string & name, float * matrix, bool transpose)
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

void ShaderProgram::sendUniformMat4Array(const string & name, const unsigned int numOfMat, float * matrix, bool transpose)
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, numOfMat, transpose, matrix);
}

string ShaderProgram::getVertFilename() const
{
	return _vertFilename;
}

void ShaderProgram::setVertFilename(const string & vertFilename)
{
	_vertFilename = vertFilename;
}

string ShaderProgram::getFragFilename() const
{
	return _fragFilename;
}

void ShaderProgram::setFragFilename(const string & fragFilename)
{
	_fragFilename = fragFilename;
}

string ShaderProgram::getProgramName() const
{
	return _programName;
}

void ShaderProgram::setProgramName(const string & programName)
{
	_programName = programName;
}

void ShaderProgram::reload()
{
	load(_vertFilename, _fragFilename);
}

string ShaderProgram::readFile(const string & fileName) const
{
	ifstream inStream(fileName);

	if (!inStream.good())
	{
		cout << "Shader file not found." << endl;
		return string();
	}

	string data(istreambuf_iterator<char>(inStream), (istreambuf_iterator<char>()));
	return data;
}

bool ShaderProgram::compileShader(GLuint shader) const
{
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	return (success == GL_TRUE);
}

// Write to a text file
void ShaderProgram::outputShaderLog(GLuint shader) const
{
	vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]);

	cout << string(infoLog.begin(), infoLog.end()) << endl;
}

void ShaderProgram::outputProgramLog() const
{
	vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &infoLen);

	glGetProgramInfoLog(_program, sizeof(char) * 512, &infoLen, &infoLog[0]);

	cout << string(infoLog.begin(), infoLog.end()) << endl;
}
