#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

Vector3 Light::getPosition() const
{
	return _position;
}

void Light::setPosition(const Vector3 & position)
{
	_position = position;
}

Vector3 Light::getAmbient() const
{
	return _ambient;
}

void Light::setAmbient(const Vector3 & ambient)
{
	_ambient = ambient;
}

Vector3 Light::getDiffuse() const
{
	return _diffuse;
}

void Light::setDiffuse(const Vector3 & diffuse)
{
	_diffuse = diffuse;
}

Vector3 Light::getSpecular() const
{
	return _specular;
}

void Light::setSpecular(const Vector3 & specular)
{
	_specular = specular;
}

float Light::getSpecularExp() const
{
	return _specularExp;
}

void Light::setSpecularExp(const float specularExp)
{
	_specularExp = specularExp;
}

float Light::getAttenuationConstant() const
{
	return _attenuationConstant;
}

void Light::setAttenuationConstant(const float attenuationConstant)
{
	_attenuationConstant = attenuationConstant;
}

float Light::getAttenuationLinear() const
{
	return _attenuationLinear;
}

void Light::setAttenuationLinear(const float attenuationLinear)
{
	_attenuationLinear = attenuationLinear;
}

float Light::getAttenuationQuadratic() const
{
	return _attenuationQuadratic;
}

void Light::setAttenuationQuadratic(const float attenuationQuadratic)
{
	_attenuationQuadratic = attenuationQuadratic;
}
