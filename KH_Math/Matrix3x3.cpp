#include "Matrix3x3.h"
#include <cmath>

#include "Vector3D.h"

Matrix3x3::Matrix3x3() :
	e{
	(1), (0), (0),
	(0), (1), (0),
	(0), (0), (1) }
{

}

constexpr Matrix3x3::Matrix3x3(
	float e00, float e01, float e02,
	float e10, float e11, float e12,
	float e20, float e21, float e22) :
	e{
	(e00), (e01), (e02),
	(e10), (e11), (e12),
	(e20), (e21), (e22) }
{

}

Matrix3x3::Matrix3x3(const Matrix3x3& mat) :
	e{
	(mat.e[0][0]), (mat.e[0][1]), (mat.e[0][2]),
	(mat.e[1][0]), (mat.e[1][1]), (mat.e[1][2]),
	(mat.e[2][0]), (mat.e[2][1]), (mat.e[2][2]) }
{

}

Matrix3x3::Matrix3x3(const Matrix3x3&& mat) noexcept :
	e{
	(mat.e[0][0]), (mat.e[0][1]), (mat.e[0][2]),
	(mat.e[1][0]), (mat.e[1][1]), (mat.e[1][2]),
	(mat.e[2][0]), (mat.e[2][1]), (mat.e[2][2]) }
{

}

Matrix3x3::~Matrix3x3()
{

}

Matrix3x3 Matrix3x3::IdentityMatrix()
{
	return Matrix3x3();
}

Matrix3x3 Matrix3x3::TransformMatrix(float moveX, float moveY, float theta, float scaleX, float scaleY)
{
	return Matrix3x3(
		(scaleX * cos(theta)), (scaleX * sin(theta)), (0),
		(-scaleY * sin(theta)), (scaleY * cos(theta)), (0),
		(moveX), (moveY), (1)
	);
}

Matrix3x3 Matrix3x3::TranslateMatrix(float moveX, float moveY)
{
	return Matrix3x3(
		(1), (0), (0),
		(0), (1), (0),
		(moveX), (moveY), (1)
	);
}

Matrix3x3 Matrix3x3::RotateMatrix(float theta)
{
	return Matrix3x3(
		(cos(theta)), (sin(theta)), (0),
		(-sin(theta)), (cos(theta)), (0),
		(0), (0), (1)
	);
}


Matrix3x3 Matrix3x3::ScaleMatrix(float scaleX, float scaleY)
{
	return Matrix3x3(
		(scaleX), (0), (0),
		(0), (scaleY), (0),
		(0), (0), (1)
	);
}

Matrix3x3 Matrix3x3::ScaleRotate(float scaleX, float scaleY, float theta)
{
	return Matrix3x3(
		(scaleX * cos(theta)), (scaleX * sin(theta)), (0),
		(-scaleY * sin(theta)), (scaleY * cos(theta)), (0),
		(0), (0), (1)
	);
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] + other.e[0][0]), 
		(this->e[0][1] + other.e[0][1]), 
		(this->e[0][2] + other.e[0][2]),

		(this->e[1][0] + other.e[1][0]), 
		(this->e[1][1] + other.e[1][1]), 
		(this->e[1][2] + other.e[1][2]),

		(this->e[2][0] + other.e[2][0]), 
		(this->e[2][1] + other.e[2][1]), 
		(this->e[2][2] + other.e[2][2])
	);
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] - other.e[0][0]), 
		(this->e[0][1] - other.e[0][1]), 
		(this->e[0][2] - other.e[0][2]),

		(this->e[1][0] - other.e[1][0]), 
		(this->e[1][1] - other.e[1][1]), 
		(this->e[1][2] - other.e[1][2]),

		(this->e[2][0] - other.e[2][0]), 
		(this->e[2][1] - other.e[2][1]), 
		(this->e[2][2] - other.e[2][2])
	);
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& other) const
{
	return Matrix3x3(
		(this->e[0][0] * other.e[0][0] + this->e[0][1] * other.e[1][0] + this->e[0][2] * other.e[2][0]),
		(this->e[0][0] * other.e[0][1] + this->e[0][1] * other.e[1][1] + this->e[0][2] * other.e[2][1]),
		(this->e[0][0] * other.e[0][2] + this->e[0][1] * other.e[1][2] + this->e[0][2] * other.e[2][2]),

		(this->e[1][0] * other.e[0][0] + this->e[1][1] * other.e[1][0] + this->e[1][2] * other.e[2][0]),
		(this->e[1][0] * other.e[0][1] + this->e[1][1] * other.e[1][1] + this->e[1][2] * other.e[2][1]),
		(this->e[1][0] * other.e[0][2] + this->e[1][1] * other.e[1][2] + this->e[1][2] * other.e[2][2]),

		(this->e[2][0] * other.e[0][0] + this->e[2][1] * other.e[1][0] + this->e[2][2] * other.e[2][0]),
		(this->e[2][0] * other.e[0][1] + this->e[2][1] * other.e[1][1] + this->e[2][2] * other.e[2][1]),
		(this->e[2][0] * other.e[0][2] + this->e[2][1] * other.e[1][2] + this->e[2][2] * other.e[2][2])
	);
}

Matrix3x3 Matrix3x3::operator=(const Matrix3x3& other)
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];
		this->e[0][2] = other.e[0][2];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
		this->e[1][2] = other.e[1][2];

		this->e[2][0] = other.e[2][0];
		this->e[2][1] = other.e[2][1];
		this->e[2][2] = other.e[2][2];
	}

	return *this;
}

Matrix3x3 Matrix3x3::operator=(const Matrix3x3&& other) noexcept
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];
		this->e[0][2] = other.e[0][2];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
		this->e[1][2] = other.e[1][2];

		this->e[2][0] = other.e[2][0];
		this->e[2][1] = other.e[2][1];
		this->e[2][2] = other.e[2][2];
	}

	return *this;
}


Matrix3x3 AxisAndAngleRotation(const Vector3D& vec, float angle)
{
	Vector3D axis = vec;

	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float x = vec.x;
	float y = vec.y;
	float z = vec.z;

	return Matrix3x3(
		cos + x * x * (1 - cos),
		x * y * (1 - cos) - z * sin,
		x * z * (1 - cos) + y * sin,

		y * x * (1 - cos) + z * sin,
		cos + y * y * (1 - cos),
		y * z * (1- cos) - x * sin,

		z * x * (1 - cos) - y * sin,
		z * y * (1 - cos) + x * sin,
		cos + z * z * (1 - cos)
	);
}