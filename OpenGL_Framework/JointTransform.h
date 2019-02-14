#pragma once

#include "MathLib/MathLibCore.h"

using MathUtils::lerp;

class JointTransform
{
public:
	JointTransform(const vec3& position, const Quaternion& rotation);
	~JointTransform();

	mat4 getLocalTransform();
	static JointTransform interpolate(const JointTransform& begin, const JointTransform& end, float interValue);

private:
	vec3 _localPosition;
	Quaternion _localRotation;
};