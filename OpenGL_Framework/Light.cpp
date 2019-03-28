#include "Light.h"
#include "Framebuffer.h"
#include "MathLib/MathLibCore.h"

Light::Light()
{
	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	position = vec4(0.0f);
	constantAtten = 1.0f;
	linearAtten = 0.01f;
	quadAtten = 0.001f;
}

void Light::init()
{
	// Initialized as a point light
	_UBO.allocateMemory(sizeof(vec4) * 3 + sizeof(float) * 4);
	_UBO.setName("Light");
	type = LightType::Point;
}

void Light::bind()
{
	_UBO.bind(3);
}

void Light::update(float dt)
{
	Transform::update(dt);
	calculateRadius();
	_UBO.sendData(&color, sizeof(vec4) * 3 + sizeof(float) * 4, 0);
}

float Light::calculateRadius()
{
	float luminance = vec3::dot((vec3(color)/color.w), vec3(0.3f, 0.59f, 0.11f));
	float luminanceMin = 0.05f;
	radius = (-linearAtten + sqrtf(linearAtten * linearAtten - 4.0f * quadAtten * (constantAtten - luminance / luminanceMin))) / (2.0f * quadAtten);
	return radius;
}