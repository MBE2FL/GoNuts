#pragma once

#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

class MeshRendererComponent : public Component
{
public:
	MeshRendererComponent(Mesh* mesh, ShaderProgram* shaderProgram, vector<Texture*>& textures);
	~MeshRendererComponent();

	Mesh* getMesh() const;
	void setMesh(Mesh* mesh);
	ShaderProgram* getShaderProgram() const;
	void setShaderProgram(ShaderProgram* shaderProgram);
	Texture* getTexture(unsigned int index) const;
	vector<Texture*> getTextures() const;
	bool getIsTransparent() const;
	void setIsTransparent(const bool transparent);

private:
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	vector<Texture*> _textures;
	bool _isTransparent = false;
};