#include "MathLib.h"
#include <iostream>

//Vector2 vec1 = { 1,8 };
//Vector2 vec2 = { 6,1.5 };
Vector3 vec1 = { 5,2,4 };
Vector3 vec2 = { 6,3,5 };
Vector3 vec3 = { 1,1,5 };

Matrix33 m1;

MathLib myLib;


int main()
{
	//Example of all functions in the mathlib	
	/*std::cout << "======================================================================================================" << std::endl;
	std::cout << "Welcome to the mathlib tester" << std::endl;
	std::cout << "First we will display all Vectors" << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Displaying all Vectors	Vector2(x,y)	Vector3(x,y,z)" << std::endl;
	std::cout << "vec1: " << vec1 << "| vec2: " << vec2 << std::endl;	
	std::cout << "vec3: " << vec3 << "| vec4: " << vec4 << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Length of all Vectors" << std::endl;
	std::cout << "vec1 Length: " << myLib.V2.Length(vec1) << "| vec2 Length: " << myLib.V2.Length(vec2) << std::endl;
	std::cout << "vec3 Length: " << myLib.V3.Length(vec3) << "| vec4 Length: " << myLib.V3.Length(vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Unit Direction of all Vectors & Between Vectors" << std::endl;
	std::cout << "vec1 Unit Direction: " << myLib.V2.Normalize(vec1) << "| vec2 Unit Direction: " << myLib.V2.Normalize(vec2) << std::endl;
	std::cout << "Unit Direction between vec1 & vec2: " << myLib.V2.Normalize((vec2 - vec1)) << std::endl;
	std::cout << "vec3 Unit Direction: " << myLib.V3.Normalize(vec3) << "| vec4 Unit Direction: " << myLib.V3.Normalize(vec4) << std::endl;
	std::cout << "Unit Direction between vec3 & vec4: " << myLib.V3.Normalize((vec4 - vec3)) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "DotProduct of all Vectors" << std::endl;
	std::cout << "vec1 DOT vec2 = " << myLib.V2.DotProduct(vec1,vec2) << std::endl;	
	std::cout << "vec3 DOT vec4 = " << myLib.V3.DotProduct(vec3,vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Projection Length" << std::endl;
	std::cout << "vec2 on vec1 = " << myLib.V2.ProjLength(vec1, vec2) << std::endl;
	std::cout << "vec4 on vec3 = " << myLib.V3.ProjLength(vec3, vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Projection Point" << std::endl;
	std::cout << "vec2 on vec1 = " << myLib.V2.ProjPoint(vec1, vec2) << std::endl;
	std::cout << "vec4 on vec3 = " << myLib.V3.ProjPoint(vec3, vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Projection Angle" << std::endl;
	std::cout << "vec2 on vec1 = " << myLib.V2.ProjAngle(vec1, vec2) << std::endl;
	std::cout << "vec4 on vec3 = " << myLib.V3.ProjAngle(vec3, vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Distance between" << std::endl;
	std::cout << "vec1 on vec2 = " << myLib.V2.DistanceBetweenObj(vec1, vec2) << std::endl;
	std::cout << "vec3 on vec4 = " << myLib.V3.DistanceBetweenObj(vec3, vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;
	std::cout << "Cross Product" << std::endl;
	std::cout << "vec1 X vec2 = " << myLib.V2.CrossProduct(vec1, vec2) << std::endl;
	std::cout << "vec3 X vec4 = " << myLib.V3.CrossProduct(vec3, vec4) << std::endl;
	std::cout << "======================================================================================================" << std::endl;*/

	m1.SetRows(vec1, vec2, vec3);
	std::cout << m1 << std::endl;
	m1.Transpose();
	std::cout << m1 << std::endl;
	m1 = m1 * 9001;
	std::cout << m1 << std::endl;
	std::cin.ignore();
	return 0;
}