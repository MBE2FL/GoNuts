#pragma once

#include "Collider.h"
#include <GL/glew.h>

class BoxCollider : public Collider
{
public:
	BoxCollider(const vec3& centre, const vec3& size);
	BoxCollider(const Bounds& other);
	~BoxCollider();

	void update(TransformComponent* transform);
	void draw();
	const vec3* getVertices() const;


private:
	vec3 _centre;
	vec3 _size;
	vec3 _extends;


	GLuint _cube_vert_VBO = GL_NONE;
	GLuint _cube_colour_VBO = GL_NONE;
	GLuint _cube_VAO = GL_NONE;


	GLuint _cube_elements_IBO;

	vec3 _vertices[8] =				// FRONT
	{	vec3(-1.0, -1.0, 1.0),	// bot left
		vec3(1.0, -1.0, 1.0),	// bot right
		vec3(1.0, 1.0, 1.0),	// top right
		vec3(-1.0, 1.0, 1.0),	// top left
									// BACK
		vec3(-1.0, -1.0, -1.0),	// bot left
		vec3(1.0, -1.0, -1.0),	// bot right
		vec3(1.0, 1.0, -1.0),	// top right
		vec3(-1.0, 1.0, -1.0) };// top left

	GLuint _cubeElements[36] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};
};