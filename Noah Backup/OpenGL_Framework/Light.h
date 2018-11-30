#pragma once
#include "MathLib/MathLibCore.h"

class Light
{
public:
	Light();
	~Light();

	Vector3 getPosition() const;
	void setPosition(const Vector3& position);
	Vector3 getAmbient() const;
	void setAmbient(const Vector3& ambient);
	Vector3 getDiffuse() const;
	void setDiffuse(const Vector3& diffuse);
	Vector3 getSpecular() const;
	void setSpecular(const Vector3& specular);
	float getSpecularExp() const;
	void setSpecularExp(const float specularExp);
	float getAttenuationConstant() const;
	void setAttenuationConstant(const float attenuationConstant);
	float getAttenuationLinear() const;
	void setAttenuationLinear(const float attenuationLinear);
	float getAttenuationQuadratic() const;
	void setAttenuationQuadratic(const float attenuationQuadratic);

private:
	Vector3 _position;
	Vector3 _ambient;
	Vector3 _diffuse;
	Vector3 _specular;
	float _specularExp;
	float _attenuationConstant;
	float _attenuationLinear;
	float _attenuationQuadratic;

};
