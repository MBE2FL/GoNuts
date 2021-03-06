#include "Matrix44.h"

std::ostream & operator<<(std::ostream & out, const Matrix44 source)
{
	//Row
	for (int i = 0; i < 4; i++)
	{
		out << "| ";
		//Column
		for (int j = 0; j < 4; j++)
		{
			out << source.mV[i * 4 + j] << ' ';
		}
		out << '|' << "\n";
	}
	return out;
}

Matrix44 Matrix44::operator+(const Matrix44 rhs) const
{
	Matrix44 result;
	for (int i = 0; i < 16; i++)
	{
		result.mV[i] = mV[i] + rhs.mV[i];
	}
	return result;
}

Matrix44 Matrix44::operator-(const Matrix44 rhs) const
{
	Matrix44 result;
	for (int i = 0; i < 16; i++)
	{
		result.mV[i] = mV[i] - rhs.mV[i];
	}
	return result;
}

Matrix44 Matrix44::operator*(const Matrix44 rhs) const
{
	Matrix44 result;

	result.mV[0] = mV[0] * rhs.mV[0] + mV[1] * rhs.mV[4] + mV[2] * rhs.mV[8] + mV[3] * rhs.mV[12];
	result.mV[1] = mV[0] * rhs.mV[0 + 1] + mV[1] * rhs.mV[4 + 1] + mV[2] * rhs.mV[8 + 1] + mV[3] * rhs.mV[12 + 1];
	result.mV[2] = mV[0] * rhs.mV[0 + 2] + mV[1] * rhs.mV[4 + 2] + mV[2] * rhs.mV[8 + 2] + mV[3] * rhs.mV[12 + 2];
	result.mV[3] = mV[0] * rhs.mV[0 + 3] + mV[1] * rhs.mV[4 + 3] + mV[2] * rhs.mV[8 + 3] + mV[3] * rhs.mV[12 + 3];

	result.mV[4] = mV[4] * rhs.mV[0] +     mV[5] * rhs.mV[4] +     mV[6] * rhs.mV[8] +     mV[7] * rhs.mV[12];
	result.mV[5] = mV[4] * rhs.mV[0 + 1] + mV[5] * rhs.mV[4 + 1] + mV[6] * rhs.mV[8 + 1] + mV[7] * rhs.mV[12 + 1];
	result.mV[6] = mV[4] * rhs.mV[0 + 2] + mV[5] * rhs.mV[4 + 2] + mV[6] * rhs.mV[8 + 2] + mV[7] * rhs.mV[12 + 2];
	result.mV[7] = mV[4] * rhs.mV[0 + 3] + mV[5] * rhs.mV[4 + 3] + mV[6] * rhs.mV[8 + 3] + mV[7] * rhs.mV[12 + 3];

	result.mV[8]  = mV[8] * rhs.mV[0] +     mV[9] * rhs.mV[4] +     mV[10] * rhs.mV[8] +     mV[11] * rhs.mV[12];
	result.mV[9]  = mV[8] * rhs.mV[0 + 1] + mV[9] * rhs.mV[4 + 1] + mV[10] * rhs.mV[8 + 1] + mV[11] * rhs.mV[12 + 1];
	result.mV[10] = mV[8] * rhs.mV[0 + 2] + mV[9] * rhs.mV[4 + 2] + mV[10] * rhs.mV[8 + 2] + mV[11] * rhs.mV[12 + 2];
	result.mV[11] = mV[8] * rhs.mV[0 + 3] + mV[9] * rhs.mV[4 + 3] + mV[10] * rhs.mV[8 + 3] + mV[11] * rhs.mV[12 + 3];

	result.mV[12] = mV[12] * rhs.mV[0] +     mV[13] * rhs.mV[4] +     mV[14] * rhs.mV[8] +     mV[15] * rhs.mV[12];
	result.mV[13] = mV[12] * rhs.mV[0 + 1] + mV[13] * rhs.mV[4 + 1] + mV[14] * rhs.mV[8 + 1] + mV[15] * rhs.mV[12 + 1];
	result.mV[14] = mV[12] * rhs.mV[0 + 2] + mV[13] * rhs.mV[4 + 2] + mV[14] * rhs.mV[8 + 2] + mV[15] * rhs.mV[12 + 2];
	result.mV[15] = mV[12] * rhs.mV[0 + 3] + mV[13] * rhs.mV[4 + 3] + mV[14] * rhs.mV[8 + 3] + mV[15] * rhs.mV[12 + 3];

	return result;
}

Matrix44 Matrix44::operator*(const float scalar) const
{
	Matrix44 result;
	for (int i = 0; i < 16; i++)
	{
		result.mV[i] = mV[i] * scalar;
	}
	return result;
}

void Matrix44::SetRows(Vector3 row1, Vector3 row2, Vector3 row3)
{
}

void Matrix44::GetRows(Vector3 row1, Vector3 row2, Vector3 row3)
{
}

Vector3 Matrix44::GetRow(int i)
{
	return Vector3();
}

void Matrix44::SetColumns(Vector3 col1, Vector3 col2, Vector3 col3)
{
}

void Matrix44::GetColumns(Vector3 col1, Vector3 col2, Vector3 col3)
{
}

Vector3 Matrix44::GetColumn(int i)
{
	return Vector3();
}

Matrix44 Matrix44::Inverse()
{
	return Matrix44();
}

Matrix44 Matrix44::Transpose()
{
	return Matrix44();
}

void Matrix44::Identity()
{
}

float Matrix44::Determinant()
{
	return 0.0f;
}

float Matrix44::Trace()
{
	return 0.0f;
}

void Matrix44::RotateX(float angle)
{
	Matrix44 rotation;
	rotation.mV[0] = 1.0f;
	rotation.mV[1] = 0.f;
	rotation.mV[2] = 0.f;
	rotation.mV[3] = 0.f;
		
	rotation.mV[4] = 0.f;
	rotation.mV[5] = cosf(angle);
	rotation.mV[6] = -sinf(angle);
	rotation.mV[7] = 0.f;
		
	rotation.mV[8] = 0.f;
	rotation.mV[9] = sinf(angle);
	rotation.mV[10] = cosf(angle);
	rotation.mV[11] = 0.f;
		
	rotation.mV[12] = 0.f;
	rotation.mV[13] = 0.f;
	rotation.mV[14] = 0.f;
	rotation.mV[15] = 1.0f;

	*this = rotation;
}

void Matrix44::RotateY(float angle)
{
	Matrix44 rotation;
	rotation.mV[0] = cosf(angle);
	rotation.mV[1] = 0.f;
	rotation.mV[2] = sinf(angle);
	rotation.mV[3] = 0.f;

	rotation.mV[4] = 0.f;
	rotation.mV[5] = 1.0f;
	rotation.mV[6] = 0.f;
	rotation.mV[7] = 0.f;

	rotation.mV[8] = -sinf(angle);
	rotation.mV[9] = 0.f;
	rotation.mV[10] = cosf(angle);
	rotation.mV[11] = 0.f;

	rotation.mV[12] = 0.f;
	rotation.mV[13] = 0.f;
	rotation.mV[14] = 0.f;
	rotation.mV[15] = 1.0f;

	*this = rotation;
}

void Matrix44::RotateZ(float angle)
{
	Matrix44 rotation;
	rotation.mV[0] = cosf(angle);
	rotation.mV[1] = -sinf(angle);
	rotation.mV[2] = 0.f;
	rotation.mV[3] = 0.f;

	rotation.mV[4] = sinf(angle);
	rotation.mV[5] = cosf(angle);
	rotation.mV[6] = 0.f;
	rotation.mV[7] = 0.f;

	rotation.mV[8] = 0.f;
	rotation.mV[9] = 0.f;
	rotation.mV[10] = 1.0f;
	rotation.mV[11] = 0.f;

	rotation.mV[12] = 0.f;
	rotation.mV[13] = 0.f;
	rotation.mV[14] = 0.f;
	rotation.mV[15] = 1.0f;

	*this = rotation;
}

void Matrix44::Translate(const Vector3 & translation)
{
	Matrix44 tran;
	tran.mV[0] = 1.0f;
	tran.mV[1] = 0.f;
	tran.mV[2] = 0.f;
	tran.mV[3] = translation.x;

	tran.mV[4] = 0.f;
	tran.mV[5] = 1.0f;
	tran.mV[6] = 0.f;
	tran.mV[7] = translation.y;

	tran.mV[8] = 0.f;
	tran.mV[9] = 0.f;
	tran.mV[10] = 1.0f;
	tran.mV[11] = translation.z;

	tran.mV[12] = 0.f;
	tran.mV[13] = 0.f;
	tran.mV[14] = 0.f;
	tran.mV[15] = 1.0f;

	*this = tran;
}

void Matrix44::Scale(float scale)
{
	Matrix44 s;
	s.mV[0] = scale;
	s.mV[1] = 0.f;
	s.mV[2] = 0.f;
	s.mV[3] = 0.f;
	
	s.mV[4] = 0.f;
	s.mV[5] = scale;
	s.mV[6] = 0.f;
	s.mV[7] = 0.f;
	
	s.mV[8] = 0.f;
	s.mV[9] = 0.f;
	s.mV[10] = scale;
	s.mV[11] = 0.f;
	
	s.mV[12] = 0.f;
	s.mV[13] = 0.f;
	s.mV[14] = 0.f;
	s.mV[15] = 1.0f;

	*this = s;
}
