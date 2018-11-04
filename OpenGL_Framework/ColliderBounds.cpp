#include "ColliderBounds.h"

ColliderBounds::ColliderBounds() : 
	Bounds()
{
}

ColliderBounds::ColliderBounds(const float minX, const float maxX, const float minY, const float maxY, const float minZ, const float maxZ) :
	Bounds(minX, maxX, minY, maxY, minZ, maxZ)
{
	points[0] = Vector3(minX, minY, minZ); // Bot left
	points[1] = Vector3(minX, minY, maxZ);	// Bot back left
	points[2] = Vector3(minX, maxY, minZ);	// Top left
	points[3] = Vector3(minX, maxY, maxZ); // Top back left
	points[4] = Vector3(maxX, minY, minZ);	// Bot right
	points[5] = Vector3(maxX, minY, maxZ);	// Bot back right
	points[6] = Vector3(maxX, maxY, minZ);	// Top right
	points[7] = Vector3(maxX, maxY, maxZ);	// Top back right
}

ColliderBounds::ColliderBounds(MeshBounds meshBounds) : 
	Bounds(meshBounds.getMin().x, meshBounds.getMax().x, meshBounds.getMin().y, meshBounds.getMax().y, 
		meshBounds.getMin().z, meshBounds.getMax().z)
{
	float minX = meshBounds.getMin().x;
	float minY = meshBounds.getMin().y;
	float minZ = meshBounds.getMin().z;

	float maxX = meshBounds.getMax().x;
	float maxY = meshBounds.getMax().y;
	float maxZ = meshBounds.getMax().z;

	points[0] = Vector3(minX, minY, minZ); // Bot left
	points[1] = Vector3(minX, minY, maxZ);	// Bot back left
	points[2] = Vector3(minX, maxY, minZ);	// Top left
	points[3] = Vector3(minX, maxY, maxZ); // Top back left
	points[4] = Vector3(maxX, minY, minZ);	// Bot right
	points[5] = Vector3(maxX, minY, maxZ);	// Bot back right
	points[6] = Vector3(maxX, maxY, minZ);	// Top right
	points[7] = Vector3(maxX, maxY, maxZ);	// Top back right
}

void ColliderBounds::update(const Transform& transform)
{
	updatePoint(transform);
}


//void ColliderBounds::recalculateMaxMin(const Vector3& point)
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

//void ColliderBounds::updatePoints(const Transform& transform)
//{
//	setCentre(transform.getPosition());
//
//	for (int i = 0; i < 8; i++)
//	{
//		Vector3 modifiedPoints[8];
//		// Create 4x4 transformation matrix
//
//		// Create rotation matrix
//		Matrix44 rx;
//		Matrix44 ry;
//		Matrix44 rz;
//
//		rx.RotateX(transform.getRotationAngleX());
//		ry.RotateY(transform.getRotationAngleY());
//		rz.RotateZ(transform.getRotationAngleZ());
//		// Note: pay attention to rotation order, ZYX is not the same as XYZ
//		Matrix44 rotation = rz * ry * rx;
//
//		// Create translation matrix
//		Matrix44 tran;
//		tran.Translate(points[i] + getCentre());
//
//		// Create scale matrix
//		Matrix44 scale;
//		scale.Scale(transform.getScale());
//		
//		Matrix44 newPointMatrix = tran * rotation * scale;
//		Vector4 blah = (newPointMatrix * Vector4(points[i], 1.0f));
//		
//		modifiedPoints[i] = Vector3(blah.x, blah.y, blah.z);
//	}
//}

void ColliderBounds::updatePoint(const Transform& transform)
{
	setCentre(transform.getPosition());
	// Create 4x4 transformation matrix

	// Create rotation matrix
	Matrix44 rx;
	Matrix44 ry;
	Matrix44 rz;

	rx.RotateX(transform.getRotationAngleX());
	ry.RotateY(transform.getRotationAngleY());
	rz.RotateZ(transform.getRotationAngleZ());
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	Matrix44 rotation = rz * ry * rx;

	// Create translation matrix
	Matrix44 minTran;
	minTran.Translate(getCentre());

	Matrix44 maxTran;
	maxTran.Translate(getCentre());

	// Create scale matrix
	Matrix44 scale;
	scale.Scale(transform.getScale());

	Matrix44 newMinMatrix = minTran * rotation * scale;
	Matrix44 newMaxMatrix = maxTran * rotation * scale;

	Vector4 newMin = (newMinMatrix * Vector4(points[0], 1.0f));
	setMin(Vector3(newMin.x, newMin.y, newMin.z));

	Vector4 newMax = (newMaxMatrix * Vector4(points[7], 1.0f));
	setMax(Vector3(newMax.x, newMax.y, newMax.z));
}