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
	void sortMeshes(vector<Entity*>& cullList);

private:
	TransformComponent* _cameraTrans = nullptr;
	CameraComponent* _cameraComp = nullptr;
	vector<Entity*> _opaqueObjects;
	vector<Entity*> _opaqueCullList;
	vector<Entity*> _transObjects;
	vector<Entity*> _transCullList;

	void drawHelper(const vector<Entity*>& drawList, Light* light, Light* spotLight, Matrix44& cameraInverse);
};