#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

Vector3 GameObject::getPosition() const
{
	return _transform.getPosition();
}

void GameObject::setPosition(const Vector3 & newPosition)
{
	_transform.setPosition(newPosition);
}

float GameObject::getRotationAngleX() const
{
	return 0.0f;
}

void GameObject::setRotationAngleX(const float newAngle)
{
	_transform.setRotationAngleX(newAngle);
}

float GameObject::getRotationAngleY() const
{
	return _transform.getRotationAngleY();
}

void GameObject::setRotationAngleY(const float newAngle)
{
	_transform.setRotationAngleY(newAngle);
}

float GameObject::getRotationAngleZ() const
{
	return 0.0f;
}

void GameObject::setRotationAngleZ(const float newAngle)
{
	_transform.setRotationAngleZ(newAngle);
}

float GameObject::getScale() const
{
	return 0.0f;
}

void GameObject::setScale(float newScale)
{
}

Matrix44 GameObject::getLocalToWorldMatrix() const
{
	return _transform.getLocalToWorldMatrix();
}

void GameObject::update(float deltaTime)
{
	_physicsBody.updatePhysicsBody(_transform, deltaTime);
	_transform.update(deltaTime);
}

void GameObject::loadShaderProgram(const string & vertFile, const string & fragFile)
{
	// Load shaders
	if (!_shaderProgram->load("./Assets/Shaders/PassThrough.vert", "./Assets/Shaders/PassThrough.frag"))
	{
		cout << "Shaders failed to initialize." << endl;
		system("pause");
		exit(0);
	}
}

void GameObject::loadMesh(const string & file)
{
	// Load mesh
	if (!_mesh->loadFromFile("./Assets/Models/Monkey.obj"))
	{
		cout << "Model failed to load." << endl;
		system("pause");
		exit(0);
	}
}

void GameObject::setShaderProgram(ShaderProgram * shaderProgram)
{
	_shaderProgram = shaderProgram;
}

void GameObject::setMesh(Mesh * mesh)
{
	_mesh = mesh;
}

void GameObject::unLoad()
{
	_shaderProgram->unLoad();
	_mesh->Unload();
}

void GameObject::draw(Camera& camera)
{
	_shaderProgram->bind();
	_shaderProgram->sendUniformMat4("uModel", getLocalToWorldMatrix().mV, false);
	_shaderProgram->sendUniformMat4("uView", camera.getLocalToWorldMatrix().Inverse().mV, false);
	_shaderProgram->sendUniformMat4("uProj", camera.getProjection().mV, false);

	glBindVertexArray(_mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, _mesh->getNumVertices());
	glBindVertexArray(GL_NONE);

	_shaderProgram->unBind();
}

void GameObject::addPhysicsBody(const bool _useGravity)
{
	_physicsBody = PhysicsBody(_mesh->getMeshBounds());
	_physicsBody.setUseGravity(_useGravity);
}

PhysicsBody GameObject::getPhysicsBody() const
{
	return _physicsBody;
}

bool GameObject::checkCollisions(GameObject& other)
{
	return _physicsBody.collision(other.getPhysicsBody());
}

void GameObject::addForce(const Vector2 & force)
{
	_physicsBody.addForce(force);
}

void GameObject::useGravity(const bool _useGravity)
{
	_physicsBody.setUseGravity(_useGravity);
}

void GameObject::setVelocity(const Vector2 & velocity)
{
	_physicsBody.setVelocity(velocity);
}

//MeshBounds GameObject::getMeshBounds() const
//{
//	return _mesh.getMeshBounds();
//}
