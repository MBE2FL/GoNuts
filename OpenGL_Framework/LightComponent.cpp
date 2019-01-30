#include "LightComponent.h"

LightComponent::LightComponent()
{
	_type = ComponentType::Light;
}

LightComponent::~LightComponent()
{
}

vec3 LightComponent::getPosition() const
{
	return _position;
}

void LightComponent::setPosition(const vec3 & position)
{
	_position = position;
}

vec3 LightComponent::getAmbient() const
{
	return _ambient;
}

void LightComponent::setAmbient(const vec3 & ambient)
{
	_ambient = ambient;
}

vec3 LightComponent::getDiffuse() const
{
	return _diffuse;
}

void LightComponent::setDiffuse(const vec3 & diffuse)
{
	_diffuse = diffuse;
}

vec3 LightComponent::getSpecular() const
{
	return _specular;
}

void LightComponent::setSpecular(const vec3 & specular)
{
	_specular = specular;
}

float LightComponent::getSpecularExp() const
{
	return _specularExp;
}

void LightComponent::setSpecularExp(const float specularExp)
{
	_specularExp = specularExp;
}

float LightComponent::getAttenuationConstant() const
{
	return _attenuationConstant;
}

void LightComponent::setAttenuationConstant(const float attenuationConstant)
{
	_attenuationConstant = attenuationConstant;
}

float LightComponent::getAttenuationLinear() const
{
	return _attenuationLinear;
}

void LightComponent::setAttenuationLinear(const float attenuationLinear)
{
	_attenuationLinear = attenuationLinear;
}

float LightComponent::getAttenuationQuadratic() const
{
	return _attenuationQuadratic;
}

void LightComponent::setAttenuationQuadratic(const float attenuationQuadratic)
{
	_attenuationQuadratic = attenuationQuadratic;
}

vec3 LightComponent::getDirection() const
{
	return _direction;
}

void LightComponent::setDirection(const vec3 & direction)
{
	_direction = direction;
}

LightType LightComponent::getLightType() const
{
	return _lightType;
}

void LightComponent::setLightType(LightType type)
{
	_lightType = type;
}
