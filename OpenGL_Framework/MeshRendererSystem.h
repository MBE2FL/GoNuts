#pragma once

#include "System.h"
#include "MeshRendererComponent.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "Light.h"
#include "LightComponent.h"
#include <algorithm>


using std::sort;
using std::partition;

class MeshRendererSystem : public System
{
public:
	MeshRendererSystem(EntityManager* entityManager);
	~MeshRendererSystem();

	void draw(Light* light, Light* spotLight);
	void cull(vector<MeshRendererComponent*>& cullList, vector<MeshRendererComponent*>& objectList);
	void sortMeshes(vector<MeshRendererComponent*>& cullList);

private:
	TransformComponent* _cameraTrans = nullptr;
	CameraComponent* _cameraComp = nullptr;
	vector<MeshRendererComponent*> _opaqueObjects;
	vector<MeshRendererComponent*> _opaqueCullList;
	vector<MeshRendererComponent*> _transObjects;
	vector<MeshRendererComponent*> _transCullList;

	void drawHelper(const vector<MeshRendererComponent*>& drawList, Light* light, Light* spotLight, mat4& cameraInverse);
};