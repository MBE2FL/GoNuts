#include "Matrix44.h"
#include "Vector4.h"

std::ostream & operator<<(std::ostream & out, const Matrix44 source)
{
	//Row
	for (int i = 0; i < 4; i++)
	{
		out << "| ";
		//Column
		for (int j = 0; j < 4; j++)
		{
			out << source.mV[j * 4 + i] << ' ';
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

	result.mV[0 ] = mV[0] * rhs.mV[0] + mV[4] * rhs.mV[1] + mV[8 ] * rhs.mV[2] + mV[12] * rhs.mV[3];
	result.mV[1 ] = mV[1] * rhs.mV[0] + mV[5] * rhs.mV[1] + mV[9 ] * rhs.mV[2] + mV[13] * rhs.mV[3];
	result.mV[2 ] = mV[2] * rhs.mV[0] + mV[6] * rhs.mV[1] + mV[10] * rhs.mV[2] + mV[14] * rhs.mV[3];
	result.mV[3 ] = mV[3] * rhs.mV[0] + mV[7] * rhs.mV[1] + mV[11] * rhs.mV[2] + mV[15] * rhs.mV[3];

	result.mV[4 ] = mV[0] * rhs.mV[4] + mV[4] * rhs.mV[5] + mV[8 ] * rhs.mV[6] + mV[12] * rhs.mV[7];
	result.mV[5 ] = mV[1] * rhs.mV[4] + mV[5] * rhs.mV[5] + mV[9 ] * rhs.mV[6] + mV[13] * rhs.mV[7];
	result.mV[6 ] = mV[2] * rhs.mV[4] + mV[6] * rhs.mV[5] + mV[10] * rhs.mV[6] + mV[14] * rhs.mV[7];
	result.mV[7 ] = mV[3] * rhs.mV[4] + mV[7] * rhs.mV[5] + mV[11] * rhs.mV[6] + mV[15] * rhs.mV[7];

	result.mV[8 ] = mV[0] * rhs.mV[8] + mV[4] * rhs.mV[9] + mV[8 ] * rhs.mV[10] + mV[12] * rhs.mV[11];
	result.mV[9 ] = mV[1] * rhs.mV[8] + mV[5] * rhs.mV[9] + mV[9 ] * rhs.mV[10] + mV[13] * rhs.mV[11];
	result.mV[10] = mV[2] * rhs.mV[8] + mV[6] * rhs.mV[9] + mV[10] * rhs.mV[10] + mV[14] * rhs.mV[11];
	result.mV[11] = mV[3] * rhs.mV[8] + mV[7] * rhs.mV[9] + mV[11] * rhs.mV[10] + mV[15] * rhs.mV[11];

	result.mV[12] = mV[0] * rhs.mV[12] + mV[4] * rhs.mV[13] + mV[8 ] * rhs.mV[14] + mV[12] * rhs.mV[15];
	result.mV[13] = mV[1] * rhs.mV[12] + mV[5] * rhs.mV[13] + mV[9 ] * rhs.mV[14] + mV[13] * rhs.mV[15];
	result.mV[14] = mV[2] * rhs.mV[12] + mV[6] * rhs.mV[13] + mV[10] * rhs.mV[14] + mV[14] * rhs.mV[15];
	result.mV[15] = mV[3] * rhs.mV[12] + mV[7] * rhs.mV[13] + mV[11] * rhs.mV[14] + mV[15] * rhs.mV[15];

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

Matrix44 Matrix44::GetInverse(const Matrix44 & rot, const Vector3 & tran)
{
	Matrix44 transRot;
	transRot.mV[15] = 0.0f;
	transRot = rot.Transpose(); // R^t

	Vector4 rT = -(transRot * Vector4(tran, 1.0f)); // -R^t * T
	
	transRot.mV[12] = rT.x;
	transRot.mV[13] = rT.y;
	transRot.mV[14] = rT.z;
	transRot.mV[15] = 1.0f;


	return transRot;
}

Matrix44 Matrix44::GetInverse()
{
	float DetMOM[16];
	DetMOM[0] = mV[5] * mV[10] * mV[15] + 
				mV[6] * mV[11] * mV[13] +
				mV[7] * mV[9]  * mV[14] -
				mV[7] * mV[10] * mV[13] -
				mV[6] * mV[9]  * mV[15] -
				mV[5] * mV[11] * mV[14];

	DetMOM[1] = mV[4] * mV[10] * mV[15] +
				mV[6] * mV[11] * mV[12] +
				mV[7] * mV[8]  * mV[14] -
				mV[7] * mV[10] * mV[12] -
				mV[6] * mV[8]  * mV[15] -
				mV[4] * mV[11] * mV[14];

	DetMOM[2] = mV[4] * mV[9]  * mV[15] +
				mV[5] * mV[11] * mV[12] +
				mV[7] * mV[8]  * mV[13] -
				mV[7] * mV[9]  * mV[12] -
				mV[5] * mV[8]  * mV[15] -
				mV[4] * mV[11] * mV[13];

	DetMOM[3] = mV[4] * mV[9]  * mV[14] +
				mV[5] * mV[10] * mV[12] +
				mV[6] * mV[8]  * mV[13] -
				mV[6] * mV[9]  * mV[12] -
				mV[5] * mV[8]  * mV[14] -
				mV[4] * mV[10] * mV[13];

	DetMOM[4] = mV[1] * mV[10] * mV[15] +
				mV[2] * mV[11] * mV[13] +
				mV[3] * mV[9]  * mV[14] -
				mV[3] * mV[10] * mV[13] -
				mV[2] * mV[9]  * mV[15] -
				mV[1] * mV[11] * mV[14];

	DetMOM[5] = mV[0] * mV[10] * mV[15] +
				mV[2] * mV[11] * mV[12] +
				mV[3] * mV[8]  * mV[14] -
				mV[3] * mV[10] * mV[12] -
				mV[2] * mV[8]  * mV[15] -
				mV[0] * mV[11] * mV[14];

	DetMOM[6] = mV[0] * mV[9]  * mV[15] +
				mV[1] * mV[11] * mV[12] +
				mV[3] * mV[8]  * mV[13] -
				mV[3] * mV[9]  * mV[12] -
				mV[1] * mV[8]  * mV[15] -
				mV[0] * mV[11] * mV[13];

	DetMOM[7] = mV[0] * mV[9]  * mV[14] +
				mV[1] * mV[10] * mV[12] +
				mV[2] * mV[8]  * mV[13] -
				mV[2] * mV[9]  * mV[12] -
				mV[1] * mV[8]  * mV[14] -
				mV[0] * mV[10] * mV[13];

	DetMOM[8] = mV[1] * mV[6]  * mV[15] +
				mV[2] * mV[7]  * mV[13] +
				mV[3] * mV[5]  * mV[14] -
				mV[3] * mV[6]  * mV[13] -
				mV[2] * mV[5]  * mV[15] -
				mV[1] * mV[7]  * mV[14];

	DetMOM[9] = mV[0] * mV[6]  * mV[15] +
				mV[2] * mV[7]  * mV[12] +
				mV[3] * mV[4]  * mV[14] -
				mV[3] * mV[6]  * mV[12] -
				mV[2] * mV[4]  * mV[15] -
				mV[0] * mV[7]  * mV[14];

	DetMOM[10] = mV[0] * mV[5] * mV[15] +
				mV[1] * mV[7]  * mV[12] +
				mV[3] * mV[4]  * mV[13] -
				mV[3] * mV[5]  * mV[12] -
				mV[1] * mV[4]  * mV[15] -
				mV[0] * mV[7]  * mV[13];

	DetMOM[11] = mV[0] * mV[5] * mV[14] +
				mV[1] * mV[6]  * mV[12] +
				mV[2] * mV[4]  * mV[13] -
				mV[2] * mV[5]  * mV[12] -
				mV[1] * mV[4]  * mV[14] -
				mV[0] * mV[6]  * mV[13];

	DetMOM[12] = mV[1] * mV[6] * mV[11] +
				mV[2] * mV[7]  * mV[9]  +
				mV[3] * mV[5]  * mV[10] -
				mV[3] * mV[6]  * mV[9]  -
				mV[2] * mV[5]  * mV[11] -
				mV[1] * mV[7]  * mV[10];

	DetMOM[13] = mV[0] * mV[6] * mV[11] +
				mV[2] * mV[7]  * mV[8]  +
				mV[3] * mV[4]  * mV[10] -
				mV[3] * mV[6]  * mV[8]  -
				mV[2] * mV[4]  * mV[11] -
				mV[0] * mV[7]  * mV[10];

	DetMOM[14] = mV[0] * mV[5] * mV[11] +
				mV[1] * mV[7]  * mV[8]  +
				mV[3] * mV[4]  * mV[9]  -
				mV[3] * mV[5]  * mV[8]  -
				mV[1] * mV[4]  * mV[11] -
				mV[0] * mV[7]  * mV[9];

	DetMOM[15] = mV[0] * mV[5] * mV[10] +
				mV[1] * mV[6]  * mV[8]  +
				mV[2] * mV[4]  * mV[9]  -
				mV[2] * mV[5]  * mV[8]  -
				mV[1] * mV[4]  * mV[10] -
				mV[0] * mV[6]  * mV[9];
				
	Matrix44 Adjugate;
	Adjugate.mV[0]  = DetMOM[0];
	Adjugate.mV[4]  = -DetMOM[1];
	Adjugate.mV[8]  = DetMOM[2];
	Adjugate.mV[12] = -DetMOM[3];

	Adjugate.mV[1]  = -DetMOM[4];
	Adjugate.mV[5]  = DetMOM[5];
	Adjugate.mV[9]  = -DetMOM[6];
	Adjugate.mV[13] = DetMOM[7];

	Adjugate.mV[2]  = DetMOM[8];
	Adjugate.mV[6]  = -DetMOM[9];
	Adjugate.mV[10] = DetMOM[10];
	Adjugate.mV[14] = -DetMOM[11];

	Adjugate.mV[3]  = -DetMOM[12];
	Adjugate.mV[7]  = DetMOM[13];
	Adjugate.mV[11] = -DetMOM[14];
	Adjugate.mV[15] = DetMOM[15];

	float Det = (mV[0] * mV[5] * mV[10] * mV[15]) + (mV[0] * mV[6] * mV[11] * mV[13]) + (mV[0] * mV[7] * mV[9] * mV[14])

		- (mV[0] * mV[7] * mV[10] * mV[13]) - (mV[0] * mV[6] * mV[9] * mV[15]) - (mV[0] * mV[5] * mV[11] * mV[14])

		- (mV[1] * mV[4] * mV[10] * mV[15]) - (mV[2] * mV[4] * mV[11] * mV[13]) - (mV[3] * mV[4] * mV[9] * mV[14])

		+ (mV[3] * mV[4] * mV[10] * mV[13]) + (mV[2] * mV[4] * mV[9] * mV[15]) + (mV[1] * mV[4] * mV[11] * mV[14])

		+ (mV[1] * mV[6] * mV[8] * mV[15]) + (mV[2] * mV[7] * mV[8] * mV[13]) + (mV[3] * mV[5] * mV[8] * mV[14])

		- (mV[3] * mV[6] * mV[8] * mV[13]) - (mV[2] * mV[5] * mV[8] * mV[15]) - (mV[1] * mV[7] * mV[8] * mV[14])

		- (mV[1] * mV[6] * mV[11] * mV[12]) - (mV[2] * mV[7] * mV[9] * mV[12]) - (mV[3] * mV[5] * mV[10] * mV[12])

		+ (mV[3] * mV[6] * mV[9] * mV[12]) + (mV[2] * mV[5] * mV[11] * mV[12]) + (mV[1] * mV[7] * mV[10] * mV[12]);
				
	Matrix44 result;

	result = Adjugate * (1 / Det);

	return result;
}

Matrix44 Matrix44::Transpose() const
{
	Matrix44 result;


	result.mV[0] = mV[0];
	result.mV[1] = mV[4];
	result.mV[2] = mV[8];
	result.mV[3] = mV[12];

	result.mV[4] = mV[1];
	result.mV[5] = mV[5];
	result.mV[6] = mV[9];
	result.mV[7] = mV[13];

	result.mV[8] = mV[2];
	result.mV[9] = mV[6];
	result.mV[10] = mV[10];
	result.mV[11] = mV[14];

	result.mV[12] = mV[3];
	result.mV[13] = mV[7];
	result.mV[14] = mV[11];
	result.mV[15] = mV[15];


	return result;
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
	rotation.mV[6] = sinf(angle);
	rotation.mV[7] = 0.f;
		
	rotation.mV[8] = 0.f;
	rotation.mV[9] = -sinf(angle);
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
	rotation.mV[2] = -sinf(angle);
	rotation.mV[3] = 0.f;

	rotation.mV[4] = 0.f;
	rotation.mV[5] = 1.0f;
	rotation.mV[6] = 0.f;
	rotation.mV[7] = 0.f;

	rotation.mV[8] = sinf(angle);
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
	rotation.mV[1] = sinf(angle);
	rotation.mV[2] = 0.f;
	rotation.mV[3] = 0.f;

	rotation.mV[4] = -sinf(angle);
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
	tran.mV[3] = 0.f;

	tran.mV[4] = 0.f;
	tran.mV[5] = 1.0f;
	tran.mV[6] = 0.f;
	tran.mV[7] = 0.f;

	tran.mV[8] = 0.f;
	tran.mV[9] = 0.f;
	tran.mV[10] = 1.0f;
	tran.mV[11] = 0.f;

	tran.mV[12] = translation.x;
	tran.mV[13] = translation.y;
	tran.mV[14] = translation.z;
	tran.mV[15] = 1.0f;

	*this = tran;
}

void Matrix44::Scale(Vector3 scale)
{
	Matrix44 s;
	s.mV[0] = scale.x;
	s.mV[1] = 0.f;
	s.mV[2] = 0.f;
	s.mV[3] = 0.f;
	
	s.mV[4] = 0.f;
	s.mV[5] = scale.y;
	s.mV[6] = 0.f;
	s.mV[7] = 0.f;
	
	s.mV[8] = 0.f;
	s.mV[9] = 0.f;
	s.mV[10] = scale.z;
	s.mV[11] = 0.f;
	
	s.mV[12] = 0.f;
	s.mV[13] = 0.f;
	s.mV[14] = 0.f;
	s.mV[15] = 1.0f;

	*this = s;
}

Matrix44 Matrix44::PerspectiveProjection(float FOVy, float aspect, float zNear, float zFar)
{
	FOVy = FOVy * 180.0f / PI;
	Matrix44 result;
	result.mV[0] = 1.0f / (aspect * tanf(FOVy / 2.0f));
	result.mV[1] = 0.f;
	result.mV[2] = 0.f;
	result.mV[3] = 0.f;
	result.mV[4] = 0.f;
	result.mV[5] = 1.0f / tanf(FOVy / 2.0f);
	result.mV[6] = 0.f;
	result.mV[7] = 0.f;
	result.mV[8] = 0.f;
	result.mV[9] = 0.f;
	result.mV[10] = -((zFar + zNear) / (zFar - zNear));
	result.mV[11] = -((2.0f * zFar * zNear) / (zFar - zNear));
	result.mV[12] = 0.f;
	result.mV[13] = 0.f;
	result.mV[14] = -1.0f;
	result.mV[15] = 0.f;

	return result;
}

Matrix44 Matrix44::OrthographicProjection(float left, float right, float top, float bottom, float near, float far)
{
	Matrix44 result;
	result.mV[0] = 2.0f/(right - left);
	result.mV[1] = 0.f;
	result.mV[2] = 0.f;
	result.mV[3] = -((right + left)/ (right - left));
	result.mV[4] = 0.f;
	result.mV[5] = 2.0f /(top - bottom);
	result.mV[6] = 0.f;
	result.mV[7] = -((top + bottom) / (top - bottom));
	result.mV[8] = 0.f;
	result.mV[9] = 0.f;
	result.mV[10] = -(-2.0f / (far - near));
	result.mV[11] = -((far + near) / (far - near));
	result.mV[12] = 0.f;
	result.mV[13] = 0.f;
	result.mV[14] = 0.f;
	result.mV[15] = 1.0f;

	return result;
}
