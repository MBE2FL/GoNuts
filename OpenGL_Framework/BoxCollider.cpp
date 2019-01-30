#include "BoxCollider.h"

BoxCollider::BoxCollider(const vec3 & centre, const vec3 & size)
	: Collider()
{
	_bounds = new Bounds(centre, size);

	glGenVertexArrays(1, &_cube_VAO);

	glGenBuffers(1, &_cube_vert_VBO);
	glGenBuffers(1, &_cube_elements_IBO);

	glBindVertexArray(_cube_VAO);


	glEnableVertexAttribArray(0); // Point vertex


	glBindBuffer(GL_ARRAY_BUFFER, _cube_vert_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), reinterpret_cast<void*>(0));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cube_elements_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_cubeElements), _cubeElements, GL_STATIC_DRAW);



	// Cleanup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

BoxCollider::BoxCollider(const Bounds & other)
	: Collider()
{
	_bounds = new Bounds(other);



	glGenVertexArrays(1, &_cube_VAO);

	glGenBuffers(1, &_cube_vert_VBO);
	glGenBuffers(1, &_cube_elements_IBO);

	glBindVertexArray(_cube_VAO);


	glEnableVertexAttribArray(0); // Point vertex


	glBindBuffer(GL_ARRAY_BUFFER, _cube_vert_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), reinterpret_cast<void*>(0));


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cube_elements_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_cubeElements), _cubeElements, GL_STATIC_DRAW);



	// Cleanup
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::update(TransformComponent * transform)
{
	//_centre = _bounds->centre + transform->getWorldPosition();
	_centre = _bounds->centre + transform->getLocalPosition();
	_size = _bounds->size * transform->getLocalScale();


	_extends = _size * 0.5f;
	//_max = _centre + _extends;
	//_min = _centre - _extends;


	_max = _extends;
	_min = -_extends;


	// FRONT
	// bot left
	_vertices[0] = vec3(_min.x, _min.y, _max.z);
	// bot right
	_vertices[1] = vec3(_max.x, _min.y, _max.z);
	// top right
	_vertices[2] = vec3(_max.x, _max.y, _max.z);
	// top left
	_vertices[3] = vec3(_min.x, _max.y, _max.z);

	// BACK
	// bot left
	_vertices[4] = vec3(_min.x, _min.y, _min.z);
	// bot right
	_vertices[5] = vec3(_max.x, _min.y, _min.z);
	// top right
	_vertices[6] = vec3(_max.x, _max.y, _min.z);
	// top left
	_vertices[7] = vec3(_min.x, _max.y, _min.z);



	// Transform the bounding box, so it follows the object properly.
	mat4 _localRotationMatrix = transform->getLocalRotationMatrix();

	// Create translation matrix
	mat4 tran;
	tran.translate(_centre);

	// Create scale matrix
	mat4 scale;
	scale.scale(vec3(1.0f));

	mat4 _orbitRotationMatrix = transform->getOrbitRotationMatrix();

	mat4 _colliderMatrix;

	TransformComponent* parent = transform->getParent();

	if (transform->getParent())
		_colliderMatrix = parent->getLocalToWorldMatrix() * (_orbitRotationMatrix * tran * _localRotationMatrix * scale);
	else
		_colliderMatrix = _orbitRotationMatrix * tran * _localRotationMatrix * scale;


	// Re-calculate bounding box vertices.
	vec4 temp = _colliderMatrix * vec4(_vertices[0], 1.0f);
	_vertices[0] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[1], 1.0f);
	_vertices[1] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[2], 1.0f);
	_vertices[2] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[3], 1.0f);
	_vertices[3] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[4], 1.0f);
	_vertices[4] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[5], 1.0f);
	_vertices[5] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[6], 1.0f);
	_vertices[6] = vec3(temp.x, temp.y, temp.z);

	temp = _colliderMatrix * vec4(_vertices[7], 1.0f);
	_vertices[7] = vec3(temp.x, temp.y, temp.z);


	// Re-calculate bounding box min and max, based on new vertices.
	_max = _vertices[0];
	_min = _vertices[0];

	for (int i = 1; i < 8; ++i)
	{
		_max.x = _max.x > _vertices[i].x ? _max.x : _vertices[i].x;
		_max.y = _max.y > _vertices[i].y ? _max.y : _vertices[i].y;
		_max.z = _max.z > _vertices[i].z ? _max.z : _vertices[i].z;

		_min.x = _min.x < _vertices[i].x ? _min.x : _vertices[i].x;
		_min.y = _min.y < _vertices[i].y ? _min.y : _vertices[i].y;
		_min.z = _min.z < _vertices[i].z ? _min.z : _vertices[i].z;
	}
}

void BoxCollider::draw()
{
	glBindVertexArray(_cube_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _cube_vert_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

	//vec4 colour = vec4(1.0f, 0.0f, 0.0f, 0.3f);
	//glBindBuffer(GL_ARRAY_BUFFER, _cube_colour_VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, &colour, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _cube_elements_IBO);
	//glDrawElements(GL_TRIANGLES, sizeof(_vertices) / sizeof(vec3), GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(GL_NONE);
}

const vec3 * BoxCollider::getVertices() const
{
	return _vertices;
}
