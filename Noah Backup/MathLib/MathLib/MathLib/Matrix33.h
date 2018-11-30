//Matrixx33.h
#pragma once

#include "Vector3.h"
#include <ostream>

class Matrix33 {
public:

	float mV[9];
	Matrix33() {};
	~Matrix33() {};
	//Text Output
	friend std::ostream& operator<<(std::ostream& out, const Matrix33 source);
	//Basic Operations
	Matrix33 operator+(const Matrix33 rhs) const;
	Matrix33 operator-(const Matrix33 rhs) const;
	Matrix33 operator*(const Matrix33 rhs) const;
	Matrix33 operator*(const float scalar) const;
	//Row Manipulators
	void SetRows(Vector3 row1, Vector3 row2, Vector3 row3);
	void GetRows(Vector3 row1, Vector3 row2, Vector3 row3);
	Vector3 GetRow(int i);
	//Column Manipulators
	void SetColumns(Vector3 col1, Vector3 col2, Vector3 col3);
	void GetColumns(Vector3 col1, Vector3 col2, Vector3 col3);
	Vector3 GetColumn(int i);
	//Manipulators
	Matrix33 Inverse();
	Matrix33 Transpose();
	void Identity();
	float Determinant();
	float Trace();
};
