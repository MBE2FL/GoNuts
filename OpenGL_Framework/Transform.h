#pragma once

//#include <MiniMath/Core.h>
#include "MathLib/MathLibCore.h" 
#include <string>
#include <vector>


using std::vector;
using MathUtils::toRadians;


class Transform
{
public:
	Transform();

	void setLocalPosition(const vec3& newPosition);
	void setLocalRotationAngleX(const float newAngle);
	void setLocalRotationAngleY(const float newAngle);
	void setLocalRotationAngleZ(const float newAngle);
	void setLocalScale(const vec3 newScale);
	void setLocalScaleX(const float _x);
	void setLocalScaleY(const float _y);
	void setLocalScaleZ(const float _z);
	
	vec3 getLocalPosition() const;
	float getLocalRotationAngleX() const;
	float getLocalRotationAngleY() const;
	float getLocalRotationAngleZ() const;
	vec3 getLocalScale() const;
	float getLocalScaleX() const;
	float getLocalScaleY() const;
	float getLocalScaleZ() const;

	mat4 getLocalToWorldMatrix() const;

	vec3 getWorldPosition();
	void setWorldPosition(const vec3& newPosition);
	mat4 getWorldRotation();
	void setWorldRotation(const mat4& newRotation);
	void setWorldRotationAngleX(const float newAngle);
	void setWorldRotationAngleY(const float newAngle);
	void setWorldRotationAngleZ(const float newAngle);

	// TODO: Implement this update function
	virtual void update(float dt);


	void addChild(Transform* child);
	void setParent(Transform* parent);
	Transform* getParent() const;
	vector<Transform*> getChildren() const;

private:
	// Other Properties
	std::string name;
	//vec4 color;

	vec3 m_pScale = vec3(1.0f);
	float m_pRotX = 0.0f;
	float m_pRotY = 0.0f;
	float m_pRotZ = 0.0f;

	vec3 m_pLocalPosition;

	// TODO: Compute these matrices in the Update function
	mat4 m_pLocalRotation;
	mat4 m_pLocalTransformMatrix;
	mat4 m_pLocalToWorldMatrix;


	Transform* _parent;
	vector<Transform*> _children;
};