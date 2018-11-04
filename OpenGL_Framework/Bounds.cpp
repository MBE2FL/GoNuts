#include "Bounds.h"

Bounds::Bounds()
{
	_centre = Vector3::Zero;
	_extends = Vector3(0.5f, 0.5f, 0.5f);
	_max = _extends;
	_min = -_extends;
	_size = 2.0f * _extends;
}

Bounds::Bounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ)
{
	_max = Vector3(maxX, maxY, maxZ);
	_min = Vector3(minX, minY, minZ);
	_centre = (_max + _min) * 0.5f;
	_extends = _max - _centre;
	_size = 2.0f * _extends;
}

Bounds::~Bounds()
{
}

Vector3 Bounds::getCentre() const
{
	return _centre;
}

void Bounds::setCentre(const Vector3 & centre)
{
	_centre = centre;
}

Vector3 Bounds::getExtends() const
{
	return _extends;
}

Vector3 Bounds::getMax() const
{
	return _max;
}

void Bounds::setMax(const Vector3 & max)
{
	_max = max;
}

Vector3 Bounds::getMin() const
{
	return _min;
}

void Bounds::setMin(const Vector3 & min)
{
	_min = min;
}

Vector3 Bounds::getSize() const
{
	return _size;
}

void Bounds::setSize(const Vector3 & size)
{
	_size = size;
}
