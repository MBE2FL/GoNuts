#include "Animation.h"

void Animation::animate(ShaderProgram* shader)
{	
	T += 0.6f;
	if (T > 1.0f)
	{
		T = 0.0f;
		currentIndex++;
		if (currentIndex >= targets.size())
		{
			currentIndex = 0;
		}
		currentTarget = targets[currentIndex];
	}

	shader->sendUniform("T", T);

	glBindBuffer(GL_ARRAY_BUFFER, currentTarget.meshA->getVBO_Verts());
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, currentTarget.meshB->getVBO_Verts());
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
}

void Animation::addMesh(Mesh * mesh)
{
	if (meshes.size() > 0)
	{
		MorphTarget target;

		target.meshA = meshes.back();
		target.meshB = mesh;
		targets.push_back(target);

		if (meshes.size() == 1)
			currentTarget = targets.front();
	}
	meshes.push_back(mesh);
}
