#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "PhysicsBody.h"
#include "Component.h"
#include "Texture.h"
#include "Light.h"
#include "ObjectLoader.h"
#include "Animation.h"

enum class Tag
{
	Floor,
	Wall,
	Player
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	Vector3 getPosition() const;
	void setPosition(const Vector3& newPosition);
	float getRotationAngleX() const;
	void setRotationAngleX(const float newAngle);
	float getRotationAngleY() const;
	void setRotationAngleY(const float newAngle);
	float getRotationAngleZ() const;
	void setRotationAngleZ(const float newAngle);
	Vector3 getScale() const;
	void setScale(const Vector3 newScale);

	Matrix44 getLocalToWorldMatrix() const;

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
	virtual void draw(Camera& camera, Light* light, Light* spotLight);

	//MeshBounds getMeshBounds() const;
	void addPhysicsBody(const bool _useGravity);
	PhysicsBody* getPhysicsBody() const;
	bool checkCollisions(GameObject& other);
	bool checkCoinCollisions(GameObject& other);
	void addForce(const Vector2& force);
	void useGravity(const bool _useGravity);
	void setVelocity(const Vector2& velocity);


	void addChild(GameObject* child);
	
	Transform* _transform;
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	PhysicsBody* _physicsBody;
	vector<Texture*> _texture;

private:

	Animation animation;
	bool isAnimated = false;

	GameObject* _parent;
	vector<GameObject*> _children;
};