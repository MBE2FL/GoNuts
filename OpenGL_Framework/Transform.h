#pragma once

#include <MiniMath/Core.h>
#include <string>

class Transform
{
public:
	Transform();

	void setPosition(const vec3& newPosition);
	void setRotationAngleX(const float newAngle);
	void setRotationAngleY(const float newAngle);
	void setRotationAngleZ(const float newAngle);
	void setScale(const float newScale);

	vec3 getPosition() const;
	float getRotationAngleX() const;
	float getRotationAngleY() const;
	float getRotationAngleZ() const;
	float getScale() const;

	mat4 getLocalToWorldMatrix() const;

	// TODO: Implement this update function
	virtual void update(float dt);

private:
	// Other Properties
	std::string name;
	vec4 color;

	float m_pScale = 1.0f;
	float m_pRotX = 0.0f;
	float m_pRotY = 0.0f;
	float m_pRotZ = 0.0f;

	vec3 m_pLocalPosition;

	// TODO: Compute these matrices in the Update function
	mat4 m_pLocalRotation;
	mat4 m_pLocalToWorldMatrix;
};