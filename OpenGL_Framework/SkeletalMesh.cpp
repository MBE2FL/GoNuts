#include "SkeletalMesh.h"
#include <sstream>

using std::stringstream;
using tinyxml2::XMLDocument;
using tinyxml2::XMLText;
using tinyxml2::XMLError;
using tinyxml2::XMLNode;
using tinyxml2::XMLElement;


struct VertexJointInfo
{
	unsigned int id;
	float weight;
};

SkeletalMesh::SkeletalMesh()
{
	// Create new animator
	_animator = new Animator(this);
	//_rootJoint->calculateInverseBindTransform(mat4::Identity);

	_zyCorrection.rotateX(toRadians(-90.0f));
	_zyCorrectionInverted = _zyCorrection.getInverse();
}

SkeletalMesh::~SkeletalMesh()
{
}

Joint * SkeletalMesh::getRootJoint() const
{
	return _rootJoint;
}

bool SkeletalMesh::loadFromFile(const string & path)
{
	setFilename(path);

	XMLDocument colladeFile;
	XMLError result = colladeFile.LoadFile(path.c_str());
	if (result != 0)
	{
		_IsLoaded = false;
		return false;
	}
	
	XMLNode* rootNode = colladeFile.FirstChildElement("COLLADA");

	//loadGeoTwo(rootNode);
	loadGeoFour(rootNode);
	loadAnimTwo(rootNode);
	//loadJoints(rootNode);
	loadJointsTwo(rootNode);
	loadJointHierarchyTwo(rootNode);


	//ifstream file;
	//file.open(path);

	//// File could not open.
	//if (!file)
	//{
	//	_IsLoaded = false;
	//	return false;
	//}

	//string line;
	//std::getline(file, line);

	//findLine(line, file, "<library_geometries>");
	//loadGeometry(line, file);

	//findLine(line, file, "<library_animations>");
	//loadAnimations(line, file);

	return true;
}

void SkeletalMesh::uploadToGPU()
{
	unsigned int numTris = (unsigned int)(dataVertex.size() / 3);

	if (dataVertex.size() > 0)
	{
		VertexBufferData posAttrib;
		posAttrib.attributeType = AttributeLocations::VERTEX;
		posAttrib.data = &dataVertex[0];
		posAttrib.sizeOfElement = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElementsPerAttribute = 4;
		posAttrib.numElements = numTris * 3 * posAttrib.numElementsPerAttribute;
		vao.addVBO(posAttrib);
	}

	if (dataTexture.size() > 0)
	{
		VertexBufferData textureAttrib;
		textureAttrib.attributeType = AttributeLocations::TEXCOORD;
		textureAttrib.data = &dataTexture[0];
		textureAttrib.sizeOfElement = sizeof(float);
		textureAttrib.elementType = GL_FLOAT;
		textureAttrib.numElementsPerAttribute = 4;
		textureAttrib.numElements = numTris * 3 * textureAttrib.numElementsPerAttribute;
		vao.addVBO(textureAttrib);
	}

	if (dataNormal.size() > 0)
	{
		VertexBufferData normalAttrib;
		normalAttrib.attributeType = AttributeLocations::NORMAL;
		normalAttrib.data = &dataNormal[0];
		normalAttrib.sizeOfElement = sizeof(float);
		normalAttrib.elementType = GL_FLOAT;
		normalAttrib.numElementsPerAttribute = 4;
		normalAttrib.numElements = numTris * 3 * normalAttrib.numElementsPerAttribute;
		vao.addVBO(normalAttrib);
	}

	if (_jointIdsPerVertex.size() > 0)
	{
		VertexBufferData jointIDSAttrib;
		jointIDSAttrib.attributeType = AttributeLocations::JOINT_IDS;
		jointIDSAttrib.data = &_jointIdsPerVertex[0];
		jointIDSAttrib.sizeOfElement = sizeof(int);
		jointIDSAttrib.elementType = GL_INT;
		jointIDSAttrib.numElementsPerAttribute = 4;
		jointIDSAttrib.numElements = numTris * 3 * jointIDSAttrib.numElementsPerAttribute;
		vao.addVBO(jointIDSAttrib);
	}

	
	//vector<vec4> floatTest;
	//for (ivec4 joint : _jointIdsPerVertex)
	//{
	//	vec4 floatJoint = vec4(static_cast<float>(joint.x), static_cast<float>(joint.y),
	//		static_cast<float>(joint.z), static_cast<float>(joint.w));
	//	floatTest.push_back(floatJoint);
	//}

	//if (_jointIdsPerVertex.size() > 0)
	//{
	//	VertexBufferData jointIDSAttrib;
	//	jointIDSAttrib.attributeType = AttributeLocations::JOINT_IDS;
	//	jointIDSAttrib.data = &floatTest[0];
	//	jointIDSAttrib.sizeOfElement = sizeof(float);
	//	jointIDSAttrib.elementType = GL_FLOAT;
	//	jointIDSAttrib.numElementsPerAttribute = 4;
	//	jointIDSAttrib.numElements = numTris * 3 * jointIDSAttrib.numElementsPerAttribute;
	//	vao.addVBO(jointIDSAttrib);
	//}

	if (_jointWeightsPerVertex.size() > 0)
	{
		VertexBufferData jointWeightsAttrib;
		jointWeightsAttrib.attributeType = AttributeLocations::JOINT_WEIGHTS;
		jointWeightsAttrib.data = &_jointWeightsPerVertex[0];
		jointWeightsAttrib.sizeOfElement = sizeof(float);
		jointWeightsAttrib.elementType = GL_FLOAT;
		jointWeightsAttrib.numElementsPerAttribute = 4;
		jointWeightsAttrib.numElements = numTris * 3 * jointWeightsAttrib.numElementsPerAttribute;
		vao.addVBO(jointWeightsAttrib);
	}

	//if (dataVertex.size() > 0)
	//{
	//	VertexBufferData posAttrib;
	//	posAttrib.attributeType = AttributeLocations::VERTEX;
	//	posAttrib.data = &dataVertex[0];
	//	posAttrib.sizeOfElement = sizeof(float);
	//	posAttrib.elementType = GL_FLOAT;
	//	posAttrib.numElementsPerAttribute = 4;
	//	posAttrib.numElements = dataVertex.size() * posAttrib.numElementsPerAttribute;
	//	vao.addVBO(posAttrib);

	//	//IndexBufferData jointIDSIBOAttrib;
	//	//jointIDSIBOAttrib.data = &vertexIndices[0];
	//	//jointIDSIBOAttrib.sizeOfIndex = sizeof(unsigned int);
	//	//jointIDSIBOAttrib.elementType = GL_UNSIGNED_INT;
	//	//jointIDSIBOAttrib.numIndices = vertexIndices.size();
	//	//vao.addIBO(jointIDSIBOAttrib);
	//}

	//if (dataTexture.size() > 0)
	//{
	//	VertexBufferData textureAttrib;
	//	textureAttrib.attributeType = AttributeLocations::TEXCOORD;
	//	textureAttrib.data = &dataTexture[0];
	//	textureAttrib.sizeOfElement = sizeof(float);
	//	textureAttrib.elementType = GL_FLOAT;
	//	textureAttrib.numElementsPerAttribute = 4;
	//	textureAttrib.numElements = dataTexture.size() * textureAttrib.numElementsPerAttribute;
	//	vao.addVBO(textureAttrib);
	//}

	//if (dataNormal.size() > 0)
	//{
	//	VertexBufferData normalAttrib;
	//	normalAttrib.attributeType = AttributeLocations::NORMAL;
	//	normalAttrib.data = &dataNormal[0];
	//	normalAttrib.sizeOfElement = sizeof(float);
	//	normalAttrib.elementType = GL_FLOAT;
	//	normalAttrib.numElementsPerAttribute = 4;
	//	normalAttrib.numElements = dataNormal.size() * normalAttrib.numElementsPerAttribute;
	//	vao.addVBO(normalAttrib);
	//}

	//if (_jointIDS.size() > 0)
	//{
	//	VertexBufferData jointIDSVBOAttrib;
	//	jointIDSVBOAttrib.attributeType = AttributeLocations::JOINT_IDS;
	//	jointIDSVBOAttrib.data = &_jointIDS[0];
	//	jointIDSVBOAttrib.sizeOfElement = sizeof(int);
	//	jointIDSVBOAttrib.elementType = GL_INT;
	//	jointIDSVBOAttrib.numElementsPerAttribute = 4;
	//	jointIDSVBOAttrib.numElements = _jointIDS.size() * jointIDSVBOAttrib.numElementsPerAttribute;
	//	vao.addVBO(jointIDSVBOAttrib);
	//}

	//if (_jointWeights.size() > 0)
	//{
	//	VertexBufferData jointWeightsVBOAttrib;
	//	jointWeightsVBOAttrib.attributeType = AttributeLocations::JOINT_WEIGHTS;
	//	jointWeightsVBOAttrib.data = &_jointWeights[0];
	//	jointWeightsVBOAttrib.sizeOfElement = sizeof(float);
	//	jointWeightsVBOAttrib.elementType = GL_FLOAT;
	//	jointWeightsVBOAttrib.numElementsPerAttribute = 4;
	//	jointWeightsVBOAttrib.numElements = _jointWeights.size() * jointWeightsVBOAttrib.numElementsPerAttribute;
	//	vao.addVBO(jointWeightsVBOAttrib);
	//}


	vao.createVAO();
	_IsLoaded = true;
}

void SkeletalMesh::setAnimation(SAnimation * anim)
{
	_animator->setAnimation(anim);
}

void SkeletalMesh::update(float deltaTime)
{
	_animator->update(deltaTime);
}

vector<mat4> SkeletalMesh::getJointTransforms()
{
	_jointTransforms.clear();
	_jointTransforms.resize(_numOfJoints); // MOVE

	getJointTransformsHelper(_rootJoint);

	return _jointTransforms;
}

unsigned int SkeletalMesh::getNumOfJoints() const
{
	return _numOfJoints;
}

bool SkeletalMesh::loadFromFileFBX(const string & path)
{
	// Create an fbx manager.
	_fbxManager = FbxManager::Create();


	// Specify which elements to import from the fbx file.
	FbxIOSettings* ios = FbxIOSettings::Create(_fbxManager, IOSROOT);
	_fbxManager->SetIOSettings(ios);
	//ios->SetBoolProp(IMP_FBX_ANIMATION, true);

	// Create an fbx importer.
	FbxImporter* importer = FbxImporter::Create(_fbxManager, "");

	// Import fbx file.
	if (!importer->Initialize(path.c_str(), -1, _fbxManager->GetIOSettings()))
	{
		cerr << "FBX file failed to import! " << importer->GetStatus().GetErrorString() << endl;
		system("pause");
		exit(-1);
	}

	// Create an fbx scene.
	FbxScene* fbxScene = FbxScene::Create(_fbxManager, "MyScene");

	// Import the fbx file into the fbx scene.
	importer->Import(fbxScene);


	// Print the nodes of the scene and their attributes recursively.
	FbxNode* rootNode = fbxScene->GetRootNode();
	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); ++i)
			printNode(rootNode->GetChild(i));
	}


	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); ++i)
			loadGeoFBX(rootNode);
	}


	// Free up memory used by the importer.
	importer->Destroy();
	   
	// Free up memory used by the fbx sdk.
	_fbxManager->Destroy();


	return true;
}

void SkeletalMesh::printNode(FbxNode * node)
{
	printTabs();
	const char* nodeName = node->GetName();
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	// Print the contents of the node.
	cout << "\n" <<
		nodeName << "\n" <<
		"Translation: " << translation[0] << " " << translation[1] << " " << translation[2] << "\n" <<
		"Rotation: " << rotation[0] << " " << rotation[1] << " " << rotation[2] << "\n" <<
		"Scale: " << scaling[0] << " " << scaling[1] << " " << scaling[2] << endl;

	numTabs++;

	// Print the node's attributes.
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
		printAttribute(node->GetNodeAttributeByIndex(i));

	// Recursively print the children.
	for (int j = 0; j < node->GetChildCount(); j++)
		printNode(node->GetChild(j));

	numTabs--;
	printTabs();
	printf("\n");
}

void SkeletalMesh::printTabs()
{
	for (int i = 0; i < numTabs; i++)
		printf("\t");
}

FbxString SkeletalMesh::getAttributeTypeName(FbxNodeAttribute::EType type)
{
	switch (type) {
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

void SkeletalMesh::printAttribute(FbxNodeAttribute * pAttribute)
{
	if (!pAttribute) return;

	FbxString typeName = getAttributeTypeName(pAttribute->GetAttributeType());
	FbxString attrName = pAttribute->GetName();
	printTabs();
	// Note: to retrieve the character array of a FbxString, use its Buffer() method.
	cout << typeName.Buffer() << " " << attrName.Buffer() << endl;
}

void SkeletalMesh::loadGeoFBX(FbxNode * node)
{
	vector<vec3> vertexDataLoad;
	FbxNodeAttribute::EType attributeType;

	if (node->GetNodeAttribute() == NULL)
	{
		cout << "NULL Node Attribute\n" << endl;
	}
	else
	{
		attributeType = (node->GetNodeAttribute()->GetAttributeType());
		switch (attributeType)
		{
		case FbxNodeAttribute::eMesh:
		{
			//DisplayMesh(node);
			FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();
			string name = node->GetName();

			FbxVector4* controlPoints = mesh->GetControlPoints();
			int polyCount = mesh->GetPolygonCount();

			for (int i = 0; i < polyCount; ++i)
			{
				int polySize = mesh->GetPolygonSize(i);

				for (int j = 0; j < polySize; ++j)
				{
					int controlPointIndex = mesh->GetPolygonVertex(i, j);
					FbxVector4 vertex = controlPoints[controlPointIndex];
					
					vec3 newVertex = vec3(static_cast<float>(vertex.mData[0]), static_cast<float>(vertex.mData[1]), static_cast<float>(vertex.mData[2]));

					vertexDataLoad.push_back(newVertex);
					computeMinMax(vertexDataLoad.back());
				}
			}


		}
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		loadGeoFBX(node->GetChild(i));
	}
}

void SkeletalMesh::getJointTransformsHelper(Joint * joint)
{
	_jointTransforms[joint->getIndex()] = joint->getAnimatedTransform();

	for (Joint* child : joint->getChildren())
	{
		getJointTransformsHelper(child);
	}
}

void SkeletalMesh::findLine(string & line, ifstream & file, const string & word)
{
	while (line.find(word) == string::npos)
	{
		std::getline(file, line);
	}
}

void SkeletalMesh::getNextLine(string & line, ifstream & file)
{
	std::getline(file, line);
}

const char* SkeletalMesh::findWord(const char * line, ifstream & file, const char * word)
{
	return strstr(line, word);
}

void SkeletalMesh::loadGeometry(string & line, ifstream & file)
{
	//Unique data
	vector<vec3> vertexDataLoad;
	vector<vec2> textureDataLoad;
	vector<vec3> normalDataLoad;


	// Find array of mesh positions.
	findLine(line, file, "<source id=");
	findLine(line, file, "<float_array id=");

	// Load in array of mesh positions as vec3.
	size_t index = line.find('>');
	string subLine = line.substr(index + 1);
	stringstream ss(subLine);
	string word;
	size_t count = 0;
	vec3 vertex;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);
			vertexDataLoad.push_back(vertex);
			computeMinMax(vertexDataLoad.back());
			count = 0;
		}
	}


	// Find array of mesh normals.
	findLine(line, file, "<source id=");
	findLine(line, file, "<float_array id=");

	// Load in array of mesh normals as vec3.
	index = line.find('>');
	subLine = line.substr(index + 1);
	ss = stringstream(subLine);
	count = 0;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);
			normalDataLoad.push_back(vertex);
			count = 0;
		}
	}


	// Find array of mesh uvs.
	findLine(line, file, "<source id=");
	findLine(line, file, "<float_array id=");

	// Load in array of mesh uvs as vec3.
	index = line.find('>');
	subLine = line.substr(index + 1);
	ss = stringstream(subLine);
	count = 0;
	vec2 uv;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			uv.x = stof(word);
		else
		{
			uv.y = stof(word);
			textureDataLoad.push_back(uv);
			count = 0;
		}
	}


	// Find array of mesh triangles.
	findLine(line, file, "<triangles");
	findLine(line, file, "<p>");

	index = line.find('>');
	subLine = line.substr(index + 1);
	ss = stringstream(subLine);
	count = 0;

	// Index/Face data
	vector<MeshFace> faceData;
	MeshFace faceTemp;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			faceTemp.vertices[0] = stoi(word);
		else if (count == 2)
			faceTemp.normals[0] = stoi(word);
		else if (count == 3)
			faceTemp.textureUVs[0] = stoi(word);
		else if (count == 5)
			faceTemp.vertices[1] = stoi(word);
		else if (count == 6)
			faceTemp.normals[1] = stoi(word);
		else if (count == 7)
			faceTemp.textureUVs[1] = stoi(word);
		else if (count == 9)
			faceTemp.vertices[2] = stoi(word);
		else if (count == 10)
			faceTemp.normals[2] = stoi(word);
		else if (count == 11)
		{
			faceTemp.textureUVs[2] = stoi(word);
			faceData.push_back(faceTemp);
		}
		else if (count == 12)
			count = 0;
	}


	//Create the mesh bounding box for this mesh
	vec3 size = maxPoint - minPoint;
	vec3 centre = size * 0.5f;
	_meshBounds = Bounds(centre, size);

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			//vec4 data = vec4(
			//	vertexDataLoad[faceData[i].vertices[j]].x,
			//	vertexDataLoad[faceData[i].vertices[j]].y,
			//	vertexDataLoad[faceData[i].vertices[j]].z,
			//	1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j]].x,
				vertexDataLoad[faceData[i].vertices[j]].y,
				vertexDataLoad[faceData[i].vertices[j]].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j]].x,
				textureDataLoad[faceData[i].textureUVs[j]].y,
				0.0f,
				1.0f));

			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j]].x,
				normalDataLoad[faceData[i].normals[j]].y,
				normalDataLoad[faceData[i].normals[j]].z,
				1.0f));
		}



	}

	uploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();
}

void SkeletalMesh::loadAnimations(string & line, ifstream & file)
{
	KeyFrame* keyFrame = nullptr;
	vector<KeyFrame*> keyFrames;
	JointAnimation* jointAnim = nullptr;
	vector<JointAnimation*> jointAnims;
	

	//findLine(line, file, "<animation id=");
	getNextLine(line, file);

	// Load in all bone animations.
	while (line.find("<animation id="))
	{
		// Ignore any IK or Pole bone animations.
		if ((line.find("IK") != string::npos) || (line.find("Pole") != string::npos))
		{
			findLine(line, file, "</animation>");
			getNextLine(line, file);
			continue;
		}

		// Load in key frame times.
		getNextLine(line, file);
		getNextLine(line, file);

		size_t index = line.find('>');
		string subLine = line.substr(index + 1);
		stringstream ss(subLine);
		string word;
		float time;

		while (std::getline(ss, word, ' '))
		{
			keyFrame = new KeyFrame();
			time = stof(word);
			keyFrame->setStartTime(time);
			keyFrames.push_back(keyFrame);
		}


		// Load in key frame local transformation matrices.
		findLine(line, file, "<source id=");
		getNextLine(line, file);

		index = line.find('>');
		subLine = line.substr(index + 1);
		ss = stringstream(subLine);
		size_t count = 0;
		size_t frameIndex = 0;
		mat4 localTransform;


		while (std::getline(ss, word, ' '))
		{
			++count;

			// Row 1
			if (count == 1)
				localTransform.data[0] = stof(word);
			else if (count == 2)
				localTransform.data[4] = stof(word);
			else if (count == 3)
				localTransform.data[8] = stof(word);
			else if (count == 4)
				localTransform.data[12] = stof(word);
			// Row 2
			else if (count == 5)
				localTransform.data[1] = stof(word);
			else if (count == 6)
				localTransform.data[5] = stof(word);
			else if (count == 7)
				localTransform.data[9] = stof(word);
			else if (count == 8)
				localTransform.data[13] = stof(word);
			// Row 3
			else if (count == 9)
				localTransform.data[2] = stof(word);
			else if (count == 10)
				localTransform.data[6] = stof(word);
			else if (count == 11)
				localTransform.data[10] = stof(word);
			else if (count == 12)
				localTransform.data[14] = stof(word);
			// Row 4
			else if (count == 13)
				localTransform.data[3] = stof(word);
			else if (count == 14)
				localTransform.data[7] = stof(word);
			else if (count == 15)
				localTransform.data[11] = stof(word);
			else if (count == 16)
			{
				localTransform.data[15] = stof(word);

				// Create a joint transform for this key frame's local transformation.
				vec3 position = localTransform.getTranslation();
				Quaternion rotation = Quaternion(localTransform);

				JointTransform* jointTrans = new JointTransform(position, rotation);
				keyFrames[frameIndex]->setJointTransform(jointTrans);

				++frameIndex;
				count = 0;
			}
		}

		jointAnim = new JointAnimation(keyFrames);
		jointAnims.push_back(jointAnim);

		findLine(line, file, "</animation>");
		getNextLine(line, file);
	}

	SAnimation* anim = new SAnimation(keyFrames.back()->getStartTime(), jointAnims);

	_animator->setAnimation(anim);


}

void SkeletalMesh::loadGeoTwo(XMLNode * rootNode)
{
	//Unique data
	vector<vec3> vertexDataLoad;
	vector<vec2> textureDataLoad;
	vector<vec3> normalDataLoad;

	XMLNode* meshNode = rootNode->FirstChildElement("asset")->NextSiblingElement("library_geometries")->FirstChildElement("geometry")
		->FirstChildElement("mesh");

	XMLNode* sourceNode = meshNode->FirstChildElement("source");

	// Find array of mesh positions.
	XMLNode* arrayNode = sourceNode->FirstChildElement("float_array");
	//XMLText* pos = testnode->FirstChild()->ToText();
	//pos->Value();

	// Load in array of mesh positions as vec3.
	stringstream ss(arrayNode->FirstChild()->Value());
	string word;
	size_t count = 0;
	vec3 vertex;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);

			// Apply z to y axis correction.
			//vertex = mat4::transform(_zyCorrection, vec4(vertex, 1.0f));

			vertexDataLoad.push_back(vertex);
			computeMinMax(vertexDataLoad.back());
			count = 0;
		}
	}



	// Find array of mesh normals.
	sourceNode = sourceNode->NextSiblingElement();
	arrayNode = sourceNode->FirstChildElement("float_array");

	// Load in array of mesh normals as vec3.
	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);

			// Apply z to y axis correction.
			//vertex = mat4::transform(_zyCorrection, vec4(vertex, 1.0f));

			normalDataLoad.push_back(vertex);
			count = 0;
		}
	}


	
	// Find array of mesh uvs.
	sourceNode = sourceNode->NextSiblingElement();
	arrayNode = sourceNode->FirstChildElement("float_array");

	// Load in array of mesh uvs as vec3.
	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;
	vec2 uv;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			uv.x = stof(word);
		else
		{
			uv.y = stof(word);
			textureDataLoad.push_back(uv);
			count = 0;
		}
	}



	// Find array of mesh triangles.
	sourceNode = meshNode->LastChildElement();
	arrayNode = sourceNode->LastChild();

	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;

	// Index/Face data
	vector<MeshFace> faceData;
	MeshFace faceTemp;

	while (std::getline(ss, word, ' '))
	{
		++count;

		if (count == 1)
			faceTemp.vertices[0] = stoi(word);
		else if (count == 2)
			faceTemp.normals[0] = stoi(word);
		else if (count == 3)
			faceTemp.textureUVs[0] = stoi(word);
		else if (count == 5)
			faceTemp.vertices[1] = stoi(word);
		else if (count == 6)
			faceTemp.normals[1] = stoi(word);
		else if (count == 7)
			faceTemp.textureUVs[1] = stoi(word);
		else if (count == 9)
			faceTemp.vertices[2] = stoi(word);
		else if (count == 10)
			faceTemp.normals[2] = stoi(word);
		else if (count == 11)
		{
			faceTemp.textureUVs[2] = stoi(word);
			faceData.push_back(faceTemp);
		}
		else if (count == 12)
			count = 0;
	}


	//Create the mesh bounding box for this mesh
	vec3 size = maxPoint - minPoint;
	vec3 centre = size * 0.5f;
	_meshBounds = Bounds(centre, size);

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			//vec4 data = vec4(
			//	vertexDataLoad[faceData[i].vertices[j]].x,
			//	vertexDataLoad[faceData[i].vertices[j]].y,
			//	vertexDataLoad[faceData[i].vertices[j]].z,
			//	1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j]].x,
				vertexDataLoad[faceData[i].vertices[j]].y,
				vertexDataLoad[faceData[i].vertices[j]].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j]].x,
				textureDataLoad[faceData[i].textureUVs[j]].y,
				0.0f,
				1.0f));

			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j]].x,
				normalDataLoad[faceData[i].normals[j]].y,
				normalDataLoad[faceData[i].normals[j]].z,
				1.0f));
		}



	}

	//uploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();
}

void SkeletalMesh::loadGeoFour(tinyxml2::XMLNode * rootNode)
{
	//Unique data
	vector<vec3> vertexDataLoad;
	vector<vec2> textureDataLoad;
	vector<vec3> normalDataLoad;

	XMLNode* meshNode = rootNode->FirstChildElement("asset")->NextSiblingElement("library_geometries")->FirstChildElement("geometry")
		->FirstChildElement("mesh");

	XMLNode* sourceNode = meshNode->FirstChildElement("source");

	// Find array of mesh positions.
	XMLNode* arrayNode = sourceNode->FirstChildElement("float_array");
	//XMLText* pos = testnode->FirstChild()->ToText();
	//pos->Value();

	// Load in array of mesh positions as vec3.
	stringstream ss(arrayNode->FirstChild()->Value());
	string word;
	size_t count = 0;
	vec3 vertex;

	while (std::getline(ss, word, ' '))
	{
		if (word == "")
			continue;

		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);

			// Apply z to y axis correction.
			vertex = mat4::transform(_zyCorrection, vec4(vertex, 1.0f));

			vertexDataLoad.push_back(vertex);
			computeMinMax(vertexDataLoad.back());
			count = 0;
		}
	}



	// Find array of mesh normals.
	sourceNode = sourceNode->NextSiblingElement();
	arrayNode = sourceNode->FirstChildElement("float_array");

	// Load in array of mesh normals as vec3.
	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;

	while (std::getline(ss, word, ' '))
	{
		if (word == "")
			continue;

		++count;

		if (count == 1)
			vertex.x = stof(word);
		else if (count == 2)
			vertex.y = stof(word);
		else
		{
			vertex.z = stof(word);

			// Apply z to y axis correction.
			vertex = mat4::transform(_zyCorrection, vec4(vertex, 1.0f));

			normalDataLoad.push_back(vertex);
			count = 0;
		}
	}



	// Find array of mesh uvs.
	sourceNode = sourceNode->NextSiblingElement();
	arrayNode = sourceNode->FirstChildElement("float_array");

	// Load in array of mesh uvs as vec3.
	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;
	vec2 uv;

	while (std::getline(ss, word, ' '))
	{
		if (word == "")
			continue;

		++count;

		if (count == 1)
			uv.x = stof(word);
		else
		{
			uv.y = stof(word);
			textureDataLoad.push_back(uv);
			count = 0;
		}
	}



	// Find array of mesh triangles.
	sourceNode = meshNode->LastChildElement();
	arrayNode = sourceNode->LastChild();

	ss = stringstream(arrayNode->FirstChild()->Value());
	count = 0;

	// Index/Face data
	vector<MeshFace> faceData;
	MeshFace faceTemp;

	while (std::getline(ss, word, ' '))
	{
		if (word == "")
			continue;

		++count;

		// Vertex One
		if (count == 1)
		{
			faceTemp.vertices[0] = stoi(word);
			vertexIndices.push_back(stoi(word));
		}
		else if (count == 2)
		{
			faceTemp.normals[0] = stoi(word);
		}
		else if (count == 3)
		{
			faceTemp.textureUVs[0] = stoi(word);
		}
		// Vertex Two
		else if (count == 5)
		{
			faceTemp.vertices[1] = stoi(word);
			vertexIndices.push_back(stoi(word));
		}
		else if (count == 6)
		{
			faceTemp.normals[1] = stoi(word);
		}
		else if (count == 7)
		{
			faceTemp.textureUVs[1] = stoi(word);
		}
		// Vertex Three
		else if (count == 9)
		{
			faceTemp.vertices[2] = stoi(word);
			vertexIndices.push_back(stoi(word));
		}
		else if (count == 10)
		{
			faceTemp.normals[2] = stoi(word);
		}
		else if (count == 11)
		{
			faceTemp.textureUVs[2] = stoi(word);
			faceData.push_back(faceTemp);
		}
		else if (count == 12)
			count = 0;
	}


	//Create the mesh bounding box for this mesh
	vec3 size = maxPoint - minPoint;
	vec3 centre = size * 0.5f;
	_meshBounds = Bounds(centre, size);

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			//vec4 data = vec4(
			//	vertexDataLoad[faceData[i].vertices[j]].x,
			//	vertexDataLoad[faceData[i].vertices[j]].y,
			//	vertexDataLoad[faceData[i].vertices[j]].z,
			//	1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j]].x,
				vertexDataLoad[faceData[i].vertices[j]].y,
				vertexDataLoad[faceData[i].vertices[j]].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j]].x,
				textureDataLoad[faceData[i].textureUVs[j]].y,
				0.0f,
				1.0f));

			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j]].x,
				normalDataLoad[faceData[i].normals[j]].y,
				normalDataLoad[faceData[i].normals[j]].z,
				1.0f));
		}



	}

	//uploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();
}

void SkeletalMesh::loadAnimTwo(XMLNode * rootNode)
{
	KeyFrame* keyFrame = nullptr;
	vector<KeyFrame*> keyFrames;
	JointAnimation* jointAnim = nullptr;
	vector<JointAnimation*> jointAnims;
	float duration = 0.0f;


	XMLElement* animNode = rootNode->FirstChildElement()->NextSiblingElement("library_animations")->FirstChildElement();
	//XMLElement* test = animNode->FirstChildElement();
	//test->Value();
	XMLNode* sourceNode;
	XMLNode* arrayNode;
	string name;

	// Load in all bone animations.
	while (animNode)
	{
		name = animNode->FirstAttribute()->Value();
		// Ignore any IK or Pole bone animations.
		if ((name.find("IK") != string::npos) || (name.find("Pole") != string::npos))
		{
			animNode = animNode->NextSiblingElement();
			continue;
		}

		// Load in key frame times.
		sourceNode = animNode->FirstChildElement();
		arrayNode = sourceNode->FirstChildElement();

		stringstream ss(arrayNode->FirstChild()->Value());
		string word;
		float time = 0.0f;

		while (std::getline(ss, word, ' '))
		{
			keyFrame = new KeyFrame();
			time = stof(word);
			keyFrame->setStartTime(time);
			keyFrames.push_back(keyFrame);
		}

		duration = (duration > +time) ? duration : time;

		// Load in key frame local transformation matrices.
		sourceNode = sourceNode->NextSiblingElement();
		arrayNode = sourceNode->FirstChildElement();

		ss = stringstream(arrayNode->FirstChild()->Value());
		size_t count = 0;
		size_t frameIndex = 0;
		mat4 localTransform;


		while (std::getline(ss, word, ' '))
		{
			++count;

			// Row 1
			if (count == 1)
				localTransform.data[0] = stof(word);
			else if (count == 2)
				localTransform.data[4] = stof(word);
			else if (count == 3)
				localTransform.data[8] = stof(word);
			else if (count == 4)
				localTransform.data[12] = stof(word);
			// Row 2
			else if (count == 5)
				localTransform.data[1] = stof(word);
			else if (count == 6)
				localTransform.data[5] = stof(word);
			else if (count == 7)
				localTransform.data[9] = stof(word);
			else if (count == 8)
				localTransform.data[13] = stof(word);
			// Row 3
			else if (count == 9)
				localTransform.data[2] = stof(word);
			else if (count == 10)
				localTransform.data[6] = stof(word);
			else if (count == 11)
				localTransform.data[10] = stof(word);
			else if (count == 12)
				localTransform.data[14] = stof(word);
			// Row 4
			else if (count == 13)
				localTransform.data[3] = stof(word);
			else if (count == 14)
				localTransform.data[7] = stof(word);
			else if (count == 15)
				localTransform.data[11] = stof(word);
			else if (count == 16)
			{
				localTransform.data[15] = stof(word);

				// Apply z to y axis correction.
				if (!jointAnim)
					localTransform = _zyCorrection * localTransform;

				// Create a joint transform for this key frame's local transformation.
				vec3 position = localTransform.getTranslation();
				Quaternion rotation = Quaternion(localTransform);

				JointTransform* jointTrans = new JointTransform(position, rotation);
				keyFrames[frameIndex]->setJointTransform(jointTrans);

				++frameIndex;
				count = 0;
			}
		}

		jointAnim = new JointAnimation(keyFrames);
		jointAnims.push_back(jointAnim);

		string target = animNode->LastChildElement()->Attribute("target");
		target = target.substr(0, target.find('/'));
		jointAnim->setName(target);


		keyFrames.clear();
		animNode = animNode->NextSiblingElement();
	}

	SAnimation* anim = new SAnimation(duration, jointAnims);

	_animator->setAnimation(anim);
}

void SkeletalMesh::loadJoints(XMLNode * rootNode)
{
	XMLElement* controllerNode = rootNode->FirstChildElement()->NextSiblingElement("library_controllers")->FirstChildElement();
	XMLNode* skinNode = controllerNode->FirstChild();
	XMLNode* sourceNode = skinNode->FirstChildElement("source");
	// Find array of joints.
	XMLNode* arrayNode = sourceNode->FirstChild();

	// Load in array of joints.
	stringstream ss(arrayNode->FirstChild()->Value());
	string word;
	unsigned int index = 0;
	//vector<Joint*> joints;
	Joint* joint = nullptr;

	while (std::getline(ss, word, ' '))
	{
		joint = new Joint();
		joint->setIndex(index);
		joint->setName(word);
		_joints.push_back(joint);

		// Check if the current joint is the root joint.
		if (index == 0)
			_rootJoint = joint;

		++index;
	}

	_numOfJoints = _joints.size();

	// Find array of joint bind transforms.
	sourceNode = sourceNode->NextSibling();
	arrayNode = sourceNode->FirstChild();

	// Load in array of joint bind transforms.
	ss = stringstream(arrayNode->FirstChild()->Value());
	unsigned int count = 0;
	index = 0;
	mat4 inverseBindTransform;

	while (std::getline(ss, word, ' '))
	{
		++count;

		// Row 1
		if (count == 1)
			inverseBindTransform.data[0] = stof(word);
		else if (count == 2)
			inverseBindTransform.data[4] = stof(word);
		else if (count == 3)
			inverseBindTransform.data[8] = stof(word);
		else if (count == 4)
			inverseBindTransform.data[12] = stof(word);
		// Row 2
		else if (count == 5)
			inverseBindTransform.data[1] = stof(word);
		else if (count == 6)
			inverseBindTransform.data[5] = stof(word);
		else if (count == 7)
			inverseBindTransform.data[9] = stof(word);
		else if (count == 8)
			inverseBindTransform.data[13] = stof(word);
		// Row 3
		else if (count == 9)
			inverseBindTransform.data[2] = stof(word);
		else if (count == 10)
			inverseBindTransform.data[6] = stof(word);
		else if (count == 11)
			inverseBindTransform.data[10] = stof(word);
		else if (count == 12)
			inverseBindTransform.data[14] = stof(word);
		// Row 4
		else if (count == 13)
			inverseBindTransform.data[3] = stof(word);
		else if (count == 14)
			inverseBindTransform.data[7] = stof(word);
		else if (count == 15)
			inverseBindTransform.data[11] = stof(word);
		else if (count == 16)
		{
			inverseBindTransform.data[15] = stof(word);

			// Apply z to y axis correction.
			if (index == 2 || index == 3)
			{
				//inverseBindTransform = inverseBindTransform * _zyCorrectionInverted;
				mat4 Corr;
				Corr.rotateY(toRadians(180.0f));
				inverseBindTransform = Corr * inverseBindTransform * _zyCorrectionInverted;
			}
			else
			{
				inverseBindTransform = inverseBindTransform * _zyCorrectionInverted;
			}

			//_joints[index]->_loadedInInverseBindTransform = inverseBindTransform;
			_joints[index]->setInverseBindTransform(inverseBindTransform);

			++index;
			count = 0;
		}
	}


	// Find array of joint bind transforms.
	sourceNode = sourceNode->NextSibling();
	arrayNode = sourceNode->FirstChild();

	// Load in array of joint bind transforms.
	ss = stringstream(arrayNode->FirstChild()->Value());
	vector<float> weights;

	while (std::getline(ss, word, ' '))
	{
		weights.push_back(stof(word));
	}



	// Find arrays of joints and skin weights.
	XMLNode* vertexWeightsNode = skinNode->FirstChildElement("vertex_weights");
	XMLNode* vcountNode = vertexWeightsNode->FirstChildElement("vcount");
	XMLNode* vcountArrayNode = vcountNode->FirstChild();
	XMLNode* vNode = vertexWeightsNode->FirstChildElement("v");
	XMLNode* vArrayNode = vNode->FirstChild();

	// Load in array of joints and skin weights.
	stringstream vcountSS(vcountArrayNode->Value());
	stringstream vSS(vArrayNode->Value());

	//vector<int> jointIDS;
	//vector<float> jointWeights;
	vector<VertexJointInfo> jointIDWeights;
	VertexJointInfo info;

	index = 0;


	// Load all the joint/weight pairings in first.
	while (std::getline(vSS, word, ' '))
	{
		++index;

		if (index == 1)
			info.id = stoul(word);
			//jointIDS.push_back(stoul(word));
		else if (index == 2)
		{
			info.weight = weights[stoi(word)];
			jointIDWeights.push_back(info);
			//jointWeights.push_back(weights[stoi(word)]);
			index = 0;
		}
	}

	index = 0;

	ivec4 currVertexJointIDS;
	vec4 currVertexJointWeights;
	vector<VertexJointInfo> adjustedJointIDWeights;
	vector<ivec4> jointIDS;
	vector<vec4> jointWeights;

	// Load in all joints affecting the vertices.
	while (std::getline(vcountSS, word, ' '))
	{
		unsigned int vertexCount = stoul(word);

		// Joint has more than the max of 4 joints affecting it.
		if (vertexCount > 4)
		{
			// Copy all id/weight pairs.
			for (unsigned int i = index; i < (index + vertexCount); ++i)
			{
				adjustedJointIDWeights.push_back(jointIDWeights[i]);
			}


			// Sort list of pairs.
			sort(adjustedJointIDWeights.begin(), adjustedJointIDWeights.end(), 
				[](const VertexJointInfo& a, const VertexJointInfo& b) -> bool
			{
				return (a.weight > b.weight);
			});


			// Normalize weights of the top four pairs.
			float weightTotal = 0.0f;

			for (unsigned int i = 0; i < 4; ++i)
			{
				weightTotal += adjustedJointIDWeights[i].weight;
			}

			float normWeight = 0.0f;
			for (unsigned int i = 0; i < 4; ++i)
			{
				info = adjustedJointIDWeights[i];
				normWeight = info.weight / weightTotal;
				
				if (i == 0)
				{
					currVertexJointIDS.x = info.id;
					currVertexJointWeights.x = normWeight;
				}
				else if (i == 1)
				{
					currVertexJointIDS.y = info.id;
					currVertexJointWeights.y = normWeight;
				}
				else if (i == 2)
				{
					currVertexJointIDS.z = info.id;
					currVertexJointWeights.z = normWeight;
				}
				else if (i == 3)
				{
					currVertexJointIDS.w = info.id;
					currVertexJointWeights.w = normWeight;
				}
			}

			adjustedJointIDWeights.clear();

		}
		// Joint has the max of 4 joints or less affecting it.
		else
		{
			unsigned int insertIndex = 0;
			for (unsigned int i = index; i < (index + vertexCount); ++i)
			{
				info = jointIDWeights[i];

				if (insertIndex == 0)
				{
					currVertexJointIDS.x = info.id;
					currVertexJointWeights.x = info.weight;
				}
				else if (insertIndex == 1)
				{
					currVertexJointIDS.y = info.id;
					currVertexJointWeights.y = info.weight;
				}
				else if (insertIndex == 2)
				{
					currVertexJointIDS.z = info.id;
					currVertexJointWeights.z = info.weight;
				}
				else if (insertIndex == 3)
				{
					currVertexJointIDS.w = info.id;
					currVertexJointWeights.w = info.weight;
				}

				++insertIndex;
			}
		}

		jointIDS.push_back(currVertexJointIDS);
		jointWeights.push_back(currVertexJointWeights);
		currVertexJointIDS = ivec4(0);
		currVertexJointWeights = vec4(0.0f);
		index += vertexCount;
	}


	// Create a joint id and weight for each time the same vertex appears in the mesh.
	for (unsigned int vertexIndex : vertexIndices)
	{
		_jointIdsPerVertex.push_back(jointIDS[vertexIndex]);
		_jointWeightsPerVertex.push_back(jointWeights[vertexIndex]);
	}

	uploadToGPU();
}

void SkeletalMesh::loadJointsTwo(tinyxml2::XMLNode * rootNode)
{
	XMLElement* controllerNode = rootNode->FirstChildElement()->NextSiblingElement("library_controllers")->FirstChildElement();
	XMLNode* skinNode = controllerNode->FirstChild();
	XMLNode* sourceNode = skinNode->FirstChildElement("source");
	// Find array of joints.
	XMLNode* arrayNode = sourceNode->FirstChild();

	// Load in array of joints.
	stringstream ss(arrayNode->FirstChild()->Value());
	string word;
	unsigned int index = 0;
	//vector<Joint*> joints;
	Joint* joint = nullptr;

	while (std::getline(ss, word, ' '))
	{
		joint = new Joint();
		joint->setIndex(index);
		joint->setName(word);
		_joints.push_back(joint);
		_skinJoints[word] = joint;

		// Check if the current joint is the root joint.
		//if (index == 0)
		//	_rootJoint = joint;

		++index;
	}

	_numOfJoints = _joints.size();

	// Find array of joint bind transforms.
	sourceNode = sourceNode->NextSibling();
	arrayNode = sourceNode->FirstChild();

	// Load in array of joint bind transforms.
	ss = stringstream(arrayNode->FirstChild()->Value());
	unsigned int count = 0;
	index = 0;
	mat4 inverseBindTransform;

	while (std::getline(ss, word, ' '))
	{
		++count;

		// Row 1
		if (count == 1)
			inverseBindTransform.data[0] = stof(word);
		else if (count == 2)
			inverseBindTransform.data[4] = stof(word);
		else if (count == 3)
			inverseBindTransform.data[8] = stof(word);
		else if (count == 4)
			inverseBindTransform.data[12] = stof(word);
		// Row 2
		else if (count == 5)
			inverseBindTransform.data[1] = stof(word);
		else if (count == 6)
			inverseBindTransform.data[5] = stof(word);
		else if (count == 7)
			inverseBindTransform.data[9] = stof(word);
		else if (count == 8)
			inverseBindTransform.data[13] = stof(word);
		// Row 3
		else if (count == 9)
			inverseBindTransform.data[2] = stof(word);
		else if (count == 10)
			inverseBindTransform.data[6] = stof(word);
		else if (count == 11)
			inverseBindTransform.data[10] = stof(word);
		else if (count == 12)
			inverseBindTransform.data[14] = stof(word);
		// Row 4
		else if (count == 13)
			inverseBindTransform.data[3] = stof(word);
		else if (count == 14)
			inverseBindTransform.data[7] = stof(word);
		else if (count == 15)
			inverseBindTransform.data[11] = stof(word);
		else if (count == 16)
		{
			inverseBindTransform.data[15] = stof(word);

			// Apply z to y axis correction.
			//if (index == 2 || index == 3)
			//{
				//inverseBindTransform = inverseBindTransform * _zyCorrectionInverted;
				//mat4 Corr;
				//Corr.rotateY(toRadians(180.0f));
				//inverseBindTransform = Corr * inverseBindTransform * _zyCorrectionInverted;
			//}
			//else
			//{
				inverseBindTransform = inverseBindTransform * _zyCorrectionInverted;
			//}

			//_joints[index]->_loadedInInverseBindTransform = inverseBindTransform;
			_joints[index]->setInverseBindTransform(inverseBindTransform);

			++index;
			count = 0;
		}
	}


	// Find array of joint bind transforms.
	sourceNode = sourceNode->NextSibling();
	arrayNode = sourceNode->FirstChild();

	// Load in array of joint bind transforms.
	ss = stringstream(arrayNode->FirstChild()->Value());
	vector<float> weights;

	while (std::getline(ss, word, ' '))
	{
		weights.push_back(stof(word));
	}



	// Find arrays of joints and skin weights.
	XMLNode* vertexWeightsNode = skinNode->FirstChildElement("vertex_weights");
	XMLNode* vcountNode = vertexWeightsNode->FirstChildElement("vcount");
	XMLNode* vcountArrayNode = vcountNode->FirstChild();
	XMLNode* vNode = vertexWeightsNode->FirstChildElement("v");
	XMLNode* vArrayNode = vNode->FirstChild();

	// Load in array of joints and skin weights.
	stringstream vcountSS(vcountArrayNode->Value());
	stringstream vSS(vArrayNode->Value());

	//vector<int> jointIDS;
	//vector<float> jointWeights;
	vector<VertexJointInfo> jointIDWeights;
	VertexJointInfo info;

	index = 0;


	// Load all the joint/weight pairings in first.
	while (std::getline(vSS, word, ' '))
	{
		++index;

		if (index == 1)
			info.id = stoul(word);
		//jointIDS.push_back(stoul(word));
		else if (index == 2)
		{
			info.weight = weights[stoi(word)];
			jointIDWeights.push_back(info);
			//jointWeights.push_back(weights[stoi(word)]);
			index = 0;
		}
	}

	index = 0;

	ivec4 currVertexJointIDS;
	vec4 currVertexJointWeights;
	vector<VertexJointInfo> adjustedJointIDWeights;
	vector<ivec4> jointIDS;
	vector<vec4> jointWeights;

	// Load in all joints affecting the vertices.
	while (std::getline(vcountSS, word, ' '))
	{
		unsigned int vertexCount = stoul(word);

		// Joint has more than the max of 4 joints affecting it.
		if (vertexCount > 4)
		{
			// Copy all id/weight pairs.
			for (unsigned int i = index; i < (index + vertexCount); ++i)
			{
				adjustedJointIDWeights.push_back(jointIDWeights[i]);
			}


			// Sort list of pairs.
			sort(adjustedJointIDWeights.begin(), adjustedJointIDWeights.end(),
				[](const VertexJointInfo& a, const VertexJointInfo& b) -> bool
			{
				return (a.weight > b.weight);
			});


			// Normalize weights of the top four pairs.
			float weightTotal = 0.0f;

			for (unsigned int i = 0; i < 4; ++i)
			{
				weightTotal += adjustedJointIDWeights[i].weight;
			}

			float normWeight = 0.0f;
			for (unsigned int i = 0; i < 4; ++i)
			{
				info = adjustedJointIDWeights[i];
				normWeight = info.weight / weightTotal;

				if (i == 0)
				{
					currVertexJointIDS.x = info.id;
					currVertexJointWeights.x = normWeight;
				}
				else if (i == 1)
				{
					currVertexJointIDS.y = info.id;
					currVertexJointWeights.y = normWeight;
				}
				else if (i == 2)
				{
					currVertexJointIDS.z = info.id;
					currVertexJointWeights.z = normWeight;
				}
				else if (i == 3)
				{
					currVertexJointIDS.w = info.id;
					currVertexJointWeights.w = normWeight;
				}
			}

			adjustedJointIDWeights.clear();

		}
		// Joint has the max of 4 joints or less affecting it.
		else
		{
			unsigned int insertIndex = 0;
			for (unsigned int i = index; i < (index + vertexCount); ++i)
			{
				info = jointIDWeights[i];

				if (insertIndex == 0)
				{
					currVertexJointIDS.x = info.id;
					currVertexJointWeights.x = info.weight;
				}
				else if (insertIndex == 1)
				{
					currVertexJointIDS.y = info.id;
					currVertexJointWeights.y = info.weight;
				}
				else if (insertIndex == 2)
				{
					currVertexJointIDS.z = info.id;
					currVertexJointWeights.z = info.weight;
				}
				else if (insertIndex == 3)
				{
					currVertexJointIDS.w = info.id;
					currVertexJointWeights.w = info.weight;
				}

				++insertIndex;
			}
		}

		jointIDS.push_back(currVertexJointIDS);
		jointWeights.push_back(currVertexJointWeights);
		currVertexJointIDS = ivec4(0);
		currVertexJointWeights = vec4(0.0f);
		index += vertexCount;
	}


	// Create a joint id and weight for each time the same vertex appears in the mesh.
	for (unsigned int vertexIndex : vertexIndices)
	{
		_jointIdsPerVertex.push_back(jointIDS[vertexIndex]);
		_jointWeightsPerVertex.push_back(jointWeights[vertexIndex]);
	}

	uploadToGPU();

	_joints.clear();
}

void SkeletalMesh::loadJointHierarchy(XMLNode * rootNode)
{
	XMLElement* visualSceneNode = rootNode->FirstChildElement()->NextSiblingElement("library_visual_scenes")->FirstChildElement();

	// Find root joint.
	XMLNode* armatureNode = visualSceneNode->FirstChildElement("node")->NextSiblingElement("node");
	XMLElement* jointNode = armatureNode->FirstChildElement("node");

	// Load in joint hierarchy.
	unsigned int index = 0;
	loadJointHierarchyHelper(jointNode, &index);

	// After loading in all the joint's local bind transforms, calculate their inverse bind transforms.
	//_rootJoint->calculateInverseBindTransform(mat4::Identity);
}

void SkeletalMesh::loadJointHierarchyTwo(tinyxml2::XMLNode * rootNode)
{
	XMLElement* visualSceneNode = rootNode->FirstChildElement()->NextSiblingElement("library_visual_scenes")->FirstChildElement();

	// Find root joint.
	XMLElement* armatureNode = visualSceneNode->FirstChildElement("node");
	while (armatureNode && (strcmp(armatureNode->Attribute("id"), "Armature") != 0))
	{
		armatureNode = armatureNode->NextSiblingElement();
	}

	XMLElement* jointNode = armatureNode->FirstChildElement("node");

	// Load in joint hierarchy.
	loadJointHierarchyHelperTwo(jointNode);
}

void SkeletalMesh::loadJointHierarchyHelper(XMLElement * jointNode, unsigned int * index)
{
	Joint* joint = _joints[*index];

	// Make sure skin joint indices and the joint hierarchy indices are the same.
	if (joint->getIndex() != *index)
	{
		cerr << "Skin joint index does not match joint hierarchy index!" << endl;
		system("pause");
		exit(0);
	}

	// Retrieve this joint's name.
	//string jointID = jointNode->FirstAttribute()->Value();
	//joint->setName(jointID);
	//string jointName = jointNode->Attribute("name");

	//// Retrieve this joint's localBindTransform.
	//XMLNode* transformNode = jointNode->FirstChildElement("matrix")->FirstChild();
	//
	//stringstream ss = stringstream(transformNode->Value());
	//size_t count = 0;
	//string word;
	//mat4 localBindTransform;

	//while (std::getline(ss, word, ' '))
	//{
	//	++count;

	//	// Row 1
	//	if (count == 1)
	//		localBindTransform.data[0] = stof(word);
	//	else if (count == 2)
	//		localBindTransform.data[4] = stof(word);
	//	else if (count == 3)
	//		localBindTransform.data[8] = stof(word);
	//	else if (count == 4)
	//		localBindTransform.data[12] = stof(word);
	//	// Row 2
	//	else if (count == 5)
	//		localBindTransform.data[1] = stof(word);
	//	else if (count == 6)
	//		localBindTransform.data[5] = stof(word);
	//	else if (count == 7)
	//		localBindTransform.data[9] = stof(word);
	//	else if (count == 8)
	//		localBindTransform.data[13] = stof(word);
	//	// Row 3
	//	else if (count == 9)
	//		localBindTransform.data[2] = stof(word);
	//	else if (count == 10)
	//		localBindTransform.data[6] = stof(word);
	//	else if (count == 11)
	//		localBindTransform.data[10] = stof(word);
	//	else if (count == 12)
	//		localBindTransform.data[14] = stof(word);
	//	// Row 4
	//	else if (count == 13)
	//		localBindTransform.data[3] = stof(word);
	//	else if (count == 14)
	//		localBindTransform.data[7] = stof(word);
	//	else if (count == 15)
	//		localBindTransform.data[11] = stof(word);
	//	else if (count == 16)
	//	{
	//		localBindTransform.data[15] = stof(word);

	//		// Apply z to y axis correction.
	//		if (*index == 0)
	//			localBindTransform = _zyCorrection * localBindTransform;

	//		joint->setLocalBindTransform(localBindTransform);

	//		count = 0;
	//	}
	//}


	XMLElement* childNode = jointNode->FirstChildElement("node");

	// Go through all of this joint's children.
	while (childNode)
	{
		// This joint has a child.
		++(*index);
		joint->addChild(_joints[*index]);
		loadJointHierarchyHelper(childNode, index);
		childNode = childNode->NextSiblingElement("node");
	}
}

void SkeletalMesh::loadJointHierarchyHelperTwo(tinyxml2::XMLElement * jointNode)
{
	// Retrieve this joint's name.
	string jointID = jointNode->FirstAttribute()->Value();
	

	//Joint* joint = _joints[*index];
	Joint* joint = nullptr;
	 //Joint* joint = _skinJoints[jointID];

	//if (_skinJoints.find(jointID) == _skinJoints.end())
	//{
	//	jointID = jointNode->Attribute("name");

	//	if (_skinJoints.find(jointID) == _skinJoints.end())
	//	{
	//		system("pause");
	//		exit(0);
	//	}
	//}

	if (_skinJoints.find(jointID) == _skinJoints.end())
	{
		string subJointID = jointID.substr(jointID.find('_') + 1, string::npos);

		if (_skinJoints.find(subJointID) == _skinJoints.end())
		{
			system("pause");
			exit(0);
		}
		else
		{
			joint = _skinJoints[subJointID];
			joint->setName(jointID);
		}
	}
	else
		joint = _skinJoints[jointID];


	_joints.push_back(joint);

	if (_joints.size() == 1)
		_rootJoint = joint;


	XMLElement* childNode = jointNode->FirstChildElement("node");

	// Go through all of this joint's children.
	while (childNode)
	{
		// This joint has a child.
		jointID = childNode->FirstAttribute()->Value();

		//if (_skinJoints.find(jointID) == _skinJoints.end())
		//{
		//	jointID = childNode->Attribute("name");

		//	if (_skinJoints.find(jointID) == _skinJoints.end())
		//	{
		//		system("pause");
		//		exit(0);
		//	}
		//}

		if (_skinJoints.find(jointID) == _skinJoints.end())
		{
			jointID = jointID.substr(jointID.find('_') + 1, string::npos);

			if (_skinJoints.find(jointID) == _skinJoints.end())
			{
				system("pause");
				exit(0);
			}
		}

		joint->addChild(_skinJoints[jointID]);

		loadJointHierarchyHelperTwo(childNode);

		childNode = childNode->NextSiblingElement("node");
	}
}
