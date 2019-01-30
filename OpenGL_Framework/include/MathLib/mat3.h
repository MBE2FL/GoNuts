#pragma once
#include "vec3.h"
#include <ostream>
class mat3
{
public:
	float data[9];
	mat3() {};
	~mat3() {};
	//Text Output
	friend std::ostream& operator<<(std::ostream& out, const mat3& source);
	//Basic Operations
	mat3 operator+(const mat3& rhs) const;
	mat3 operator-(const mat3& rhs) const;
	mat3 operator*(const mat3& rhs) const;
	mat3 operator*(const float scalar) const;
	//Row Manipulators
	void setRows(const vec3& row1, const vec3& row2, const vec3& row3);
	void getRows(vec3& row1, vec3& row2, vec3& row3);
	vec3 getRow(int i);
	//Column Manipulators
	void setColumns(const vec3& col1, const vec3& col2, const vec3& col3);
	void getColumns(vec3& col1, vec3& col2, vec3& col3);
	vec3 getColumn(int i);
	//Manipulators
	mat3 inverse();
	mat3 transpose();
	void identity();
	float determinant();
	float trace();
};
