#pragma once
#include "Vector3.h"
#include <ostream>
class Matrix44
{
public:
	float mV[16];
	Matrix44() {};
	~Matrix44() {};
	//Text Output
	friend std::ostream& operator<<(std::ostream& out, const Matrix44 source);
	//Basic Operations
	Matrix44 operator+(const Matrix44 rhs) const;
	Matrix44 operator-(const Matrix44 rhs) const;
	Matrix44 operator*(const Matrix44 rhs) const;
	Matrix44 operator*(const float scalar) const;
	//Row Manipulators
	void SetRows(Vector3 row1, Vector3 row2, Vector3 row3);
	void GetRows(Vector3 row1, Vector3 row2, Vector3 row3);
	Vector3 GetRow(int i);
	//Column Manipulators
	void SetColumns(Vector3 col1, Vector3 col2, Vector3 col3);
	void GetColumns(Vector3 col1, Vector3 col2, Vector3 col3);
	Vector3 GetColumn(int i);
	//Manipulators
	Matrix44 Inverse();
	Matrix44 Transpose();
	void Identity();
	float Determinant();
	float Trace();

	void RotateX(float angle);
	void RotateY(float angle);
	void RotateZ(float angle);

	void Translate(const Vector3 &translation);
	void Scale(float scale);

	static Matrix44 PerspectiveProjection(float FOVy, float aspect, float zNear, float zFar);
	static Matrix44 OrthographicProjection(float left, float right, float top, float bottom, float near, float far);
};
