#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();

	void setLocalPosition(const Vector3& newPosition);
	void setLocalRotation(const Vector3& rotation);
	void setLocalRotationAngleX(const float newAngle);
	void setLocalRotationAngleY(const float newAngle);
	void setLocalRotationAngleZ(const float newAngle);
	void setOrbitRotation(const Vector3& rotation);
	void setOrbitRotationAngleX(const float newAngle);
	void setOrbitRotationAngleY(const float newAngle);
	void setOrbitRotationAngleZ(const float newAngle);
	void setLocalScale(const Vector3 newScale);
	void setLocalScaleX(const float x);
	void setLocalScaleY(const float y);
	void setLocalScaleZ(const float z);

	Vector3 getLocalPosition() const;
	Vector3 getLocalRotation() const;
	float getLocalRotationAngleX() const;
	float getLocalRotationAngleY() const;
	float getLocalRotationAngleZ() const;
	Vector3 getOrbitRotation() const;
	float getOrbitRotationAngleX() const;
	float getOrbitRotationAngleY() const;
	float getOrbitRotationAngleZ() const;
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

	string getName() const;
	void setName(const string& name);

private:
	Vector3 _localPosition;
	Vector3 _localRotation;
	Vector3 _orbitRotation;
	Vector3 _scale = Vector3::One;

	Matrix44 _orbitRotationMatrix;
	Matrix44 _localRotationMatrix;
	Matrix44 _localTransformMatrix;
	Matrix44 _localToWorldMatrix;

	TransformComponent* _parent;
	vector<TransformComponent*> _children;

	string _name;
};