#pragma once

#include "MathLib/MathLibCore.h"


class Light
{
public:
	Light();
	~Light();

	vec3 getPosition() const;
	void setPosition(const vec3& position);
	vec3 getAmbient() const;
	void setAmbient(const vec3& ambient);
	vec3 getDiffuse() const;
	void setDiffuse(const vec3& diffuse);
	vec3 getSpecular() const;
	void setSpecular(const vec3& specular);
	float getSpecularExp() const;
	void setSpecularExp(const float specularExp);
	float getAttenuationConstant() const;
	void setAttenuationConstant(const float attenuationConstant);
	float getAttenuationLinear() const;
	void setAttenuationLinear(const float attenuationLinear);
	float getAttenuationQuadratic() const;
	void setAttenuationQuadratic(const float attenuationQuadratic);

private:
	vec3 _position;
	vec3 _ambient;
	vec3 _diffuse;
	vec3 _specular;
	float _specularExp;
	float _attenuationConstant;
	float _attenuationLinear;
	float _attenuationQuadratic;

};
