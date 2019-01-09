#pragma once

#include "System.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Light.h"
#include <algorithm>

using std::sort;
using std::partition;

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(EntityManager* entityManager);
	~MeshRendererSystem();

	void draw(Light* light, Light* spotLight);
	void cull(vector<Entity*>& cullList, vector<Entity*>& objectList);
	void sortMeshes(vector<Entity*>& cullList, const bool isTrans);

private:
	TransformComponent* _cameraTrans = nullptr;
	CameraComponent* _cameraComp = nullptr;
	vector<Entity*> opaqueCullList;
	vector<Entity*> transCullList;
};