#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent()
{
	_type = ComponentType::MeshRenderer;
}

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
	if (_isAnimated)
	{
		glBindVertexArray(mesh->VAO);
		glEnableVertexAttribArray(3);	// Vertex
		glBindVertexArray(GL_NONE);
		_animation->addMesh(mesh);
	}
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

void MeshRendererComponent::setTexture(unsigned int index, Texture * texture)
{
	if (index < _textures.size())
		_textures[index] = texture;
	else
		_textures.push_back(texture);
}

void MeshRendererComponent::addTexture(Texture * texture)
{
	_textures.push_back(texture);
}

vector<Texture*> MeshRendererComponent::getTextures() const
{
	return _textures;
}

void MeshRendererComponent::removeTexture(Texture * texture)
{
	vector<Texture*>::iterator it;
	for (it = _textures.begin(); it != _textures.end();)
	{
		if (*it == texture)
		{
			it = _textures.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void MeshRendererComponent::removeTexture(unsigned int index)
{
	_textures.erase(_textures.begin() + index);
}

bool MeshRendererComponent::getIsTransparent() const
{
	return _isTransparent;
}

void MeshRendererComponent::setIsTransparent(const bool transparent)
{
	_isTransparent = transparent;
}

bool MeshRendererComponent::getIsAnimated() const
{
	return _isAnimated;
}

void MeshRendererComponent::setIsAnimated(const bool animated)
{
	_isAnimated = animated;
}

Animation* MeshRendererComponent::getAnimation() const
{
	return _animation;
}

void MeshRendererComponent::setAnimation(Animation * animation)
{
	_animation = animation;
}

void MeshRendererComponent::addMesh(const string & meshName, const int totalMeshes)
{
	for (int i = 2; i <= totalMeshes; i++)
	{
		_animation->addMesh(ObjectLoader::getMesh(meshName + std::to_string(i)));
	}
}
