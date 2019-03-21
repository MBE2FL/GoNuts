#include "CameraComponent.h"


CameraComponent::CameraComponent()
{
	_type = ComponentType::Camera;
	setPerspective(60.0f, 1.0f, 0.1f, 100.0f);
}

CameraComponent::CameraComponent(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		setPerspective(60.0f, 1.0f, 0.1f, 100.0f);
	else
		setOrthographic(-10.0f, 10.0f, 10.0f, 10.0f, -100.0f, 100.0f);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::setPerspective(const float fovY, const float aspect, const float zNear, const float zFar)
{
	_projType = ProjectionType::Perspective;
	_projection = mat4::PerspectiveProjection(fovY, aspect, zNear, zFar);

	_near = zNear;
	_far = zFar;
	_fov.y = fovY;
	_fov.x = toDegrees(atan(tan(toRadians(_fov.y) * 0.5f) * aspect) * 2.0f);
	_aspectRatio = aspect;
}

void CameraComponent::setOrthographic(const float left, const float right, const float bottom, const float top, const float zNear, const float zFar)
{
	_projType = ProjectionType::Orthographic;
	_projection = mat4::OrthographicProjection(left, right, bottom, top, zNear, zFar);

	_orthoSize = vec4(left, right, top, bottom);
	_aspectRatio = (right - left) / (top - bottom);
	_near = zNear;
	_far = zFar;
}



mat4 CameraComponent::getProjection() const
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

vec4 CameraComponent::getOrthoSize() const
{
	return _orthoSize;
}

vec2 CameraComponent::getFov() const
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
