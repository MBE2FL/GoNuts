#include "JointTransform.h"

JointTransform::JointTransform(const vec3 & position, const Quaternion & rotation)
{
	_localPosition = position;
	_localRotation = rotation;
}

JointTransform::~JointTransform()
{
}

mat4 JointTransform::getLocalTransform()
{
	mat4 localTransformMatrix;
	localTransformMatrix.translate(_localPosition);
	localTransformMatrix * _localRotation.getRotationMatrix();

	return localTransformMatrix;
}

JointTransform JointTransform::interpolate(const JointTransform & begin, const JointTransform & end, float interValue)
{
	vec3 position = lerp(begin._localPosition, end._localPosition, interValue);
	Quaternion rotation = Quaternion::slerp(begin._localRotation, end._localRotation, interValue);

	return JointTransform(position, rotation);
}
