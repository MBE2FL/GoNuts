#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"
#include <vector>

using std::vector;

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void setLocalPosition(const Vector3& newPosition);
	void setLocalRotationAngleX(const float newAngle);
	void setLocalRotationAngleY(const float newAngle);
	void setLocalRotationAngleZ(const float newAngle);
	void setLocalScale(const Vector3 newScale);
	void setLocalScaleX(const float x);
	void setLocalScaleY(const float y);
	void setLocalScaleZ(const float z);

	Vector3 getLocalPosition() const;
	float getLocalRotationAngleX() const;
	float getLocalRotationAngleY() const;
	float getLocalRotationAngleZ() const;
	Vector3 getLocalScale() const;
	float getLocalScaleX() const;
	float getLocalScaleY() const;
	float getLocalScaleZ() const;

	Matrix44 getLocalToWorldMatrix() const;

	Vector3 getWorldPosition();
	void setWorldPosition(const Vector3& newPosition);
	Matrix44 getWorldRotation();
	void setWorldRotation(const Matrix44& newRotation);
	void setWorldRotationAngleX(const float newAngle);
	void setWorldRotationAngleY(const float newAngle);
	void setWorldRotationAngleZ(const float newAngle);

	void update(float deltaTime);

	void addChild(TransformComponent* child);
	void setParent(TransformComponent* parent);
	TransformComponent* getParent() const;
	vector<TransformComponent*> getChildren() const;

	bool isRoot() const;

private:
	Vector3 _localPosition = Vector3(0.0f);
	float _rotX = 0.0f;
	float _rotY = 0.0f;
	float _rotZ = 0.0f;
	Vector3 _scale = Vector3(1.0f);

	Matrix44 _localRotation;
	Matrix44 _localTransformMatrix;
	Matrix44 _localToWorldMatrix;

	TransformComponent* _parent;
	vector<TransformComponent*> _children;
};