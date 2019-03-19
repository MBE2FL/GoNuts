#pragma once
#include "vec3.h"
//#include "vec4.h"
#include <ostream>

class vec4;
class Quaternion;

class mat4
{
public:
	float data[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
	mat4() {};
	mat4(const float& other1, const float& other2, const float& other3, const float& other4, 
		const float& other5, const float& other6, const float& other7, const float& other8, 
		const float& other9, const float& other10, const float& other11, const float& other12, 
		const float& other13, const float& other14, const float& other15, const float& other16);
	mat4(const mat4& other);
	mat4(const vec3& position, vec3& eulerAngles);
	mat4(const vec3& position, Quaternion& rotation);
	~mat4() {};
	//Text Output
	friend std::ostream& operator<<(std::ostream& out, const mat4& source);
	//Basic Operations
	mat4 operator+(const mat4& rhs) const;
	mat4 operator-(const mat4& rhs) const;
	mat4 operator*(const mat4& rhs) const;
	mat4 operator*(const float scalar) const;
	//Row Manipulators
	void setRows(const vec3& row1, const vec3& row2, const vec3& row3);
	void getRows(vec3& row1, vec3& row2, vec3& row3);
	vec3 getRow(int i);
	//Column Manipulators
	void setColumns(const vec3& col1, const vec3& col2, const vec3& col3);
	void getColumns(vec3& col1, vec3& col2, vec3& col3);
	vec3 getColumn(int i);
	//Manipulators
	void inverse();
	//mat4 getInverse(const mat4& rot, const vec3& tran);
	mat4 getInverse();
	void transpose();
	mat4 getTranspose() const;
	void identity();
	float determinant();

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void translate(const vec3& translation);
	void scale(const vec3& scale);
	static vec4 transform(const mat4& transMat, const vec4& position);

	static mat4 PerspectiveProjection(float FOVy, float aspect, float zNear, float zFar);
	static mat4 OrthographicProjection(float left, float right, float bottom, float top, float near, float far);

	vec3 getTranslation() const;
	vec3 getForward() const;
	vec3 getUp() const;
	vec3 getLeft() const;

	static mat4 Identity;
};
