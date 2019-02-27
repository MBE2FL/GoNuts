#pragma once

#include "Mesh.h"
#include "Animator.h"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>

using std::cerr;
using std::endl;
using std::sort;
using std::unordered_map;

#define MAX_JOINTS 70
#define MAX_WEIGHTS 4


class SkeletalMesh : public Mesh
{
public:
	SkeletalMesh();
	~SkeletalMesh();

	Joint* getRootJoint() const;

	bool loadFromFile(const string& path);
	void uploadToGPU();
	void setAnimation(SAnimation* anim);
	void update(float deltaTime);
	vector<mat4> getJointTransforms();
	unsigned int getNumOfJoints() const;

private:
	Joint* _rootJoint;
	unsigned int _numOfJoints;
	Animator* _animator;


	vector<ivec4> _jointIdsPerVertex;
	vector<vec4> _jointWeightsPerVertex;


	vector<Joint*> _joints;
	vector<mat4> _jointTransforms;


	mat4 _zyCorrection;
	mat4 _zyCorrectionInverted;

	vector<unsigned int> vertexIndices;

	// Unordered joints loaded in from skin joints section.
	unordered_map<string, Joint*> _skinJoints;


	// FBX variables
	FbxManager* _fbxManager;


	void getJointTransformsHelper(Joint* joint);

	void findLine(string& line, ifstream& file, const string& word);
	void getNextLine(string& line, ifstream& file);
	const char* findWord(const char* line, ifstream& file, const char* word);
	void loadGeometry(string& line, ifstream& file);
	void loadAnimations(string& line, ifstream& file);

	void loadGeoTwo(tinyxml2::XMLNode* rootNode);
	void loadGeoFour(tinyxml2::XMLNode* rootNode);
	void loadAnimTwo(tinyxml2::XMLNode* rootNode);
	void loadJoints(tinyxml2::XMLNode* rootNode);
	void loadJointsTwo(tinyxml2::XMLNode* rootNode);
	void loadJointHierarchy(tinyxml2::XMLNode* rootNode);
	void loadJointHierarchyTwo(tinyxml2::XMLNode* rootNode);
	void loadJointHierarchyHelper(tinyxml2::XMLElement* jointNode, unsigned int* index);
	void loadJointHierarchyHelperTwo(tinyxml2::XMLElement* jointNode);
};