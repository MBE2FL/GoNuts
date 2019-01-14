#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	_type = ComponentType::Transform;
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::setLocalPosition(const Vector3 & newPosition)
{
	_localPosition = newPosition;
}

void TransformComponent::setLocalRotation(const Vector3 & rotation)
{
	_localRotation = rotation;
}

void TransformComponent::setLocalRotationAngleX(const float newAngle)
{
	_localRotation.x = newAngle;
}

void TransformComponent::setLocalRotationAngleY(const float newAngle)
{
	_localRotation.y = newAngle;
}

void TransformComponent::setLocalRotationAngleZ(const float newAngle)
{
	_localRotation.z = newAngle;
}

void TransformComponent::setOrbitRotation(const Vector3 & rotation)
{
	_orbitRotation = rotation;
}

void TransformComponent::setOrbitRotationAngleX(const float newAngle)
{
	_orbitRotation.x = newAngle;
}

void TransformComponent::setOrbitRotationAngleY(const float newAngle)
{
	_orbitRotation.y = newAngle;
}

void TransformComponent::setOrbitRotationAngleZ(const float newAngle)
{
	_orbitRotation.z = newAngle;
}

void TransformComponent::setLocalScale(const Vector3 newScale)
{
	_scale = newScale;
}

void TransformComponent::setLocalScaleX(const float x)
{
	_scale.x = x;
}

void TransformComponent::setLocalScaleY(const float y)
{
	_scale.y = y;
}

void TransformComponent::setLocalScaleZ(const float z)
{
	_scale.z = z;
}

Vector3 TransformComponent::getLocalPosition() const
{
	return _localPosition;
}

Vector3 TransformComponent::getLocalRotation() const
{
	return _localRotation;
}

float TransformComponent::getLocalRotationAngleX() const
{
	return _localRotation.x;
}

float TransformComponent::getLocalRotationAngleY() const
{
	return _localRotation.y;
}

float TransformComponent::getLocalRotationAngleZ() const
{
	return _localRotation.z;
}

Vector3 TransformComponent::getOrbitRotation() const
{
	return _orbitRotation;
}

float TransformComponent::getOrbitRotationAngleX() const
{
	return _orbitRotation.x;
}

float TransformComponent::getOrbitRotationAngleY() const
{
	return _orbitRotation.y;
}

float TransformComponent::getOrbitRotationAngleZ() const
{
	return _orbitRotation.z;
}

Vector3 TransformComponent::getLocalScale() const
{
	return _scale;
}

float TransformComponent::getLocalScaleX() const
{
	return _scale.x;
}

float TransformComponent::getLocalScaleY() const
{
	return _scale.y;
}

float TransformComponent::getLocalScaleZ() const
{
	return _scale.z;
}

Matrix44 TransformComponent::getLocalToWorldMatrix() const
{
	return _localToWorldMatrix;
}

Vector3 TransformComponent::getWorldPosition()
{
	if (_parent)
	{
		Vector4 temp = _parent->getLocalToWorldMatrix() * Vector4(_localPosition, 1.0f);
		return Vector3(temp.x, temp.y, temp.z);
	}
	else
		return _localPosition;
}

void TransformComponent::setWorldPosition(const Vector3 & newPosition)
{
	if (_parent)
		_parent->setWorldPosition(newPosition);
	else
		_localPosition = newPosition;
}

Matrix44 TransformComponent::getWorldRotation()
{
	if (_parent)
		return _parent->getWorldRotation() * _localRotationMatrix;
	else
		return _localRotationMatrix;
}

void TransformComponent::setWorldRotation(const Matrix44 & newRotation)
{
	if (_parent)
		_parent->setWorldRotation(newRotation);
	else
		_localRotationMatrix = newRotation;
}

void TransformComponent::setWorldRotationAngleX(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleX(newAngle);
	else
		_localRotation.x = newAngle;
}

void TransformComponent::setWorldRotationAngleY(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleY(newAngle);
	else
		_localRotation.y = newAngle;
}

void TransformComponent::setWorldRotationAngleZ(const float newAngle)
{
	if (_parent)
		_parent->setWorldRotationAngleZ(newAngle);
	else
		_localRotation.z = newAngle;
}

void TransformComponent::update(float deltaTime)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	Matrix44 rx;
	Matrix44 ry;
	Matrix44 rz;

	rx.RotateX(MathLibCore::toRadians(_localRotation.x));
	ry.RotateY(MathLibCore::toRadians(_localRotation.y));
	rz.RotateZ(MathLibCore::toRadians(_localRotation.z));
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	_localRotationMatrix = rz * ry * rx;

	// Create translation matrix
	Matrix44 tran;
	tran.Translate(_localPosition);
	//std::cout << "Position: X: " << m_pLocalPosition.x << " Y: " << m_pLocalPosition.y << " Z: " << m_pLocalPosition.z << std::endl;

	// Create scale matrix
	Matrix44 scale;
	scale.Scale(_scale);


	rx.RotateX(MathLibCore::toRadians(_orbitRotation.x));
	ry.RotateY(MathLibCore::toRadians(_orbitRotation.y));
	rz.RotateZ(MathLibCore::toRadians(_orbitRotation.z));
	_orbitRotationMatrix = rz * ry* rx;

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a parent, then we must apply the parent's transform
	_localTransformMatrix = _orbitRotationMatrix * tran * _localRotationMatrix * scale;

	if (_parent)
	{
		//_parent->update(dt);
		_localToWorldMatrix = _parent->getLocalToWorldMatrix() * _localTransformMatrix;
	}
	else
		_localToWorldMatrix = _localTransformMatrix;

	//std::cout << m_pLocalToWorldMatrix[12] << ", " << m_pLocalToWorldMatrix[13] << ", " << m_pLocalToWorldMatrix[14] 
	//	<< ", " << m_pLocalToWorldMatrix[15] << std::endl;


	// Update children
	for (TransformComponent* child : _children)
		child->update(deltaTime);
}

void TransformComponent::addChild(TransformComponent * child)
{
	child->_parent = this;
	_children.push_back(child);
}

void TransformComponent::setParent(TransformComponent * parent)
{
	_parent = parent;
}

TransformComponent * TransformComponent::getParent() const
{
	return _parent;
}

vector<TransformComponent*> TransformComponent::getChildren() const
{
	return _children;
}

bool TransformComponent::isRoot() const
{
	return !_parent;
}

string TransformComponent::getName() const
{
	return _name;
}

void TransformComponent::setName(const string & name)
{
	_name = name;
}
