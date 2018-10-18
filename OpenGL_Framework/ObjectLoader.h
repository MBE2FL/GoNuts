#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include <map>

using std::map;

class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	static void loadShaderProgram(const string& shaderProgramName, const string& vertFile, const string& fragFile);
	static void loadMesh(const string& meshName, const string& file);
	static ShaderProgram* getShaderProgram(const string& shaderProgramName);
	static Mesh* getMesh(const string& meshName);

private:
	static map<string, Mesh*> _meshes;
	static map<string, ShaderProgram*> _shaderPrograms;
};