#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "PhysicsBody.h"
#include "Component.h"

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
	float getScale() const;
	void setScale(const float newScale);

	Matrix44 getLocalToWorldMatrix() const;

	virtual void update(float deltaTime);

	void loadShaderProgram(const string& vertFile, const string& fragFile);
	void loadMesh(const string& file);

	void setShaderProgram(ShaderProgram* shaderProgram);
	void setMesh(Mesh* mesh);

	void unLoad();
	void draw(Camera& camera);

	//MeshBounds getMeshBounds() const;
	void addPhysicsBody(const bool _useGravity);
	PhysicsBody* getPhysicsBody() const;
	int checkCollisions(GameObject& other);
	void addForce(const Vector2& force);
	void useGravity(const bool _useGravity);
	void setVelocity(const Vector2& velocity);
	
private:
	Transform* _transform;
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	PhysicsBody* _physicsBody;
};