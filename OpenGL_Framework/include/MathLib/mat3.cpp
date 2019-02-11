#include "mat3.h"

std::ostream & operator<<(std::ostream & out, const mat3& source)
{
	//Row
	for (int i = 0; i < 3; i++)
	{
		out << "| ";
		//Column
		for (int j = 0; j < 3; j++)
		{
			out << source.data[j * 3 + i] << ' ';
		}
		out << '|' << "\n";
	}
	return out;
}

mat3 mat3::operator+(const mat3& rhs) const
{
	mat3 result;
	for (int i = 0; i < 9; i++)
	{
		result.data[i] = data[i] + rhs.data[i];
	}
	return result;
}

mat3 mat3::operator-(const mat3& rhs) const
{
	mat3 result;
	for (int i = 0; i < 9; i++)
	{
		result.data[i] = data[i] - rhs.data[i];
	}
	return result;
}

mat3 mat3::operator*(const mat3& rhs) const
{
	mat3 result;
	
	result.data[0] = data[0] * rhs.data[0] + data[3] * rhs.data[1] + data[6] * rhs.data[2];	
	result.data[1] = data[1] * rhs.data[0] + data[4] * rhs.data[1] + data[7] * rhs.data[2];	
	result.data[2] = data[2] * rhs.data[0] + data[5] * rhs.data[1] + data[8] * rhs.data[2];

	result.data[3] = data[0] * rhs.data[3] + data[3] * rhs.data[4] + data[6] * rhs.data[5];
	result.data[4] = data[1] * rhs.data[3] + data[4] * rhs.data[4] + data[7] * rhs.data[5];
	result.data[5] = data[2] * rhs.data[3] + data[5] * rhs.data[4] + data[8] * rhs.data[5];

	result.data[6] = data[0] * rhs.data[6] + data[3] * rhs.data[7] + data[6] * rhs.data[8];
	result.data[7] = data[1] * rhs.data[6] + data[4] * rhs.data[7] + data[7] * rhs.data[8];
	result.data[8] = data[2] * rhs.data[6] + data[5] * rhs.data[7] + data[8] * rhs.data[8];

	return result;
}

mat3 mat3::operator*(const float scalar) const
{
	mat3 result;
	for (int i = 0; i < 9; i++)
	{
		result.data[i] = data[i] * scalar;
	}
	return result;
}

void mat3::setRows(const vec3& row1, const vec3& row2, const vec3& row3)
{
	data[0] = row1.x; data[1] = row2.x; data[2] = row3.x;
	data[3] = row1.y; data[4] = row2.y; data[5] = row3.y;
	data[6] = row1.z; data[7] = row2.z; data[8] = row3.z;
}

void mat3::getRows(vec3& row1, vec3& row2, vec3& row3)
{
	row1.x = data[0]; row2.x = data[1];	row3.x = data[2];
	row1.y = data[3];	row2.y = data[4];	row3.y = data[5];
	row1.z = data[6];	row2.z = data[7];	row3.z = data[8];
}

vec3 mat3::getRow(int i)
{
	return vec3(data[i], data[i + 3], data[i+6]);
}

void mat3::setColumns(const vec3& col1, const vec3& col2, const vec3& col3)
{
	data[0] = col1.x;
	data[1] = col1.y;
	data[2] = col1.z;
	data[3] = col2.x;
	data[4] = col2.y;
	data[5] = col2.z;
	data[6] = col3.x;
	data[7] = col3.y;
	data[8] = col3.z;
}

void mat3::getColumns(vec3& col1, vec3& col2, vec3& col3)
{
	col1.x = data[0];
	col1.y = data[1];
	col1.z = data[2];
	col2.x = data[3];
	col2.y = data[4];
	col2.z = data[5];
	col3.x = data[6];
	col3.y = data[7];
	col3.z = data[8];
}

vec3 mat3::getColumn(int i)
{
	return vec3(data[3 * i], data[3 * i + 1], data[3 * i + 2]);
}

mat3 mat3::inverse()
{
	mat3 result;
	//Compute Determinant
	float cofactor0 = data[4] * data[8] - data[5] * data[7];
	float cofactor3 = data[2] * data[7] - data[1] * data[8];
	float cofactor6 = data[1] * data[5] - data[2] * data[4];
	float det = data[0] * cofactor0 + data[3] * cofactor3 + data[6] * cofactor6;
	//Create ajoint matrix and multiply by 1/det to get inverse
	float invDet = 1.0f / det;
	result.data[0] = invDet * cofactor0;
	result.data[1] = invDet * cofactor3;
	result.data[2] = invDet * cofactor6;
	
	result.data[3] = invDet * (data[5] * data[6] - data[3] * data[8]);
	result.data[4] = invDet * (data[0] * data[8] - data[2] * data[6]);
	result.data[5] = invDet * (data[2] * data[3] - data[0] * data[5]);

	result.data[6] = invDet * (data[3] * data[7] - data[4] * data[6]);
	result.data[7] = invDet * (data[1] * data[6] - data[0] * data[7]);
	result.data[8] = invDet * (data[0] * data[4] - data[1] * data[3]);
	return result;
}

mat3 mat3::transpose()
{
	float temp = data[1];
	data[1] = data[3];
	data[3] = temp;

	temp = data[2];
	data[2] = data[6];
	data[6] = temp;

	temp = data[5];
	data[5] = data[7];
	data[7] = temp;
	return *this;
}

void mat3::identity()
{
	data[0] = 1.0f;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	data[4] = 1.0f;
	data[5] = 0;
	data[6] = 0;
	data[7] = 0;
	data[8] = 1.0f;
}

float mat3::determinant()
{
	return data[0] * (data[4] * data[8] - data[5] * data[7])
		 + data[3] * (data[2] * data[7] - data[1] * data[8])
		 + data[6] * (data[1] * data[5] - data[2] * data[4]);
}

float mat3::trace()
{
	return data[0] + data[4] + data[8];
}
