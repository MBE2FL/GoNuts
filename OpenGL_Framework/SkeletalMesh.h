#pragma once

#include "Mesh.h"
#include "Animator.h"
#include "tinyxml2.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <fbxsdk.h>

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

	// FBX functions
	bool loadFromFileFBX(const string& path);
	void printNode(FbxNode* node);
	void printTabs();
	FbxString getAttributeTypeName(FbxNodeAttribute::EType type);
	void printAttribute(FbxNodeAttribute* pAttribute);
	int numTabs = 0;
	void loadGeoFBX(FbxNode* node);


	// Source tools SMD file functions
	bool loadFromFileSMD(const string& armaturePath, const string& animPath, const float fps);
	void loadJointsSMD(string& line, ifstream& file);
	void loadBindPosesSMD(string& line, ifstream& file);
	void loadTrianglesSMD(string& line, ifstream& file);
	void createJointAnims(string& line, ifstream& file, unordered_map<size_t, JointAnimation*>& jointAnims);
	void loadAnimSMD(string& line, ifstream& file, const float fps, unordered_map<size_t, JointAnimation*>& jointAnims);

	// Custom NUT file functions
	bool loadFromFileNUT(const string& armaturePath, const string& animPath);
	void loadJointsNUT(string& line, ifstream& file);
	void loadBindPosesNUT(string& line, ifstream& file);
	void loadTrianglesNUT(string& line, ifstream& file);
	void createJointAnimsNUT(string& line, ifstream& file);


	// DEBUG
	Animator* getAnimator() const;

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