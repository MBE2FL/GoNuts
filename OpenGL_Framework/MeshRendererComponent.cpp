#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent(Mesh * mesh, ShaderProgram * shaderProgram, vector<Texture*>& textures)
	: _mesh(mesh), _shaderProgram(shaderProgram), _textures(textures)
{
	_type = ComponentType::MeshRenderer;
}

MeshRendererComponent::~MeshRendererComponent()
{
}

Mesh * MeshRendererComponent::getMesh() const
{
	return _mesh;
}

void MeshRendererComponent::setMesh(Mesh * mesh)
{
	_mesh = mesh;
}

ShaderProgram * MeshRendererComponent::getShaderProgram() const
{
	return _shaderProgram;
}

void MeshRendererComponent::setShaderProgram(ShaderProgram * shaderProgram)
{
	_shaderProgram = shaderProgram;
}

Texture * MeshRendererComponent::getTexture(unsigned int index) const
{
	return _textures[index];
}

vector<Texture*> MeshRendererComponent::getTextures() const
{
	return _textures;
}
