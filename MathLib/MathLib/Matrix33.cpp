#include "Matrix33.h"

std::ostream & operator<<(std::ostream & out, const Matrix33 source)
{
	//Row
	for (int i = 0; i < 3; i++)
	{
		out << "| ";
		//Column
		for (int j = 0; j < 3; j++)
		{
			out << source.mV[j * 3 + i] << ' ';
		}
		out << '|' << "\n";
	}
	return out;
}

Matrix33 Matrix33::operator+(const Matrix33 rhs) const
{
	Matrix33 result;
	for (int i = 0; i < 9; i++)
	{
		result.mV[i] = mV[i] + rhs.mV[i];
	}
	return result;
}

Matrix33 Matrix33::operator-(const Matrix33 rhs) const
{
	Matrix33 result;
	for (int i = 0; i < 9; i++)
	{
		result.mV[i] = mV[i] - rhs.mV[i];
	}
	return result;
}

Matrix33 Matrix33::operator*(const Matrix33 rhs) const
{
	Matrix33 result;
	
	result.mV[0] = mV[0] * rhs.mV[0] + mV[3] * rhs.mV[1] + mV[6] * rhs.mV[2];	
	result.mV[1] = mV[1] * rhs.mV[0] + mV[4] * rhs.mV[1] + mV[7] * rhs.mV[2];	
	result.mV[2] = mV[2] * rhs.mV[0] + mV[5] * rhs.mV[1] + mV[8] * rhs.mV[2];

	result.mV[3] = mV[0] * rhs.mV[3] + mV[3] * rhs.mV[4] + mV[6] * rhs.mV[5];
	result.mV[4] = mV[1] * rhs.mV[3] + mV[4] * rhs.mV[4] + mV[7] * rhs.mV[5];
	result.mV[5] = mV[2] * rhs.mV[3] + mV[5] * rhs.mV[4] + mV[8] * rhs.mV[5];

	result.mV[6] = mV[0] * rhs.mV[6] + mV[3] * rhs.mV[7] + mV[6] * rhs.mV[8];
	result.mV[7] = mV[1] * rhs.mV[6] + mV[4] * rhs.mV[7] + mV[7] * rhs.mV[8];
	result.mV[8] = mV[2] * rhs.mV[6] + mV[5] * rhs.mV[7] + mV[8] * rhs.mV[8];

	return result;
}

Matrix33 Matrix33::operator*(const float scalar) const
{
	Matrix33 result;
	for (int i = 0; i < 9; i++)
	{
		result.mV[i] = mV[i] * scalar;
	}
	return result;
}

void Matrix33::SetRows(Vector3 row1, Vector3 row2, Vector3 row3)
{
	mV[0] = row1.x; mV[1] = row2.x; mV[2] = row3.x;
	mV[3] = row1.y; mV[4] = row2.y; mV[5] = row3.y;
	mV[6] = row1.z; mV[7] = row2.z; mV[8] = row3.z;
}

void Matrix33::GetRows(Vector3 row1, Vector3 row2, Vector3 row3)
{
	row1.x = mV[0]; row2.x = mV[1];	row3.x = mV[2];
	row1.y = mV[3];	row2.y = mV[4];	row3.y = mV[5];
	row1.z = mV[6];	row2.z = mV[7];	row3.z = mV[8];
}

Vector3 Matrix33::GetRow(int i)
{
	return Vector3(mV[i], mV[i + 3], mV[i+6]);
}

void Matrix33::SetColumns(Vector3 col1, Vector3 col2, Vector3 col3)
{
	mV[0] = col1.x;
	mV[1] = col1.y;
	mV[2] = col1.z;
	mV[3] = col2.x;
	mV[4] = col2.y;
	mV[5] = col2.z;
	mV[6] = col3.x;
	mV[7] = col3.y;
	mV[8] = col3.z;
}

void Matrix33::GetColumns(Vector3 col1, Vector3 col2, Vector3 col3)
{
	col1.x = mV[0];
	col1.y = mV[1];
	col1.z = mV[2];
	col2.x = mV[3];
	col2.y = mV[4];
	col2.z = mV[5];
	col3.x = mV[6];
	col3.y = mV[7];
	col3.z = mV[8];
}

Vector3 Matrix33::GetColumn(int i)
{
	return Vector3(mV[3 * i], mV[3 * i + 1], mV[3 * i + 2]);
}

Matrix33 Matrix33::Inverse()
{
	Matrix33 result;
	//Compute Determinant
	float cofactor0 = mV[4] * mV[8] - mV[5] * mV[7];
	float cofactor3 = mV[2] * mV[7] - mV[1] * mV[8];
	float cofactor6 = mV[1] * mV[5] - mV[2] * mV[4];
	float det = mV[0] * cofactor0 + mV[3] * cofactor3 + mV[6] * cofactor6;
	//Create ajoint matrix and multiply by 1/det to get inverse
	float invDet = 1.0f / det;
	result.mV[0] = invDet * cofactor0;
	result.mV[1] = invDet * cofactor3;
	result.mV[2] = invDet * cofactor6;
	
	result.mV[3] = invDet * (mV[5] * mV[6] - mV[3] * mV[8]);
	result.mV[4] = invDet * (mV[0] * mV[8] - mV[2] * mV[6]);
	result.mV[5] = invDet * (mV[2] * mV[3] - mV[0] * mV[5]);

	result.mV[6] = invDet * (mV[3] * mV[7] - mV[4] * mV[6]);
	result.mV[7] = invDet * (mV[1] * mV[6] - mV[0] * mV[7]);
	result.mV[8] = invDet * (mV[0] * mV[4] - mV[1] * mV[3]);
	return result;
}

Matrix33 Matrix33::Transpose()
{
	float temp = mV[1];
	mV[1] = mV[3];
	mV[3] = temp;

	temp = mV[2];
	mV[2] = mV[6];
	mV[6] = temp;

	temp = mV[5];
	mV[5] = mV[7];
	mV[7] = temp;
	return *this;
}

void Matrix33::Identity()
{
	mV[0] = 1.0f;
	mV[1] = 0;
	mV[2] = 0;
	mV[3] = 0;
	mV[4] = 1.0f;
	mV[5] = 0;
	mV[6] = 0;
	mV[7] = 0;
	mV[8] = 1.0f;
}

float Matrix33::Determinant()
{
	return mV[0] * (mV[4] * mV[8] - mV[5] * mV[7])
		 + mV[3] * (mV[2] * mV[7] - mV[1] * mV[8])
		 + mV[6] * (mV[1] * mV[5] - mV[2] * mV[4]);
}

float Matrix33::Trace()
{
	return mV[0] + mV[4] + mV[8];
}
