#pragma once
#include "Transform.h"

enum ProjectionType
{
	Perspective,
	Orthographic
};

class Camera : public Transform
{
public:	
	Camera();
	Camera(ProjectionType projType);

	void perspective(
		float fovy, float aspect, 
		float zNear, float zFar);

	void orthographic(
		float left, float right,
		float bottom, float top,
		float zNear, float zFar);

	mat4 getView() const;
	mat4 getProjection() const;
	ProjectionType getProjType() const;

private:
	ProjectionType projectionType = ProjectionType::Perspective;
	mat4 projection;
};