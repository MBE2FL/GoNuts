#include "UIImage.h"

UIImage::UIImage(const string & name, const vec3 & pos, const vec3 & scale, const vec3 & localRotation, const float alpha)
{
	_animator = new UIAnimator(this);
	_transform = new TransformComponent();
	_transform->setLocalPosition(pos);
	_transform->setLocalScale(scale);
	_transform->setLocalRotation(localRotation);
	_transform->setName(name);
	_alpha = alpha;
	_mesh = ObjectLoader::getMesh("UIQuad");
	_shaderProgram = ObjectLoader::getShaderProgram("UIShader");
	_name = name;
}

void UIImage::setAnimation(UIAnimation * newAnimation)
{
	_animator->addAnimation(newAnimation);
}

UIAnimator * UIImage::getAnimator()
{
	return _animator;
}

void UIImage::setAnimator(UIAnimator * animator)
{
	_animator = animator;
	_animator->setImage(this);
}

vec3 UIImage::getLocalPosition() const
{
	return _transform->getLocalPosition();
}

void UIImage::setLocalPosition(const vec3 & pos)
{
	_transform->setLocalPosition(pos);
}

vec3 UIImage::getScale() const
{
	return _transform->getLocalScale();
}

void UIImage::setScale(const vec3 & scale)
{
	_transform->setLocalScale(scale);
}

vec3 UIImage::getLocalRotation() const
{
	return _transform->getLocalRotation();
}

void UIImage::setLocalRotation(const vec3 & rotation)
{
	_transform->setLocalRotation(rotation);
}

void UIImage::setLocalRotationX(const float rotation)
{
	_transform->setLocalRotationAngleX(rotation);
}

void UIImage::setLocalRotationY(const float rotation)
{
	_transform->setLocalRotationAngleY(rotation);
}

void UIImage::setLocalRotationZ(const float rotation)
{
	_transform->setLocalRotationAngleZ(rotation);
}

TransformComponent * UIImage::getTransform() const
{
	return _transform;
}

void UIImage::setTransform(TransformComponent * transform)
{
	_transform = transform;
	_transform->setName(_name);
}

float UIImage::getAlpha() const
{
	return _alpha;
}

void UIImage::setAlpha(const float alpha)
{
	_alpha = alpha;
}

Texture * UIImage::getTexture() const
{
	return _texture;
}

void UIImage::setTexture(Texture * texture)
{
	_texture = texture;

	//float size = static_cast<float>((_texture->getSizeX() > _texture->getSizeY()) ? _texture->getSizeX() : _texture->getSizeY());
	//_transform->setLocalScale(vec3(1.0f, 1.0f, 1.0f));
}

Mesh * UIImage::getMesh() const
{
	return _mesh;
}

void UIImage::setMesh(Mesh * mesh)
{
	_mesh = mesh;
}

ShaderProgram * UIImage::getShaderProgram() const
{
	return _shaderProgram;
}

void UIImage::setShaderProgram(ShaderProgram * shaderProgram)
{
	_shaderProgram = shaderProgram;
}

string UIImage::getName() const
{
	return _name;
}

void UIImage::addChild(UIImage * child)
{
	child->_parent = this;
	_children.push_back(child);

	_transform->addChild(child->getTransform());
}

void UIImage::removeChild(UIImage * child)
{
	child->setParent(nullptr);

	vector<UIImage*>::iterator position = std::find(_children.begin(), _children.end(), child);

	if (position != _children.end())
		_children.erase(position);


	_transform->removeChild(child->getTransform());
}

void UIImage::setParent(UIImage * parent)
{
	if (parent)
		parent->addChild(this);
	else
		_parent = nullptr;
}

UIImage * UIImage::getParent() const
{
	return _parent;
}

vector<UIImage*> UIImage::getChildren() const
{
	return _children;
}
