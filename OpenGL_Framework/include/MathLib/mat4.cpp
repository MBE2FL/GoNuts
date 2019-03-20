#include "mat4.h"
#include "vec4.h"
#include "Quaternion.h"

mat4 mat4::Identity = mat4();

std::ostream & operator<<(std::ostream & out, const mat4& source)
{
	//Row
	for (int i = 0; i < 4; i++)
	{
		out << "| ";
		//Column
		for (int j = 0; j < 4; j++)
		{
			out << source.data[j * 4 + i] << ' ';
		}
		out << '|' << "\n";
	}
	return out;
}

mat4::mat4(const float & other1, const float & other2, const float & other3, const float & other4, 
	const float & other5, const float & other6, const float & other7, const float & other8, 
	const float & other9, const float & other10, const float & other11, const float & other12, 
	const float & other13, const float & other14, const float & other15, const float & other16)
{
	data[0] = other1;
	data[1] = other2;
	data[2] = other3;
	data[3] = other4;
	data[4] = other5;
	data[5] = other6;
	data[6] = other7;
	data[7] = other8;
	data[8] = other9;
	data[9] = other10;
	data[10] = other11;
	data[11] = other12;
	data[12] = other13;
	data[13] = other14;
	data[14] = other15;
	data[15] = other16;
}

mat4::mat4(const mat4 & other)
{
	for (size_t i = 0; i < 16; i++)
	{
		data[i] = other.data[i];
	}
}

mat4::mat4(const vec3 & position, vec3 & eulerAngles)
{
	float A = cos(eulerAngles.x);
	float B = sin(eulerAngles.x);
	float C = cos(eulerAngles.y);
	float D = sin(eulerAngles.y);
	float E = cos(eulerAngles.z);
	float F = sin(eulerAngles.z);
	float AD = A * D;
	float BD = B * D;

	// Row #1
	data[0] = C * E;
	data[4] = -C * F;
	data[8] = D;
	data[12] = position.x;

	// Row #2;
	data[1] = (BD * E) + (A * F);
	data[5] = (-BD * F) + (A * E);
	data[9] = -B * C;
	data[13] = position.y;

	// Row #3
	data[2] = (-AD * E) + (B * F);
	data[6] = (AD * F) + (B * E);
	data[10] = A * C;
	data[14] = position.z;

	// Row #4
	data[3] = 0.0f;
	data[7] = 0.0f;
	data[11] = 0.0f;
	data[15] = 1.0f;

}

mat4::mat4(const vec3 & position, Quaternion & rotation)
{
	*this = rotation.getRotationMatrix();

	data[12] = position.x;
	data[13] = position.y;
	data[14] = position.z;
	data[15] = 1.0f;
}

mat4 mat4::operator+(const mat4& rhs) const
{
	mat4 result;
	for (int i = 0; i < 16; i++)
	{
		result.data[i] = data[i] + rhs.data[i];
	}
	return result;
}

mat4 mat4::operator-(const mat4& rhs) const
{
	mat4 result;
	for (int i = 0; i < 16; i++)
	{
		result.data[i] = data[i] - rhs.data[i];
	}
	return result;
}

mat4 mat4::operator*(const mat4& rhs) const
{
	mat4 result;

	result.data[0 ] = data[0] * rhs.data[0] + data[4] * rhs.data[1] + data[8 ] * rhs.data[2] + data[12] * rhs.data[3];
	result.data[1 ] = data[1] * rhs.data[0] + data[5] * rhs.data[1] + data[9 ] * rhs.data[2] + data[13] * rhs.data[3];
	result.data[2 ] = data[2] * rhs.data[0] + data[6] * rhs.data[1] + data[10] * rhs.data[2] + data[14] * rhs.data[3];
	result.data[3 ] = data[3] * rhs.data[0] + data[7] * rhs.data[1] + data[11] * rhs.data[2] + data[15] * rhs.data[3];

	result.data[4 ] = data[0] * rhs.data[4] + data[4] * rhs.data[5] + data[8 ] * rhs.data[6] + data[12] * rhs.data[7];
	result.data[5 ] = data[1] * rhs.data[4] + data[5] * rhs.data[5] + data[9 ] * rhs.data[6] + data[13] * rhs.data[7];
	result.data[6 ] = data[2] * rhs.data[4] + data[6] * rhs.data[5] + data[10] * rhs.data[6] + data[14] * rhs.data[7];
	result.data[7 ] = data[3] * rhs.data[4] + data[7] * rhs.data[5] + data[11] * rhs.data[6] + data[15] * rhs.data[7];

	result.data[8 ] = data[0] * rhs.data[8] + data[4] * rhs.data[9] + data[8 ] * rhs.data[10] + data[12] * rhs.data[11];
	result.data[9 ] = data[1] * rhs.data[8] + data[5] * rhs.data[9] + data[9 ] * rhs.data[10] + data[13] * rhs.data[11];
	result.data[10] = data[2] * rhs.data[8] + data[6] * rhs.data[9] + data[10] * rhs.data[10] + data[14] * rhs.data[11];
	result.data[11] = data[3] * rhs.data[8] + data[7] * rhs.data[9] + data[11] * rhs.data[10] + data[15] * rhs.data[11];

	result.data[12] = data[0] * rhs.data[12] + data[4] * rhs.data[13] + data[8 ] * rhs.data[14] + data[12] * rhs.data[15];
	result.data[13] = data[1] * rhs.data[12] + data[5] * rhs.data[13] + data[9 ] * rhs.data[14] + data[13] * rhs.data[15];
	result.data[14] = data[2] * rhs.data[12] + data[6] * rhs.data[13] + data[10] * rhs.data[14] + data[14] * rhs.data[15];
	result.data[15] = data[3] * rhs.data[12] + data[7] * rhs.data[13] + data[11] * rhs.data[14] + data[15] * rhs.data[15];

	return result;
}

mat4 mat4::operator*(const float scalar) const
{
	mat4 result;
	for (int i = 0; i < 16; i++)
	{
		result.data[i] = data[i] * scalar;
	}
	return result;
}

void mat4::setRows(const vec3& row1, const vec3& row2, const vec3& row3)
{
}

void mat4::getRows(vec3& row1, vec3& row2, vec3& row3)
{
}

vec3 mat4::getRow(int i)
{
	return vec3();
}

void mat4::setColumns(const vec3& col1, const vec3& col2, const vec3& col3)
{
}

void mat4::getColumns(vec3& col1, vec3& col2, vec3& col3)
{
}

vec3 mat4::getColumn(int i)
{
	return vec3();
}

mat4 mat4::inverse(mat4 transRot)
{
	transRot.data[12] = 0.0f;
	transRot.data[13] = 0.0f;
	transRot.data[14] = 0.0f;
	transRot.data[15] = 0.0f;
	transRot.transpose(); // R^t

	vec4 rT = -(transRot * vec4(transRot.getTranslation(), 1.0f)); // -R^t * T
	transRot.data[12] = rT.x;
	transRot.data[13] = rT.y;
	transRot.data[14] = rT.z;
	transRot.data[15] = 1.0f;


	return transRot;
}

//mat4 mat4::getInverse(const mat4 & rot, const vec3 & tran)
//{
//	mat4 transRot;
//	transRot.data[15] = 0.0f;
//	transRot = rot.transpose(); // R^t
//
//	vec4 rT = -(transRot * vec4(tran, 1.0f)); // -R^t * T
//	
//	transRot.data[12] = rT.x;
//	transRot.data[13] = rT.y;
//	transRot.data[14] = rT.z;
//	transRot.data[15] = 1.0f;
//
//
//	return transRot;
//}

mat4 mat4::getInverse()
{
	mat4 transRot;
	//transRot.data[15] = 0.0f;
	//transRot = rot.transpose(); // R^t

	//vec4 rT = -(transRot * vec4(tran, 1.0f)); // -R^t * T

	//transRot.data[12] = rT.x;
	//transRot.data[13] = rT.y;
	//transRot.data[14] = rT.z;
	//transRot.data[15] = 1.0f;



	transRot = mat4(*this);
	transRot.data[12] = 0.0f;
	transRot.data[13] = 0.0f;
	transRot.data[14] = 0.0f;
	transRot.data[15] = 0.0f;
	transRot.transpose(); // R^t

	vec4 rT = -(transRot * vec4(getTranslation(), 1.0f)); // -R^t * T
	transRot.data[12] = rT.x;
	transRot.data[13] = rT.y;
	transRot.data[14] = rT.z;
	transRot.data[15] = 1.0f;


	return transRot;
}

void mat4::transpose()
{
	mat4 temp = mat4(*this);


	data[0] = temp.data[0];
	data[1] = temp.data[4];
	data[2] = temp.data[8];
	data[3] = temp.data[12];

	data[4] = temp.data[1];
	data[5] = temp.data[5];
	data[6] = temp.data[9];
	data[7] = temp.data[13];

	data[8] = temp.data[2];
	data[9] = temp.data[6];
	data[10] = temp.data[10];
	data[11] = temp.data[14];

	data[12] = temp.data[3];
	data[13] = temp.data[7];
	data[14] = temp.data[11];
	data[15] = temp.data[15];
}

//mat4 mat4::getInverse()
//{
//	float DetMOM[16];
//	DetMOM[0] = data[5] * data[10] * data[15] + 
//				data[6] * data[11] * data[13] +
//				data[7] * data[9]  * data[14] -
//				data[7] * data[10] * data[13] -
//				data[6] * data[9]  * data[15] -
//				data[5] * data[11] * data[14];
//
//	DetMOM[1] = data[4] * data[10] * data[15] +
//				data[6] * data[11] * data[12] +
//				data[7] * data[8]  * data[14] -
//				data[7] * data[10] * data[12] -
//				data[6] * data[8]  * data[15] -
//				data[4] * data[11] * data[14];
//
//	DetMOM[2] = data[4] * data[9]  * data[15] +
//				data[5] * data[11] * data[12] +
//				data[7] * data[8]  * data[13] -
//				data[7] * data[9]  * data[12] -
//				data[5] * data[8]  * data[15] -
//				data[4] * data[11] * data[13];
//
//	DetMOM[3] = data[4] * data[9]  * data[14] +
//				data[5] * data[10] * data[12] +
//				data[6] * data[8]  * data[13] -
//				data[6] * data[9]  * data[12] -
//				data[5] * data[8]  * data[14] -
//				data[4] * data[10] * data[13];
//
//	DetMOM[4] = data[1] * data[10] * data[15] +
//				data[2] * data[11] * data[13] +
//				data[3] * data[9]  * data[14] -
//				data[3] * data[10] * data[13] -
//				data[2] * data[9]  * data[15] -
//				data[1] * data[11] * data[14];
//
//	DetMOM[5] = data[0] * data[10] * data[15] +
//				data[2] * data[11] * data[12] +
//				data[3] * data[8]  * data[14] -
//				data[3] * data[10] * data[12] -
//				data[2] * data[8]  * data[15] -
//				data[0] * data[11] * data[14];
//
//	DetMOM[6] = data[0] * data[9]  * data[15] +
//				data[1] * data[11] * data[12] +
//				data[3] * data[8]  * data[13] -
//				data[3] * data[9]  * data[12] -
//				data[1] * data[8]  * data[15] -
//				data[0] * data[11] * data[13];
//
//	DetMOM[7] = data[0] * data[9]  * data[14] +
//				data[1] * data[10] * data[12] +
//				data[2] * data[8]  * data[13] -
//				data[2] * data[9]  * data[12] -
//				data[1] * data[8]  * data[14] -
//				data[0] * data[10] * data[13];
//
//	DetMOM[8] = data[1] * data[6]  * data[15] +
//				data[2] * data[7]  * data[13] +
//				data[3] * data[5]  * data[14] -
//				data[3] * data[6]  * data[13] -
//				data[2] * data[5]  * data[15] -
//				data[1] * data[7]  * data[14];
//
//	DetMOM[9] = data[0] * data[6]  * data[15] +
//				data[2] * data[7]  * data[12] +
//				data[3] * data[4]  * data[14] -
//				data[3] * data[6]  * data[12] -
//				data[2] * data[4]  * data[15] -
//				data[0] * data[7]  * data[14];
//
//	DetMOM[10] = data[0] * data[5] * data[15] +
//				data[1] * data[7]  * data[12] +
//				data[3] * data[4]  * data[13] -
//				data[3] * data[5]  * data[12] -
//				data[1] * data[4]  * data[15] -
//				data[0] * data[7]  * data[13];
//
//	DetMOM[11] = data[0] * data[5] * data[14] +
//				data[1] * data[6]  * data[12] +
//				data[2] * data[4]  * data[13] -
//				data[2] * data[5]  * data[12] -
//				data[1] * data[4]  * data[14] -
//				data[0] * data[6]  * data[13];
//
//	DetMOM[12] = data[1] * data[6] * data[11] +
//				data[2] * data[7]  * data[9]  +
//				data[3] * data[5]  * data[10] -
//				data[3] * data[6]  * data[9]  -
//				data[2] * data[5]  * data[11] -
//				data[1] * data[7]  * data[10];
//
//	DetMOM[13] = data[0] * data[6] * data[11] +
//				data[2] * data[7]  * data[8]  +
//				data[3] * data[4]  * data[10] -
//				data[3] * data[6]  * data[8]  -
//				data[2] * data[4]  * data[11] -
//				data[0] * data[7]  * data[10];
//
//	DetMOM[14] = data[0] * data[5] * data[11] +
//				data[1] * data[7]  * data[8]  +
//				data[3] * data[4]  * data[9]  -
//				data[3] * data[5]  * data[8]  -
//				data[1] * data[4]  * data[11] -
//				data[0] * data[7]  * data[9];
//
//	DetMOM[15] = data[0] * data[5] * data[10] +
//				data[1] * data[6]  * data[8]  +
//				data[2] * data[4]  * data[9]  -
//				data[2] * data[5]  * data[8]  -
//				data[1] * data[4]  * data[10] -
//				data[0] * data[6]  * data[9];
//				
//	mat4 Adjugate;
//	Adjugate.data[0]  = DetMOM[0];
//	Adjugate.data[4]  = -DetMOM[1];
//	Adjugate.data[8]  = DetMOM[2];
//	Adjugate.data[12] = -DetMOM[3];
//
//	Adjugate.data[1]  = -DetMOM[4];
//	Adjugate.data[5]  = DetMOM[5];
//	Adjugate.data[9]  = -DetMOM[6];
//	Adjugate.data[13] = DetMOM[7];
//
//	Adjugate.data[2]  = DetMOM[8];
//	Adjugate.data[6]  = -DetMOM[9];
//	Adjugate.data[10] = DetMOM[10];
//	Adjugate.data[14] = -DetMOM[11];
//
//	Adjugate.data[3]  = -DetMOM[12];
//	Adjugate.data[7]  = DetMOM[13];
//	Adjugate.data[11] = -DetMOM[14];
//	Adjugate.data[15] = DetMOM[15];
//
//	float Det = (data[0] * data[5] * data[10] * data[15]) + (data[0] * data[6] * data[11] * data[13]) + (data[0] * data[7] * data[9] * data[14])
//
//		- (data[0] * data[7] * data[10] * data[13]) - (data[0] * data[6] * data[9] * data[15]) - (data[0] * data[5] * data[11] * data[14])
//
//		- (data[1] * data[4] * data[10] * data[15]) - (data[2] * data[4] * data[11] * data[13]) - (data[3] * data[4] * data[9] * data[14])
//
//		+ (data[3] * data[4] * data[10] * data[13]) + (data[2] * data[4] * data[9] * data[15]) + (data[1] * data[4] * data[11] * data[14])
//
//		+ (data[1] * data[6] * data[8] * data[15]) + (data[2] * data[7] * data[8] * data[13]) + (data[3] * data[5] * data[8] * data[14])
//
//		- (data[3] * data[6] * data[8] * data[13]) - (data[2] * data[5] * data[8] * data[15]) - (data[1] * data[7] * data[8] * data[14])
//
//		- (data[1] * data[6] * data[11] * data[12]) - (data[2] * data[7] * data[9] * data[12]) - (data[3] * data[5] * data[10] * data[12])
//
//		+ (data[3] * data[6] * data[9] * data[12]) + (data[2] * data[5] * data[11] * data[12]) + (data[1] * data[7] * data[10] * data[12]);
//				
//	mat4 result;
//
//	result = Adjugate * (1 / Det);
//
//	return result;
//}

mat4 mat4::getTranspose() const
{
	mat4 result;


	result.data[0] = data[0];
	result.data[1] = data[4];
	result.data[2] = data[8];
	result.data[3] = data[12];

	result.data[4] = data[1];
	result.data[5] = data[5];
	result.data[6] = data[9];
	result.data[7] = data[13];

	result.data[8] = data[2];
	result.data[9] = data[6];
	result.data[10] = data[10];
	result.data[11] = data[14];

	result.data[12] = data[3];
	result.data[13] = data[7];
	result.data[14] = data[11];
	result.data[15] = data[15];


	return result;
}

void mat4::identity()
{
	data[0] = 1;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;

	data[4] = 0;
	data[5] = 1;
	data[6] = 0;
	data[7] = 0;

	data[8] = 0;
	data[9] = 0;
	data[10] = 1;
	data[11] = 0;

	data[12] = 0;
	data[13] = 0;
	data[14] = 0;
	data[15] = 1;
}

float mat4::determinant()
{
	return 0.0f;
}

void mat4::rotateX(float angle)
{
	mat4 rotation;
	rotation.data[0] = 1.0f;
	rotation.data[1] = 0.f;
	rotation.data[2] = 0.f;
	rotation.data[3] = 0.f;
		
	rotation.data[4] = 0.f;
	rotation.data[5] = cosf(angle);
	rotation.data[6] = sinf(angle);
	rotation.data[7] = 0.f;
		
	rotation.data[8] = 0.f;
	rotation.data[9] = -sinf(angle);
	rotation.data[10] = cosf(angle);
	rotation.data[11] = 0.f;
		
	rotation.data[12] = 0.f;
	rotation.data[13] = 0.f;
	rotation.data[14] = 0.f;
	rotation.data[15] = 1.0f;

	*this = rotation;
}

void mat4::rotateY(float angle)
{
	mat4 rotation;
	rotation.data[0] = cosf(angle);
	rotation.data[1] = 0.f;
	rotation.data[2] = -sinf(angle);
	rotation.data[3] = 0.f;

	rotation.data[4] = 0.f;
	rotation.data[5] = 1.0f;
	rotation.data[6] = 0.f;
	rotation.data[7] = 0.f;

	rotation.data[8] = sinf(angle);
	rotation.data[9] = 0.f;
	rotation.data[10] = cosf(angle);
	rotation.data[11] = 0.f;

	rotation.data[12] = 0.f;
	rotation.data[13] = 0.f;
	rotation.data[14] = 0.f;
	rotation.data[15] = 1.0f;

	*this = rotation;
}

void mat4::rotateZ(float angle)
{
	mat4 rotation;
	rotation.data[0] = cosf(angle);
	rotation.data[1] = sinf(angle);
	rotation.data[2] = 0.f;
	rotation.data[3] = 0.f;

	rotation.data[4] = -sinf(angle);
	rotation.data[5] = cosf(angle);
	rotation.data[6] = 0.f;
	rotation.data[7] = 0.f;

	rotation.data[8] = 0.f;
	rotation.data[9] = 0.f;
	rotation.data[10] = 1.0f;
	rotation.data[11] = 0.f;

	rotation.data[12] = 0.f;
	rotation.data[13] = 0.f;
	rotation.data[14] = 0.f;
	rotation.data[15] = 1.0f;

	*this = rotation;
}

void mat4::translate(const vec3 & translation)
{
	mat4 tran;
	tran.data[0] = 1.0f;
	tran.data[1] = 0.f;
	tran.data[2] = 0.f;
	tran.data[3] = 0.f;

	tran.data[4] = 0.f;
	tran.data[5] = 1.0f;
	tran.data[6] = 0.f;
	tran.data[7] = 0.f;

	tran.data[8] = 0.f;
	tran.data[9] = 0.f;
	tran.data[10] = 1.0f;
	tran.data[11] = 0.f;

	tran.data[12] = translation.x;
	tran.data[13] = translation.y;
	tran.data[14] = translation.z;
	tran.data[15] = 1.0f;

	*this = tran;
}

void mat4::scale(const vec3& scale)
{
	mat4 s;
	s.data[0] = scale.x;
	s.data[1] = 0.f;
	s.data[2] = 0.f;
	s.data[3] = 0.f;
	
	s.data[4] = 0.f;
	s.data[5] = scale.y;
	s.data[6] = 0.f;
	s.data[7] = 0.f;
	
	s.data[8] = 0.f;
	s.data[9] = 0.f;
	s.data[10] = scale.z;
	s.data[11] = 0.f;
	
	s.data[12] = 0.f;
	s.data[13] = 0.f;
	s.data[14] = 0.f;
	s.data[15] = 1.0f;

	*this = s;
}

vec4 mat4::transform(const mat4 & transMat, const vec4 & position)
{
	vec4 result;

	result = transMat * position;

	return result;
}

mat4 mat4::PerspectiveProjection(float FOVy, float aspect, float zNear, float zFar)
{
	FOVy = FOVy * 180.0f / PI;
	mat4 result;
	result.data[0] = 1.0f / (aspect * tanf(FOVy / 2.0f));
	result.data[1] = 0.f;
	result.data[2] = 0.f;
	result.data[3] = 0.f;
	result.data[4] = 0.f;
	result.data[5] = 1.0f / tanf(FOVy / 2.0f);
	result.data[6] = 0.f;
	result.data[7] = 0.f;
	result.data[8] = 0.f;
	result.data[9] = 0.f;
	result.data[10] = -((zFar + zNear) / (zFar - zNear));
	result.data[11] = -((2.0f * zFar * zNear) / (zFar - zNear));
	result.data[12] = 0.f;
	result.data[13] = 0.f;
	result.data[14] = -1.0f;
	result.data[15] = 0.f;

	return result;
}

mat4 mat4::OrthographicProjection(float left, float right, float bottom, float top, float near, float far)
{
	mat4 result;
	result.data[0] = 2.0f/(right - left);
	result.data[1] = 0.f;
	result.data[2] = 0.f;
	result.data[3] = (-(right + left)/ (right - left));
	
	result.data[4] = 0.f;
	result.data[5] = 2.0f /(top - bottom);
	result.data[6] = 0.f;
	result.data[7] = (-(top + bottom) / (top - bottom));
	
	result.data[8] = 0.f;
	result.data[9] = 0.f;
	result.data[10] = -2.0f / (far - near);
	result.data[11] = (-(far + near) / (far - near));
	
	result.data[12] = 0.f;
	result.data[13] = 0.f;
	result.data[14] = 0.f;
	result.data[15] = 1.0f;
	result = result.getTranspose();

	//mat4 result;
	//result.data[0] = 2.0f / (right - left);
	//result.data[1] = 0.f;
	//result.data[2] = 0.f;
	//result.data[3] = 0.f;
	//
	//result.data[4] = 0.f;
	//result.data[5] = 2.0f / (top - bottom);
	//result.data[6] = 0.f;
	//result.data[7] = 0.f;
	//
	//result.data[8] = 0.f;
	//result.data[9] = 0.f;
	//result.data[10] = -2.0f / (far - near);
	//result.data[11] = 0.f;
	//
	//result.data[12] = -(right + left)/(right - left);
	//result.data[13] = -(top + bottom)/(top - bottom);
	//result.data[14] = -(far + near) / (far - near);
	//result.data[15] = 1.0f;
	return result;
}

vec3 mat4::getTranslation() const
{
	return vec3(data[12], data[13], data[14]);
}

vec3 mat4::getForward() const
{
	return vec3(data[8], data[9], data[10]);
}

vec3 mat4::getUp() const
{
	return vec3(data[4], data[5], data[6]);
}

vec3 mat4::getLeft() const
{
	return vec3(data[0], data[1], data[2]);
}
