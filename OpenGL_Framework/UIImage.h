#pragma once
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "MathLib/MathLibCore.h"
#include "ObjectLoader.h"
#include "UIAnimation.h"
#include "UIAnimator.h"

class UIImage
{
public:
	UIImage(const vec3& pos = vec3::Zero, const vec3& scale = vec3::One, const vec3& localRotation = vec3::Zero, const float alpha = 1.0f);

	void setAnimation(UIAnimation*);
	UIAnimator* getAnimator();

	vec3 getlocalPosition() const;
	void setlocalPosition(const vec3& pos);
	vec3 getScale() const;
	void setScale(const vec3& scale);
	vec3 getLocalRotation() const;
	void setLocalRotation(const vec3& rotation);
	void setLocalRotationX(const float rotation);
	void setLocalRotationY(const float rotation);
	void setLocalRotationZ(const float rotation);
	TransformComponent* getTransform() const;
	float getAlpha() const;
	void setAlpha(const float alpha);
	bool getHover() const;
	void setHover(const bool isHover);
	Texture* getTexture() const;
	void setTexture(Texture* texture);
	Mesh* getMesh() const;
	ShaderProgram* getShaderProgram() const;

private:
	Mesh* _mesh;
	ShaderProgram* _shaderProgram;
	Texture* _texture;
	TransformComponent* _transform;
	float _alpha;
	bool _isHover = false;
	UIAnimator* _animator = nullptr;
};