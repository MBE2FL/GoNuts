#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
//#include "Camera.h"
#include "Component.h"
#include "Transform.h"
#include "Texture.h"
#include "Light.h"
#include "ObjectLoader.h"
#include "Animation.h"


class GameObject
{
public:
	GameObject(const bool createParent = true);
	~GameObject();

	vec3 getLocalPosition() const;
	void setLocalPosition(const vec3& newPosition);
	vec3 getWorldPosition() const;
	void setWorldPosition(const vec3& newPosition);
	mat4 getWorldRotation() const;
	void setWorldRotation(const mat4& newRotation);
	float getLocalRotationAngleX() const;
	void setLocalRotationAngleX(const float newAngle);
	float getLocalRotationAngleY() const;
	void setLocalRotationAngleY(const float newAngle);
	float getLocalRotationAngleZ() const;
	void setLocalRotationAngleZ(const float newAngle);
	vec3 getLocalScale() const;
	void setLocalScale(const vec3 newScale);

	mat4 getLocalToWorldMatrix() const;

	void setWorldRotationAngleX(const float newAngle);
	void setWorldRotationAngleY(const float newAngle);
	void setWorldRotationAngleZ(const float newAngle);

	virtual void update(float deltaTime);

	void loadShaderProgram(const string& vertFile, const string& fragFile);
	void loadMesh(const string& file);

	void setShaderProgram(ShaderProgram* shaderProgram);
	void setMesh(Mesh* mesh);
	void setTexture(Texture* texture);

	void Animated(bool animeted);
	void addMesh(Mesh* mesh);
	void addMesh(const string& meshName, const int totalMeshes);
	Animation getanimation();

	void unLoad();
	//virtual void draw(Camera& camera, Light* light, Light* spotLight, Matrix44& cameraInverse);

	//MeshBounds getMeshBounds() const;


	void addChild(GameObject* child);
	void setParent(GameObject* parent);
	GameObject* getParent() const;
	
	Transform* _transform;
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	vector<Texture*> _texture;

private:

	Animation animation;
	bool isAnimated = false;

	GameObject* _parent;
	vector<GameObject*> _children;
};