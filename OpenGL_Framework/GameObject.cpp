#include "GameObject.h"

GameObject::GameObject()
{
	_transform = new Transform();
	_mesh = nullptr;
	_shaderProgram = nullptr;
	_physicsBody = nullptr;
}

GameObject::~GameObject()
{
}

Vector3 GameObject::getPosition() const
{
	return _transform->getPosition();
}

void GameObject::setPosition(const Vector3 & newPosition)
{
	_transform->setPosition(newPosition);
}

float GameObject::getRotationAngleX() const
{
	return 0.0f;
}

void GameObject::setRotationAngleX(const float newAngle)
{
	_transform->setRotationAngleX(newAngle);
}

float GameObject::getRotationAngleY() const
{
	return _transform->getRotationAngleY();
}

void GameObject::setRotationAngleY(const float newAngle)
{
	_transform->setRotationAngleY(newAngle);
}

float GameObject::getRotationAngleZ() const
{
	return 0.0f;
}

void GameObject::setRotationAngleZ(const float newAngle)
{
	_transform->setRotationAngleZ(newAngle);
}

Vector3 GameObject::getScale() const
{
	return _transform->getScale();
}

void GameObject::setScale(Vector3 newScale)
{
	_transform->setScale(newScale);
}

Matrix44 GameObject::getLocalToWorldMatrix() const
{
	return _transform->getLocalToWorldMatrix();
}

void GameObject::update(float deltaTime)
{
	if (_physicsBody)
		_physicsBody->updatePhysicsBody(_transform, deltaTime);

	_transform->update(deltaTime);
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

void GameObject::setTexture(Texture * texture)
{
	_texture = texture;
}

void GameObject::unLoad()
{
	_shaderProgram->unLoad();
	_mesh->Unload();
}

void GameObject::draw(Camera& camera, Light* light)
{
	_shaderProgram->bind();
	_shaderProgram->sendUniformMat4("uModel", getLocalToWorldMatrix().mV, false);
	_shaderProgram->sendUniformMat4("uView", camera.getLocalToWorldMatrix().GetInverse().mV, false);
	_shaderProgram->sendUniformMat4("uProj", camera.getProjection().mV, false);


	_shaderProgram->sendUniform("uTex", 0);


	_shaderProgram->sendUniform("lightPosition", camera.getLocalToWorldMatrix().GetInverse() * Vector4(light->getPosition(), 1.0f));
	_shaderProgram->sendUniform("lightAmbient", light->getAmbient());
	_shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
	_shaderProgram->sendUniform("lightSpecular", light->getSpecular());
	_shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
	_shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
	_shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
	_shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());

	_texture->bind();

	glBindVertexArray(_mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, _mesh->getNumVertices());
	glBindVertexArray(GL_NONE);

	_texture->unBind();

	_shaderProgram->unBind();
}

void GameObject::addPhysicsBody(const bool _useGravity)
{
	_physicsBody = new PhysicsBody(_mesh->getMeshBounds());
	_physicsBody->setUseGravity(_useGravity);
}

PhysicsBody* GameObject::getPhysicsBody() const
{
	return _physicsBody;
}

bool GameObject::checkCollisions(GameObject& other)
{
	if (_physicsBody)
		return _physicsBody->collision(other.getPhysicsBody());

	return false;
}

void GameObject::addForce(const Vector2 & force)
{
	if (_physicsBody)
		_physicsBody->addForce(force);
}

void GameObject::useGravity(const bool _useGravity)
{
	if (_physicsBody)
		_physicsBody->setUseGravity(_useGravity);
}

void GameObject::setVelocity(const Vector2 & velocity)
{
	if (_physicsBody)
		_physicsBody->setVelocity(velocity);
}

//MeshBounds GameObject::getMeshBounds() const
//{
//	return _mesh.getMeshBounds();
//}
