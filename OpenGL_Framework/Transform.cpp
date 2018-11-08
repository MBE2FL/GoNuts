#include "Transform.h"
#include <iostream>

Transform::Transform()
	: color(Vector4(1.0f))
{
}

void Transform::setPosition(const Vector3& newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setRotationAngleX(const float newAngle)
{
	m_pRotX = newAngle;
}

void Transform::setRotationAngleY(const float newAngle)
{
	m_pRotY = newAngle;
}

void Transform::setRotationAngleZ(const float newAngle)
{
	m_pRotZ = newAngle;
}

void Transform::setScale(const Vector3 newScale)
{
	m_pScale = newScale;
}

void Transform::setScaleX(const float _x)
{
	m_pScale.x = _x;
}

void Transform::setScaleY(const float _y)
{
	m_pScale.y = _y;
}

void Transform::setScaleZ(const float _z)
{
	m_pScale.z = _z;
}

Vector3 Transform::getPosition() const
{
	return m_pLocalPosition;
}

float Transform::getRotationAngleX() const
{
	return m_pRotX;
}

float Transform::getRotationAngleY() const
{
	return m_pRotY;
}

float Transform::getRotationAngleZ() const
{
	return m_pRotZ;
}

Vector3 Transform::getScale() const
{
	return m_pScale;
}

float Transform::getScaleX() const
{
	return m_pScale.x;
}

float Transform::getScaleY() const
{
	return m_pScale.y;
}

float Transform::getScaleZ() const
{
	return m_pScale.z;
}

Matrix44 Transform::getLocalToWorldMatrix() const
{
	return m_pLocalToWorldMatrix;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix

	Matrix44 rx; 
	Matrix44 ry; 
	Matrix44 rz; 

	rx.RotateX(m_pRotX);
	ry.RotateY(m_pRotY);
	rz.RotateZ(m_pRotZ);
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	Matrix44 tran;
	tran.Translate(m_pLocalPosition);
	//std::cout << "Position: X: " << m_pLocalPosition.x << " Y: " << m_pLocalPosition.y << " Z: " << m_pLocalPosition.z << std::endl;

	// Create scale matrix
	Matrix44 scale; 
	scale.Scale(m_pScale);

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalToWorldMatrix = scale * m_pLocalRotation * tran;

	//std::cout << m_pLocalToWorldMatrix[12] << ", " << m_pLocalToWorldMatrix[13] << ", " << m_pLocalToWorldMatrix[14] 
	//	<< ", " << m_pLocalToWorldMatrix[15] << std::endl;
}