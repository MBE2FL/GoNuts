#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"
#include "UniformBuffer.h"


using MathUtils::toDegrees;
using MathUtils::toRadians;


enum ProjectionType
{
	Perspective,
	Orthographic
};

class CameraComponent : public Component
{
public:
	CameraComponent();
	CameraComponent(ProjectionType projType);
	~CameraComponent();

	void setPerspective(const float fovY, const float aspect, const float zNear, const float zFar);
	void setOrthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

	mat4 getProjection() const;
	ProjectionType getProjType() const;
	bool getCullingActive() const;
	void setCullingActive(const bool cull);
	vec4 getOrthoSize() const;
	vec2 getFov() const;
	float getAspectRatio() const;
	float getNear() const;
	float getFar() const;
	UniformBuffer* getUBO() const;

private:
	ProjectionType _projType = ProjectionType::Perspective;
	mat4 _projection;
	bool _cullingActive = false;
	vec4 _orthoSize;
	vec2 _fov;
	float _aspectRatio;
	float _near;
	float _far;
	UniformBuffer* _ubo;
};