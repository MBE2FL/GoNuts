#pragma once
#include "ShaderProgram.h"
#include "Mesh.h"
#include <vector>
#include "MathLib/Vector3.h"
#include "ObjectLoader.h"

using std::vector;

struct MorphTarget
{
	Mesh* meshA;
	Mesh* meshB;
};

class Animation
{
public:
	Animation() {};
	~Animation() {};

	void animate(ShaderProgram* shader);
	void addMesh(Mesh* mesh);
	float getT() { return T; };

private:
	vector<Mesh*> meshes;
	vector <MorphTarget> targets;
	float T = 0;
	unsigned int currentIndex = 0;
	MorphTarget currentTarget;
	MorphTarget restartTarget;
};