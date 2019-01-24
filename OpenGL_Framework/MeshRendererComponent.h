#pragma once

#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Animation.h"

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
	void setTexture(unsigned int index, Texture* texture);
	void addTexture(Texture* texture);
	vector<Texture*> getTextures() const;
	void removeTexture(Texture* texture);
	void removeTexture(unsigned int index);
	bool getIsTransparent() const;
	void setIsTransparent(const bool transparent);
	bool getIsAnimated() const;
	void setIsAnimated(const bool animated);
	Animation* getAnimation() const;
	void setAnimation(Animation* animation);
	void addMesh(const string & meshName, const int totalMeshes);


private:
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	vector<Texture*> _textures;
	Animation* _animation = nullptr;
	bool _isTransparent = false;
	bool _isAnimated = false;
};