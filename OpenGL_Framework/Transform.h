#pragma once

//#include <MiniMath/Core.h>
#include "MathLib/MathLibCore.h" 
#include <string>
#include <vector>

using std::vector;

class Transform
{
public:
	Transform();

	void setLocalPosition(const Vector3& newPosition);
	void setLocalRotationAngleX(const float newAngle);
	void setLocalRotationAngleY(const float newAngle);
	void setLocalRotationAngleZ(const float newAngle);
	void setLocalScale(const Vector3 newScale);
	void setLocalScaleX(const float _x);
	void setLocalScaleY(const float _y);
	void setLocalScaleZ(const float _z);
	
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

	// TODO: Implement this update function
	virtual void update(float dt);


	void addChild(Transform* child);
	void setParent(Transform* parent);
	Transform* getParent() const;

private:
	// Other Properties
	std::string name;
	Vector4 color;

	Vector3 m_pScale = 1.0f;
	float m_pRotX = 0.0f;
	float m_pRotY = 0.0f;
	float m_pRotZ = 0.0f;

	Vector3 m_pLocalPosition;

	// TODO: Compute these matrices in the Update function
	Matrix44 m_pLocalRotation;
	Matrix44 m_pLocalTransformMatrix;
	Matrix44 m_pLocalToWorldMatrix;


	Transform* _parent;
	vector<Transform*> _children;
};