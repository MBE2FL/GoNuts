#include "Mesh.h"


struct MeshFace
{
	MeshFace()
	{
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;
	}

	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool Mesh::loadFromFile(const string & file)
{
	ifstream input;
	input.open(file);

	if (!input)
	{
		cout << "Could not open the file." << endl;
		return false;
	}

	char inputString[CHAR_BUFFER_SIZE];

	// Unique data
	vector<Vector3> vertexData;
	vector<Vector2> textureData;
	vector<Vector3> normalData;

	// Index/face data
	vector<MeshFace> faceData;

	// OpenGL ready data
	vector<float> unPackedVertexData;
	vector<float> unPackedTextureData;
	vector<float> unPackedNormalData;

	while (!input.eof())
	{
		input.getline(inputString, CHAR_BUFFER_SIZE);

		// Found a comment
		if (strstr(inputString, "#") != nullptr)
		{
			continue;
		}
		// Found a normal
		else if (strstr(inputString, "vn") != nullptr)
		{
			Vector3 temp;

			sscanf(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);
		}
		// Found a texture UV
		else if (strstr(inputString, "vt") != nullptr)
		{
			Vector2 temp;

			sscanf(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);
		}
		// Found a vertex
		else if (strstr(inputString, "v") != nullptr)
		{
			Vector3 temp;

			sscanf(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			vertexData.push_back(temp);

			// Re-compute the min and max points for the mesh bounding box
			computeMinMax(temp);
		}
		// Found a face
		else if (strstr(inputString, "f") != nullptr)
		{
			MeshFace temp;

			sscanf(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
				&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
				&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
				&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);

			faceData.push_back(temp);
		}
	}

	input.close();

	// Create the mesh bounding box for this mesh
	_meshBounds = MeshBounds(minPoint.x, maxPoint.x, minPoint.y, maxPoint.y, minPoint.z, maxPoint.z);

	// Unpack the data
	for (unsigned i = 0; i < faceData.size(); i++)
	{
		for (unsigned j = 0; j < 3; j++)
		{
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);

			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);

			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);
		}
	}

	_numFaces = faceData.size();
	_numVertices = _numFaces * 3;

	// Send data to OpenGL
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_Vertices);
	glGenBuffers(1, &VBO_UVs);
	glGenBuffers(1, &VBO_Normals);

	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);	// Vertex
	glEnableVertexAttribArray(1);	// UVs
	glEnableVertexAttribArray(2);	// Normals

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();
	unPackedVertexData.clear();
	unPackedTextureData.clear();
	unPackedNormalData.clear();

	return true;
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

MeshBounds Mesh::getMeshBounds() const
{
	return _meshBounds;
}

void Mesh::computeMinMax(const Vector3 & vertex)
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
