#pragma once

#include "System.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "Light.h"

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(EntityManager* entityManager);
	~MeshRendererSystem();

	void draw(Camera& camera, Light* light, Light* spotLight, Matrix44& cameraInverse);
};