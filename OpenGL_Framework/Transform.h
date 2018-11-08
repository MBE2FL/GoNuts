#pragma once

//#include <MiniMath/Core.h>
#include "MathLib/MathLibCore.h" 
#include <string>

class Transform
{
public:
	Transform();

	void setPosition(const Vector3& newPosition);
	void setRotationAngleX(const float newAngle);
	void setRotationAngleY(const float newAngle);
	void setRotationAngleZ(const float newAngle);
	void setScale(const Vector3 newScale);
	void setScaleX(const float _x);
	void setScaleY(const float _y);
	void setScaleZ(const float _z);
	
	Vector3 getPosition() const;
	float getRotationAngleX() const;
	float getRotationAngleY() const;
	float getRotationAngleZ() const;
	Vector3 getScale() const;
	float getScaleX() const;
	float getScaleY() const;
	float getScaleZ() const;

	Matrix44 getLocalToWorldMatrix() const;

	// TODO: Implement this update function
	virtual void update(float dt);

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
	Matrix44 m_pLocalToWorldMatrix;
};