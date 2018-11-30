#include "MathLib.h"
#include <iostream>

Vector3 vec1 = { 5,2,4 };
Vector3 vec2 = { 6,3,5 };
Vector3 vec3 = { 1,1,5 };

Matrix33 m1;

//MathLib myLib;


int main()
{
	m1.SetRows(vec1, vec2, vec3);
	std::cout << m1 << std::endl;
	m1.Transpose();
	std::cout << m1 << std::endl;
	m1 = m1 * 9001;
	std::cout << m1 << std::endl;
	Matrix44 m,mm;
	for (int i = 0; i < 16; i++)
	{
		m.mV[i] = i + 1; mm.mV[i] = i + 1;
	}
	std::cout << m << std::endl;
	std::cout << mm << std::endl;
	std::cout << m*mm << std::endl;

	Vector4 v(1, 2, 3, 4);
	std::cout << v << std::endl;
	std::cout << m * v << std::endl;


	std::cin.ignore();
	return 0;
}