#include "Mesh.h"


Mesh::Mesh(bool instanced)
{
	if (!instanced)
		vao = new VertexArrayObject();
	else
		vao = new InstVertexArrayObject();

	_isInstanced = instanced;
}

Mesh::~Mesh()
{
	delete vao;
	vao = nullptr;
}

//bool Mesh::loadFromFile(const string & path)
//{
//	setFilename(path);
//
//	ifstream file;
//	file.open(path);
//
//	if (!file)
//	{
//		cout << "Could not open the path." << endl;
//		return false;
//	}
//
//	char inputString[CHAR_BUFFER_SIZE];
//
//	// Unique data
//	vector<vec3> vertexData;
//	vector<vec2> textureData;
//	vector<vec3> normalData;
//
//	// Index/face data
//	vector<MeshFace> faceData;
//
//	// OpenGL ready data
//	vector<float> unPackedVertexData;
//	vector<float> unPackedTextureData;
//	vector<float> unPackedNormalData;
//
//	while (!file.eof())
//	{
//		file.getline(inputString, CHAR_BUFFER_SIZE);
//
//		// Found a comment
//		if (strstr(inputString, "#") != nullptr)
//		{
//			continue;
//		}
//		// Found a normal
//		else if (strstr(inputString, "vn") != nullptr)
//		{
//			vec3 temp;
//
//			sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
//			normalData.push_back(temp);
//		}
//		// Found a texture UV
//		else if (strstr(inputString, "vt") != nullptr)
//		{
//			vec2 temp;
//
//			sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
//			textureData.push_back(temp);
//		}
//		// Found a vertex
//		else if (strstr(inputString, "v") != nullptr)
//		{
//			vec3 temp;
//
//			sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
//			vertexData.push_back(temp);
//
//			// Re-compute the min and max points for the mesh bounding box
//			computeMinMax(temp);
//		}
//		// Found a face
//		else if (strstr(inputString, "f") != nullptr)
//		{
//			MeshFace temp;
//
//			sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
//				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
//				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
//				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
//
//			faceData.push_back(temp);
//		}
//	}
//
//	file.close();
//
//	// Create the mesh bounding box for this mesh
//	vec3 size = maxPoint - minPoint;
//	vec3 centre = size * 0.5f;
//	_meshBounds = Bounds(centre, size);
//
//	// Unpack the data
//	for (unsigned i = 0; i < faceData.size(); i++)
//	{
//		for (unsigned j = 0; j < 3; j++)
//		{
//			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x);
//			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
//			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);
//
//			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
//			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);
//
//			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
//			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
//			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);
//		}
//	}
//
//	_numFaces = faceData.size();
//	_numVertices = _numFaces * 3;
//
//	// Send data to OpenGL
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO_Vertices);
//	glGenBuffers(1, &VBO_UVs);
//	glGenBuffers(1, &VBO_Normals);
//	glGenBuffers(1, GL_NONE);
//
//	glBindVertexArray(VAO);
//
//	glEnableVertexAttribArray(0);	// Vertex
//	glEnableVertexAttribArray(1);	// UVs
//	glEnableVertexAttribArray(2);	// Normals
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
//	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
//	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
//	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));
//
//	// Cleanup
//	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
//	glBindVertexArray(GL_NONE);
//
//	vertexData.clear();
//	textureData.clear();
//	normalData.clear();
//	faceData.clear();
//	unPackedVertexData.clear();
//	unPackedTextureData.clear();
//	unPackedNormalData.clear();
//
//	return true;
//}
bool Mesh::loadFromFile(const string & path)
{
	setFilename(path);

	std::ifstream file;
	file.open(path);

	if (!file)
	{
		_IsLoaded = false;
		return false;
	}
	char line[CHAR_BUFFER_SIZE];

	vec3 topCorn; // Bounding box corner
	vec3 btmCorn; // Bounding box corner

	//Unique data
	std::vector<vec3> vertexDataLoad;
	std::vector<vec2> textureDataLoad;
	std::vector<vec3> normalDataLoad;

	// Index/Face data
	std::vector<MeshFace> faceData;

	float x, y, z;
	MeshFace faceTemp;

	while (!file.eof()) // Go to end of path
	{
		file.getline(line, CHAR_BUFFER_SIZE);

		switch (line[0])
		{
		case 'v':
			switch (line[1])
			{
			case ' ':
				std::sscanf(line, "v %f %f %f", &x, &y, &z);
				vertexDataLoad.push_back(vec3(x, y, z));

				computeMinMax(vertexDataLoad.back());
				break;
			case 't':
				std::sscanf(line, "vt %f %f", &x, &y);
				textureDataLoad.push_back(vec2(x, y));
				break;
			case 'n':
				std::sscanf(line, "vn %f %f %f", &x, &y, &z);
				normalDataLoad.push_back(vec3(x, y, z));
				break;
			}
			break;
		case 'f':
			std::sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&faceTemp.vertices[0], &faceTemp.textureUVs[0], &faceTemp.normals[0],
				&faceTemp.vertices[1], &faceTemp.textureUVs[1], &faceTemp.normals[1],
				&faceTemp.vertices[2], &faceTemp.textureUVs[2], &faceTemp.normals[2]);
			faceData.push_back(faceTemp);
			break;
		case 'o':
		case '#':
		case 'm':
		default:
			break;
		}
	}
	file.close();

	//Create the mesh bounding box for this mesh
	vec3 size = maxPoint - minPoint;
	vec3 centre = size * 0.5f;
	_meshBounds = Bounds(centre, size);

	//Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			vec4 data = vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z,
				1.0f);
			dataVertex.push_back(vec4(
				vertexDataLoad[faceData[i].vertices[j] - 1].x,
				vertexDataLoad[faceData[i].vertices[j] - 1].y,
				vertexDataLoad[faceData[i].vertices[j] - 1].z,
				1.0f));

			dataTexture.push_back(vec4(
				textureDataLoad[faceData[i].textureUVs[j] - 1].x,
				textureDataLoad[faceData[i].textureUVs[j] - 1].y,
				0.0f,
				1.0f));

			dataNormal.push_back(vec4(
				normalDataLoad[faceData[i].normals[j] - 1].x,
				normalDataLoad[faceData[i].normals[j] - 1].y,
				normalDataLoad[faceData[i].normals[j] - 1].z,
				1.0f));
		}



	}

	uploadToGPU();

	vertexDataLoad.clear(); //Clear the vectors from RAM now that everything's in the GPU.
	textureDataLoad.clear();
	normalDataLoad.clear();
	faceData.clear();

	return true;
}

void Mesh::uploadToGPU()
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
		vao->addVBO(posAttrib);
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
		vao->addVBO(textureAttrib);
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
		vao->addVBO(normalAttrib);
	}

	if (dataColor.size() > 0)
	{
		VertexBufferData colorAttrib;
		colorAttrib.attributeType = AttributeLocations::COLOR;
		colorAttrib.data = &dataColor[0];
		colorAttrib.sizeOfElement = sizeof(float);
		colorAttrib.elementType = GL_FLOAT;
		colorAttrib.numElementsPerAttribute = 4;
		colorAttrib.numElements = numTris * 3 * colorAttrib.numElementsPerAttribute;
		vao->addVBO(colorAttrib);
	}

	if (!_isInstanced)
		vao->createVAO();
	else
		dynamic_cast<InstVertexArrayObject*>(vao)->createVAO();

	_IsLoaded = true;
}


void Mesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Vertices);
	glDeleteVertexArrays(1, &VAO);

	VBO_Normals = GL_NONE;
	VBO_UVs = GL_NONE;
	VBO_Vertices = GL_NONE;
	VAO = GL_NONE;

	_numFaces = 0;
	_numVertices = 0;
}

unsigned int Mesh::GetNumFaces() const
{
	return _numFaces;
}

unsigned int Mesh::getNumVertices() const
{
	return _numVertices;
}

GLuint Mesh::getVBO_Verts()
{
	return VBO_Vertices;
}

Bounds Mesh::getMeshBounds() const
{
	return _meshBounds;
}

string Mesh::getFilename() const
{
	return _filename;
}

void Mesh::setFilename(const string & filename)
{
	_filename = filename;
}

string Mesh::getName() const
{
	return _name;
}

void Mesh::setName(const string & name)
{
	_name = name;
}

void Mesh::computeMinMax(const vec3 & vertex)
{
	// Found new max x
	if (vertex.x > maxPoint.x)
		maxPoint.x = vertex.x;
	// Found new min x
	else if (vertex.x < minPoint.x)
		minPoint.x = vertex.x;

	// Found new max y
	if (vertex.y > maxPoint.y)
		maxPoint.y = vertex.y;
	// Found new min y
	else if (vertex.y < minPoint.y)
		minPoint.y = vertex.y;

	// Found new max z
	if (vertex.z > maxPoint.z)
		maxPoint.z = vertex.z;
	// Found new min z
	else if (vertex.z < minPoint.z)
		minPoint.z = vertex.z;
}
