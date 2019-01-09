#include "CameraComponent.h"

CameraComponent::CameraComponent()
{
	_type = ComponentType::Camera;
	perspective(60.0f, 1.0f, 0.1f, 100.0f);
}

CameraComponent::CameraComponent(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f);
	else
		orthographic(-10.0f, 10.0f, 10.0f, 10.0f, -100.0f, 100.0f);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::perspective(const float fovY, const float aspect, const float zNear, const float zFar)
{
	_projType = ProjectionType::Perspective;
	_projection = Matrix44::PerspectiveProjection(fovY, aspect, zNear, zFar);

	_near = zNear;
	_far = zFar;
	_fov.y = fovY;
	_fov.x = MathLibCore::toDegrees(atan(tan(MathLibCore::toRadians(_fov.y) * 0.5f) * aspect) * 2.0f);
}

void CameraComponent::orthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
	_projType = ProjectionType::Orthographic;
	_projection = Matrix44::OrthographicProjection(left, right, bottom, top, zNear, zFar);

	_orthoSize = Vector4(left, right, top, bottom);
	_aspectRatio = (right - left) / (top - bottom);
	_near = zNear;
	_far = zFar;
}

Matrix44 CameraComponent::getView() const
{
	return Matrix44();
}

Matrix44 CameraComponent::getProjection() const
{
	return _projection;
}

ProjectionType CameraComponent::getProjType() const
{
	return _projType;
}

bool CameraComponent::getCullingActive() const
{
	return _cullingActive;
}

void CameraComponent::setCullingActive(const bool cull)
{
	_cullingActive = cull;
}

Vector4 CameraComponent::getOrthoSize() const
{
	return _orthoSize;
}

Vector2 CameraComponent::getFov() const
{
	return _fov;
}

float CameraComponent::getAspectRatio() const
{
	return _aspectRatio;
}

float CameraComponent::getNear() const
{
	return _near;
}

float CameraComponent::getFar() const
{
	return _far;
}
