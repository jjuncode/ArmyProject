#include "pch.h"
#include "Core.h"

int main()
{
	Mat3 mat;
	mat = Mat3(Vec3(1, 2, 3), Vec3(4, 5, 6), Vec3(7, 8, 9));
	std::cout << mat << std::endl;

	Vec3 vec1(1, 2, 3);
	auto temp = mat * vec1;
	std::cout << "mat * vec1 = " << temp << std::endl;

	// Core::Init(800,800);
	// Core::Loop();
}  