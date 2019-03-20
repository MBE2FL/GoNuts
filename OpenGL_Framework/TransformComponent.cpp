#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	_type = ComponentType::Transform;
}

TransformComponent::~TransformComponent()
{
}

vec3 TransformComponent::getPlayerSpawnPosition() const
{
	return _playerSpawnPosition;
}

void TransformComponent::setPlayerSpawnPosition(const vec3 & newSpawn)
{
	_playerSpawnPosition = newSpawn;
}

void TransformComponent::setLocalPosition(const vec3 & newPosition)
{
	_localPosition = newPosition;
}

void TransformComponent::setLocalRotation(const vec3 & rotation)
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

void TransformComponent::setOrbitRotation(const vec3 & rotation)
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

void TransformComponent::setLocalScale(const vec3 & newScale)
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

vec3 TransformComponent::getLocalPosition() const
{
	return _localPosition;
}

vec3 TransformComponent::getLocalRotation() const
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

vec3 TransformComponent::getOrbitRotation() const
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

vec3 TransformComponent::getLocalScale() const
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

mat4 TransformComponent::getLocalTransformMatrix() const
{
	return _localTransformMatrix;
}

mat4 TransformComponent::getLocalRotationMatrix() const
{
	return _localRotationMatrix;
}

mat4 TransformComponent::getOrbitRotationMatrix() const
{
	return _orbitRotationMatrix;
}

mat4 TransformComponent::getLocalToWorldMatrix() const
{
	return _localToWorldMatrix;
}

mat4 TransformComponent::getView() const
{
	return _localToWorldMatrix.getInverse();
}

vec3 TransformComponent::getWorldPosition() const
{
	if (_parent)
	{
		//vec4 temp = _parent->getLocalToWorldMatrix() * vec4(_localPosition, 1.0f);
		vec4 temp = _parent->getLocalToWorldMatrix() * vec4(getTranslation(), 1.0f);
		return vec3(temp);
	}
	else
		return _localPosition;
}

void TransformComponent::setWorldPosition(const vec3 & newPosition)
{
	if (_parent)
		_parent->setWorldPosition(newPosition);
	else
		_localPosition = newPosition;
}

mat4 TransformComponent::getWorldRotation() const
{
	if (_parent)
		return _parent->getWorldRotation() * _localRotationMatrix;
	else
		return _localRotationMatrix;
}

void TransformComponent::setWorldRotation(const mat4 & newRotation)
{
	if (_parent)
		_parent->setWorldRotation(newRotation);
	else
		_localRotationMatrix = newRotation;
}

mat4 TransformComponent::getWorldOrbitRotation() const
{
	if (_parent)
		return _parent->getWorldOrbitRotation() * _orbitRotationMatrix;
	else
		return _orbitRotationMatrix;
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
	mat4 rx;
	mat4 ry;
	mat4 rz;

	rx.rotateX(toRadians(_localRotation.x));
	ry.rotateY(toRadians(_localRotation.y));
	rz.rotateZ(toRadians(_localRotation.z));
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	_localRotationMatrix = rz * ry * rx;

	// Create translation matrix
	mat4 tran;
	tran.translate(_localPosition);
	//std::cout << "Position: X: " << m_pLocalPosition.x << " Y: " << m_pLocalPosition.y << " Z: " << m_pLocalPosition.z << std::endl;

	// Create scale matrix
	mat4 scale;
	scale.scale(_scale);


	rx.rotateX(toRadians(_orbitRotation.x));
	ry.rotateY(toRadians(_orbitRotation.y));
	rz.rotateZ(toRadians(_orbitRotation.z));
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

void TransformComponent::removeChild(TransformComponent * child)
{
	child->setParent(nullptr);

	vector<TransformComponent*>::iterator position = std::find(_children.begin(), _children.end(), child);

	if (position != _children.end())
		_children.erase(position);
}

void TransformComponent::setParent(TransformComponent * parent)
{
	if (parent)
		parent->addChild(this);
	else
		_parent = nullptr;
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

vec3 TransformComponent::getTranslation() const
{
	return _localToWorldMatrix.getTranslation();
}

vec3 TransformComponent::getForward() const
{
	return _localToWorldMatrix.getForward();
}

vec3 TransformComponent::getUp() const
{
	return _localToWorldMatrix.getUp();
}

vec3 TransformComponent::getLeft() const
{
	return _localToWorldMatrix.getLeft();
}

void TransformComponent::setTarget(TransformComponent * target, const vec3 & offset)
{
	_target = target;
	_targetOffset = offset;
}

void TransformComponent::followTarget(const float speed)
{
	if (_target)
		setWorldPosition(lerp(getWorldPosition(), _target->getWorldPosition() - _targetOffset, speed));
}
