#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
using MathUtils::toRadians;
using MathUtils::lerp;


class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void setLocalPosition(const vec3& newPosition);
	void setLocalRotation(const vec3& rotation);
	void setLocalRotationAngleX(const float newAngle);
	void setLocalRotationAngleY(const float newAngle);
	void setLocalRotationAngleZ(const float newAngle);
	void setOrbitRotation(const vec3& rotation);
	void setOrbitRotationAngleX(const float newAngle);
	void setOrbitRotationAngleY(const float newAngle);
	void setOrbitRotationAngleZ(const float newAngle);
	void setLocalScale(const vec3& newScale);
	void setLocalScaleX(const float x);
	void setLocalScaleY(const float y);
	void setLocalScaleZ(const float z);

	vec3 getLocalPosition() const;
	vec3 getLocalRotation() const;
	float getLocalRotationAngleX() const;
	float getLocalRotationAngleY() const;
	float getLocalRotationAngleZ() const;
	vec3 getOrbitRotation() const;
	float getOrbitRotationAngleX() const;
	float getOrbitRotationAngleY() const;
	float getOrbitRotationAngleZ() const;
	vec3 getLocalScale() const;
	float getLocalScaleX() const;
	float getLocalScaleY() const;
	float getLocalScaleZ() const;

	mat4 getLocalTransformMatrix() const;
	mat4 getLocalRotationMatrix() const;
	mat4 getOrbitRotationMatrix() const;
	mat4 getLocalToWorldMatrix() const;

	vec3 getWorldPosition() const;
	void setWorldPosition(const vec3& newPosition);
	mat4 getWorldRotation() const;
	void setWorldRotation(const mat4& newRotation);
	mat4 getWorldOrbitRotation() const;
	void setWorldRotationAngleX(const float newAngle);
	void setWorldRotationAngleY(const float newAngle);
	void setWorldRotationAngleZ(const float newAngle);

	void update(float deltaTime);

	void addChild(TransformComponent* child);
	void setParent(TransformComponent* parent);
	TransformComponent* getParent() const;
	vector<TransformComponent*> getChildren() const;

	bool isRoot() const;

	string getName() const;
	void setName(const string& name);

	vec3 getTranslation() const;
	vec3 getForward() const;
	vec3 getUp() const;
	vec3 getLeft() const;

	void setTarget(TransformComponent* target, const vec3& offset = vec3::Zero);
	void followTarget(const float speed);

private:
	vec3 _localPosition;
	vec3 _localRotation;
	vec3 _orbitRotation;
	vec3 _scale = vec3(1.0f);

	mat4 _orbitRotationMatrix;
	mat4 _localRotationMatrix;
	mat4 _localTransformMatrix;
	mat4 _localToWorldMatrix;

	TransformComponent* _parent;
	vector<TransformComponent*> _children;

	string _name;

	TransformComponent* _target = nullptr;
	vec3 _targetOffset;
};