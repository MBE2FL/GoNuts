#include "Transform.h"
#include <iostream>

Transform::Transform()
{
}

void Transform::setLocalPosition(const vec3& newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setLocalRotationAngleX(const float newAngle)
{
	m_pRotX = newAngle;
}

void Transform::setLocalRotationAngleY(const float newAngle)
{
	m_pRotY = newAngle;
}

void Transform::setLocalRotationAngleZ(const float newAngle)
{
	m_pRotZ = newAngle;
}

void Transform::setLocalScale(const vec3 newScale)
{
	m_pScale = newScale;
}

void Transform::setLocalScaleX(const float _x)
{
	m_pScale.x = _x;
}

void Transform::setLocalScaleY(const float _y)
{
	m_pScale.y = _y;
}

void Transform::setLocalScaleZ(const float _z)
{
	m_pScale.z = _z;
}

vec3 Transform::getLocalPosition() const
{
	return m_pLocalPosition;
}

float Transform::getLocalRotationAngleX() const
{
	return m_pRotX;
}

float Transform::getLocalRotationAngleY() const
{
	return m_pRotY;
}

float Transform::getLocalRotationAngleZ() const
{
	return m_pRotZ;
}

vec3 Transform::getLocalScale() const
{
	return m_pScale;
}

float Transform::getLocalScaleX() const
{
	return m_pScale.x;
}

float Transform::getLocalScaleY() const
{
	return m_pScale.y;
}

float Transform::getLocalScaleZ() const
{
	return m_pScale.z;
}

mat4 Transform::getLocalToWorldMatrix() const
{
	return m_pLocalToWorldMatrix;
}

vec3 Transform::getWorldPosition()
{
	if (_parent)
	{
		vec4 temp = _parent->getLocalToWorldMatrix() * vec4(m_pLocalPosition, 1.0f);
		return vec3(temp.x, temp.y, temp.z);
	}
	else
		return m_pLocalPosition;
}

void Transform::setWorldPosition(const vec3 & newPosition)
{
	if (_parent)
		_parent->setWorldPosition(newPosition);
	else
		m_pLocalPosition = newPosition;
}

mat4 Transform::getWorldRotation()
{
	if (_parent)
		return _parent->getWorldRotation() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

void Transform::setWorldRotation(const mat4 & newRotation)
{
	if (_parent)
		_parent->setWorldRotation(newRotation);
	else
		m_pLocalRotation = newRotation;
}

void Transform::setWorldRotationAngleX(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleX(newAngle);
	else
		m_pRotX = newAngle;
}

void Transform::setWorldRotationAngleY(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleY(newAngle);
	else
		m_pRotY = newAngle;
}

void Transform::setWorldRotationAngleZ(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleZ(newAngle);
	else
		m_pRotZ = newAngle;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	mat4 rx;
	mat4 ry;
	mat4 rz;

	rx.rotateX(toRadians(m_pRotX));
	ry.rotateY(toRadians(m_pRotY));
	rz.rotateZ(toRadians(m_pRotZ));
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran;
	tran.translate(m_pLocalPosition);
	//std::cout << "Position: X: " << m_pLocalPosition.x << " Y: " << m_pLocalPosition.y << " Z: " << m_pLocalPosition.z << std::endl;

	// Create scale matrix 
	mat4 scale;
	scale.scale(m_pScale);

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scale;

	if (_parent)
	{
		//_parent->update(dt);
		m_pLocalToWorldMatrix = _parent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	}
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	//std::cout << m_pLocalToWorldMatrix[12] << ", " << m_pLocalToWorldMatrix[13] << ", " << m_pLocalToWorldMatrix[14] 
	//	<< ", " << m_pLocalToWorldMatrix[15] << std::endl;
}

void Transform::addChild(Transform * child)
{
	child->_parent = this;
	_children.push_back(child);
}

void Transform::setParent(Transform * parent)
{
}

Transform* Transform::getParent() const
{
	return _parent;
}

vector<Transform*> Transform::getChildren() const
{
	return _children;
}
