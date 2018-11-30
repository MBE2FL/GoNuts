#include "GameObject.h"

GameObject::GameObject(const bool createParent)
{
	_transform = new Transform();
	_mesh = nullptr;
	_shaderProgram = nullptr;
	_physicsBody = nullptr;

	if (createParent)
	{
		GameObject* parent = new GameObject(false);
		parent->addChild(this);
	}
		

}

GameObject::~GameObject()
{
}

Vector3 GameObject::getLocalPosition() const
{
	return _transform->getLocalPosition();
}

void GameObject::setLocalPosition(const Vector3 & newPosition)
{
	_transform->setLocalPosition(newPosition);
}

Vector3 GameObject::getWorldPosition() const
{
	return _transform->getWorldPosition();
}

void GameObject::setWorldPosition(const Vector3 & newPosition)
{
	_transform->setWorldPosition(newPosition);
}

Matrix44 GameObject::getWorldRotation() const
{
	return _transform->getWorldRotation();
}

void GameObject::setWorldRotation(const Matrix44 & newRotation)
{
	_transform->setWorldRotation(newRotation);
}

float GameObject::getLocalRotationAngleX() const
{
	return _transform->getLocalRotationAngleX();
}

void GameObject::setLocalRotationAngleX(const float newAngle)
{
	_transform->setLocalRotationAngleX(newAngle);
}

float GameObject::getLocalRotationAngleY() const
{
	return _transform->getLocalRotationAngleY();
}

void GameObject::setLocalRotationAngleY(const float newAngle)
{
	_transform->setLocalRotationAngleY(newAngle);
}

float GameObject::getLocalRotationAngleZ() const
{
	return _transform->getLocalRotationAngleZ();
}

void GameObject::setLocalRotationAngleZ(const float newAngle)
{
	_transform->setLocalRotationAngleZ(newAngle);
}

Vector3 GameObject::getLocalScale() const
{
	return _transform->getLocalScale();
}

void GameObject::setLocalScale(Vector3 newScale)
{
	_transform->setLocalScale(newScale);
}

Matrix44 GameObject::getLocalToWorldMatrix() const
{
	//if (_parent)
	//	return (_parent->getLocalToWorldMatrix() * _transform->getLocalToWorldMatrix());
	//else
	return _transform->getLocalToWorldMatrix();
}

void GameObject::setWorldRotationAngleX(const float newAngle)
{
	_transform->setWorldRotationAngleX(newAngle);
}

void GameObject::setWorldRotationAngleY(const float newAngle)
{
	_transform->setWorldRotationAngleY(newAngle);
}

void GameObject::setWorldRotationAngleZ(const float newAngle)
{
	_transform->setWorldRotationAngleZ(newAngle);
}

void GameObject::update(float deltaTime)
{
	//if (_transform->getParent())
	//	_transform->getParent()->update(deltaTime);


	if (_physicsBody)
	{
		_physicsBody->updatePhysicsBody(_transform, deltaTime);
		if (_transform->getParent())
			_transform->getParent()->update(deltaTime);
	}
		

	_transform->update(deltaTime);

	for (GameObject* child : _children)
		child->update(deltaTime);
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
	if (isAnimated)
	{
		glBindVertexArray(mesh->VAO);
		glEnableVertexAttribArray(3);	// Vertex
		glBindVertexArray(GL_NONE);
		animation.addMesh(mesh);
	}
}

void GameObject::setTexture(Texture * texture)
{
	_texture.push_back(texture);
}

void GameObject::Animated(bool animeted)
{
	isAnimated = animeted;
}

void GameObject::addMesh(Mesh * mesh)
{
	animation.addMesh(mesh);
}

void GameObject::addMesh(const string & meshName, const int totalMeshes)
{
	for (int i = 2; i <= totalMeshes; i++)
	{
		addMesh(ObjectLoader::getMesh(meshName + std::to_string(i)));
	}
}

Animation GameObject::getanimation()
{
	return animation;
}

void GameObject::unLoad()
{
	_shaderProgram->unLoad();
	_mesh->Unload();
}

void GameObject::draw(Camera& camera, Light* light, Light* spotLight, Matrix44& cameraInverse)
{
	_shaderProgram->bind();
	_shaderProgram->sendUniformMat4("uModel", getLocalToWorldMatrix().mV, false);
	_shaderProgram->sendUniformMat4("uView", cameraInverse.mV, false);
	_shaderProgram->sendUniformMat4("uProj", camera.getProjection().mV, false);

	
	_shaderProgram->sendUniform("uTex", 0);

	
	_shaderProgram->sendUniform("lightPosition", cameraInverse * Vector4(light->getPosition(), 1.0f));
	_shaderProgram->sendUniform("lightAmbient", light->getAmbient());
	_shaderProgram->sendUniform("lightDiffuse", light->getDiffuse());
	_shaderProgram->sendUniform("lightSpecular", light->getSpecular());
	_shaderProgram->sendUniform("lightSpecularExponent", light->getSpecularExp());
	_shaderProgram->sendUniform("attenuationConstant", light->getAttenuationConstant());
	_shaderProgram->sendUniform("attenuationLinear", light->getAttenuationLinear());
	_shaderProgram->sendUniform("attenuationQuadratic", light->getAttenuationQuadratic());

	_shaderProgram->sendUniform("spotLightPosition", cameraInverse * Vector4(spotLight->getPosition(), 1.0f));
	_shaderProgram->sendUniform("spotLightDirection", Vector3(1, 0, 0));
	_shaderProgram->sendUniform("spotLightAmbient", spotLight->getAmbient());
	_shaderProgram->sendUniform("spotLightDiffuse", spotLight->getDiffuse());
	_shaderProgram->sendUniform("spotLightSpecular", spotLight->getSpecular());
	_shaderProgram->sendUniform("spotLightSpecularExponent", spotLight->getSpecularExp());
	_shaderProgram->sendUniform("spotLightattenuationConstant", spotLight->getAttenuationConstant());
	_shaderProgram->sendUniform("spotLightattenuationLinear", spotLight->getAttenuationLinear());
	_shaderProgram->sendUniform("spotLightattenuationQuadratic", spotLight->getAttenuationQuadratic());


	vector<Texture*>::iterator it;
	unsigned int texLoc = 0;
	for (it = _texture.begin(); it != _texture.end(); it++)
	{
		(*it)->bind(texLoc);
		texLoc++;
	}

	glBindVertexArray(_mesh->VAO);

	if (isAnimated)
		animation.animate(_shaderProgram, _physicsBody->getVelocity().x/ 7.5f);

	glDrawArrays(GL_TRIANGLES, 0, _mesh->getNumVertices());
	glBindVertexArray(GL_NONE);

	vector<Texture*>::reverse_iterator revIt;
	for (revIt = _texture.rbegin(); revIt != _texture.rend(); revIt++)
	{
		texLoc--;
		(*revIt)->unBind(texLoc);
	}

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

bool GameObject::checkSpikeCollisions(GameObject & other)
{
	if (_physicsBody)
		return _physicsBody->spikeCollision(other.getPhysicsBody());

	return false;
}

bool GameObject::checkCoinCollisions(GameObject & other)
{
	if (_physicsBody)
		return _physicsBody->coinCollision(other.getPhysicsBody());

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

void GameObject::addChild(GameObject * child)
{
	_transform->addChild(child->_transform);

	child->_parent = this;
	_children.push_back(child);
}

void GameObject::setParent(GameObject * parent)
{
	parent->addChild(this);
	_parent = parent;
}

GameObject * GameObject::getParent() const
{
	return _parent;
}

//MeshBounds GameObject::getMeshBounds() const
//{
//	return _mesh.getMeshBounds();
//}
