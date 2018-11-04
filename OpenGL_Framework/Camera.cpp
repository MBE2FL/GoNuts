#include "Camera.h"

Camera::Camera()
{
	perspective(60.0f, 1.0f, 0.1f, 100.0f); 
}

Camera::Camera(ProjectionType projType)
{
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f); 
	else
		orthographic(-10, 10, -10, 10, -100, 100);
}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	projectionType = ProjectionType::Perspective;
	projection = Matrix44::PerspectiveProjection(
		fovy, aspect, 
		zNear, zFar);
}

void Camera::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionType = ProjectionType::Orthographic;
	projection = Matrix44::OrthographicProjection(
		left, right, 
		bottom, top,
		zNear, zFar);
}

Matrix44 Camera::getView() const
{
	return getLocalToWorldMatrix();
}

Matrix44 Camera::getProjection() const
{
	return projection;
}

ProjectionType Camera::getProjType() const
{
	return projectionType;
}
