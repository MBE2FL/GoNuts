#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "PhysicsBody.h"

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

	vec3 getPosition() const;
	void setPosition(const vec3& newPosition);
	float getRotationAngleX() const;
	void setRotationAngleX(const float newAngle);
	float getRotationAngleY() const;
	void setRotationAngleY(const float newAngle);
	float getRotationAngleZ() const;
	void setRotationAngleZ(const float newAngle);
	float getScale() const;
	void setScale(const float newScale);

	mat4 getLocalToWorldMatrix() const;

	virtual void update(float deltaTime);

	void loadShaderProgram(const string& vertFile, const string& fragFile);
	void loadMesh(const string& file);

	void setShaderProgram(ShaderProgram* shaderProgram);
	void setMesh(Mesh* mesh);

	void unLoad();
	void draw(Camera& camera);

	//MeshBounds getMeshBounds() const;
	void addPhysicsBody(const bool _useGravity);
	PhysicsBody getPhysicsBody() const;
	bool checkCollisions(GameObject other);

	
private:
	Transform _transform;
	Mesh _mesh;
	ShaderProgram _shaderProgram;
	PhysicsBody _physicsBody;
};