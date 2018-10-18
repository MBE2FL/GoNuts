#include "ColliderBounds.h"

ColliderBounds::ColliderBounds() : 
	Bounds()
{
}

ColliderBounds::ColliderBounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ) :
	Bounds(minX, maxX, minY, maxY, minZ, maxZ)
{
	points[0] = vec3(minX, minY, minZ); // Bot left
	points[1] = vec3(minX, minY, maxZ);	// Bot back left
	points[2] = vec3(minX, maxY, minZ);	// Top left
	points[3] = vec3(minX, maxY, maxZ); // Top back left
	points[4] = vec3(maxX, minY, minZ);	// Bot right
	points[5] = vec3(maxX, minY, maxZ);	// Bot back right
	points[6] = vec3(maxX, maxY, minZ);	// Top right
	points[7] = vec3(maxX, maxY, maxZ);	// Top back right
}

void ColliderBounds::update(const Transform& transform)
{
	updatePoints(transform);
}


//void ColliderBounds::recalculateMaxMin(const vec3& point)
//{
//	// Found new max
//	if ((point.x > getMax().x) && (point.y > getMax().y) && (point.z > getMax().z))
//		setMax(point);
//	// Found new min
//	else if ((point.x < getMin().x) && (point.y < getMin().y) && (point.z < getMin().z))
//		setMin(point);
//
//	// Update new size
//	setSize(getMax() - getMin());
//
//}

void ColliderBounds::updatePoints(const Transform& transform)
{
	setCentre(transform.getPosition());

	for (int i = 0; i < 8; i++)
	{
		vec3 modifiedPoints[8];
		// Create 4x4 transformation matrix

		// Create rotation matrix
		mat4 rx;
		mat4 ry;
		mat4 rz;

		rx.RotateX(transform.getRotationAngleX());
		ry.RotateY(transform.getRotationAngleY());
		rz.RotateZ(transform.getRotationAngleZ());
		// Note: pay attention to rotation order, ZYX is not the same as XYZ
		mat4 rotation = rz * ry * rx;

		// Create translation matrix
		mat4 tran;
		tran.Translate(points[i] + getCentre());

		// Create scale matrix
		mat4 scale;
		scale.Scale(transform.getScale());
		
		mat4 newPointMatrix = tran * rotation * scale;
		vec4 blah = (newPointMatrix * vec4(points[i], 1.0f));
		
		modifiedPoints[i] = vec3(blah.x, blah.y, blah.z);
	}
}

void ColliderBounds::updatePoint(const Transform& transform)
{
	setCentre(transform.getPosition());
	// Create 4x4 transformation matrix

	// Create rotation matrix
	mat4 rx;
	mat4 ry;
	mat4 rz;

	rx.RotateX(transform.getRotationAngleX());
	ry.RotateY(transform.getRotationAngleY());
	rz.RotateZ(transform.getRotationAngleZ());
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	mat4 rotation = rz * ry * rx;

	// Create translation matrix
	mat4 minTran;
	minTran.Translate(points[0] + getCentre());

	mat4 maxTran;
	maxTran.Translate(points[7] + getCentre());

	// Create scale matrix
	mat4 scale;
	scale.Scale(transform.getScale());

	mat4 newMinMatrix = minTran * rotation * scale;
	mat4 newMaxMatrix = maxTran * rotation * scale;

	vec4 newMin = (newMinMatrix * vec4(points[0], 1.0f));
	setMin(vec3(newMin.x, newMin.y, newMin.z));

	vec4 newMax = (newMaxMatrix * vec4(points[7], 1.0f));
	setMax(vec3(newMax.x, newMax.y, newMax.z));
}