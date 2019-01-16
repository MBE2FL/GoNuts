#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <map>

using std::map;

class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	static void loadShaderProgram(const string& shaderProgramName, const string& vertFile, const string& fragFile);
	static void loadMesh(const string& meshName, const string& file);
	static void loadMesh(const string& meshName, const string& file, const int totalObjs);
	static void loadTexture(const string& texName, const string& file);
	static ShaderProgram* getShaderProgram(const string& shaderProgramName);
	static vector<ShaderProgram*> getShaderPrograms();
	static vector<string> getShaderProgramNames();
	static Mesh* getMesh(const string& meshName);
	static vector<Mesh*> getMeshes();
	static Texture* getTexture(const string& texName);
	static vector<Texture*> getTextures();
	static vector<string> getTextureNames();

private:
	static map<string, Mesh*> _meshes;
	static map<string, ShaderProgram*> _shaderPrograms;
	static map<string, Texture*> _textures;
};