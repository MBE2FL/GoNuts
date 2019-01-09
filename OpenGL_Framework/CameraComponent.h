#pragma once

#include "Component.h"
#include "MathLib/MathLibCore.h"

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

	void perspective(const float fovY, const float aspect, const float zNear, const float zFar);
	void orthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar);

	Matrix44 getView() const;
	Matrix44 getProjection() const;
	ProjectionType getProjType() const;
	bool getCullingActive() const;
	void setCullingActive(const bool cull);
	Vector4 getOrthoSize() const;
	Vector2 getFov() const;
	float getAspectRatio() const;
	float getNear() const;
	float getFar() const;

private:
	ProjectionType _projType = ProjectionType::Perspective;
	Matrix44 _projection;
	bool _cullingActive = false;
	Vector4 _orthoSize;
	Vector2 _fov;
	float _aspectRatio;
	float _near;
	float _far;
};