#include "Bounds.h"

Bounds::Bounds()
{
	_centre = vec3::Zero;
	_extends = vec3(0.5f, 0.5f, 0.5f);
	_max = _extends;
	_min = -_extends;
	_size = 2.0f * _extends;
}

Bounds::Bounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ)
{
	_max = vec3(maxX, maxY, maxZ);
	_min = vec3(minX, minY, minZ);
	_centre = (_max + _min) * 0.5f;
	_extends = _max - _centre;
	_size = 2.0f * _extends;
}

Bounds::~Bounds()
{
}

vec3 Bounds::getCentre() const
{
	return _centre;
}

void Bounds::setCentre(const vec3 & centre)
{
	_centre = centre;
}

vec3 Bounds::getExtends() const
{
	return _extends;
}

vec3 Bounds::getMax() const
{
	return _max;
}

void Bounds::setMax(const vec3 & max)
{
	_max = max;
}

vec3 Bounds::getMin() const
{
	return _min;
}

void Bounds::setMin(const vec3 & min)
{
	_min = min;
}

vec3 Bounds::getSize() const
{
	return _size;
}

void Bounds::setSize(const vec3 & size)
{
	_size = size;
}
