#include "ObjectLoader.h"

map<string, Mesh*> ObjectLoader::_meshes = map<string, Mesh*>();
map<string, ShaderProgram*> ObjectLoader::_shaderPrograms = map<string, ShaderProgram*>();
map<string, Texture*> ObjectLoader::_textures = map<string, Texture*>();

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
}

void ObjectLoader::loadShaderProgram(const string & shaderProgramName, const string & vertFile, const string & fragFile)
{
	// No mesh with shaderProgramName currently exists
	if (_shaderPrograms.count(shaderProgramName) == 0)
	{
		// Load mesh
		ShaderProgram* shaderProgram = new ShaderProgram();;
		// Load shaders
		if (!shaderProgram->load(vertFile, fragFile))
		{
			cout << "Shaders failed to initialize." << endl;
			system("pause");
			exit(0);
		}

		// Store mesh for later use
		_shaderPrograms[shaderProgramName] = shaderProgram;
		return;
	}

	// Mesh already exists with shaderProgramName
	cout << "Shaders with name, " << shaderProgramName << ", already exists." << endl;
	system("pause");
	exit(0);
}

void ObjectLoader::loadMesh(const string & meshName, const string & file)
{
	// No mesh with meshName currently exists
	if (_meshes.count(meshName) == 0)
	{
		// Load mesh
		Mesh* mesh = new Mesh();
		if (!mesh->loadFromFile(file))
		{
			cout << "Model failed to load." << endl;
			system("pause");
			exit(0);
		}

		// Store mesh for later use
		_meshes[meshName] = mesh;
		return;
	}

	// Mesh already exists with meshName
	cout << "Model with name, " << meshName << ", already exists." << endl;
	system("pause");
	exit(0);
}

void ObjectLoader::loadMesh(const string & meshName, const string & file, const int totalObjs)
{
	for (int i = 1; i <= totalObjs; i++)
	{
		if (i < 10)
			ObjectLoader::loadMesh(meshName + std::to_string(i) ,file + "00000" + std::to_string(i) + ".obj");
		else
			ObjectLoader::loadMesh(meshName + std::to_string(i), file + "0000" + std::to_string(i) + ".obj");
	}
}

void ObjectLoader::loadTexture(const string & texName, const string & file)
{
	// No mesh with shaderProgramName currently exists
	if (_textures.count(texName) == 0)
	{
		// Load mesh
		Texture* texture = new Texture();;
		// Load shaders
		if (!texture->load(file))
		{
			cout << "Shaders failed to initialize." << endl;
			system("pause");
			exit(0);
		}

		// Store mesh for later use
		_textures[texName] = texture;
		return;
	}

	// Mesh already exists with shaderProgramName
	cout << "Shaders with name, " << texName << ", already exists." << endl;
	system("pause");
	exit(0);
}

ShaderProgram * ObjectLoader::getShaderProgram(const string & shaderProgramName)
{
	return _shaderPrograms[shaderProgramName];
}

Mesh * ObjectLoader::getMesh(const string & meshName)
{
	return _meshes[meshName];
}

Texture * ObjectLoader::getTexture(const string & texName)
{
	return _textures[texName];
}