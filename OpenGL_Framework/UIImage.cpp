#include "UIImage.h"

UIImage::UIImage(const vec3 & pos, const vec3 & scale, const vec3 & localRotation, const float alpha)
{
	_animator = new UIAnimator;
	_transform = new TransformComponent();
	_transform->setLocalPosition(pos);
	_transform->setLocalScale(scale);
	_transform->setLocalRotation(localRotation);
	_alpha = alpha;
	_mesh = ObjectLoader::getMesh("UIQuad");
	_shaderProgram = ObjectLoader::getShaderProgram("UIShader");
}

void UIImage::setAnimation(UIAnimation * newAnimation)
{
	_animator->addAnimation(newAnimation);
}

UIAnimator * UIImage::getAnimator()
{
	return _animator;
}

vec3 UIImage::getlocalPosition() const
{
	return _transform->getLocalPosition();
}

void UIImage::setlocalPosition(const vec3 & pos)
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

float UIImage::getAlpha() const
{
	return _alpha;
}

void UIImage::setAlpha(const float alpha)
{
	_alpha = alpha;
}

bool UIImage::getHover() const
{
	return _isHover;
}

void UIImage::setHover(const bool isHover)
{
	_isHover = isHover;
}

Texture * UIImage::getTexture() const
{
	return _texture;
}

void UIImage::setTexture(Texture * texture)
{
	_texture = texture;

	//float size = static_cast<float>((_texture->getSizeX() > _texture->getSizeY()) ? _texture->getSizeX() : _texture->getSizeY());
	_transform->setLocalScale(vec3(1.0f, 1.0f, 1.0f));
}

Mesh * UIImage::getMesh() const
{
	return _mesh;
}

ShaderProgram * UIImage::getShaderProgram() const
{
	return _shaderProgram;
}
