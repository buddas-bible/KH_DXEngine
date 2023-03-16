#include "Matrix2x2.h"
#include <cmath>

Matrix2x2::Matrix2x2() : 
	e{
	(1), (0),
	(0), (1) }
{

}

Matrix2x2::Matrix2x2(
	float e00, float e01, 
	float e10, float e11) :
	e{
	(e00), (e01),
	(e10), (e11) }
{

}

Matrix2x2::Matrix2x2(const Matrix2x2& mat) :
	e{
	(mat.e[0][0]), (mat.e[0][1]),
	(mat.e[1][0]), (mat.e[1][1]) }
{

}

Matrix2x2::Matrix2x2(const Matrix2x2&& mat) noexcept :
	e{
	(mat.e[0][0]), (mat.e[0][1]),
	(mat.e[1][0]), (mat.e[1][1]) }
{

}

Matrix2x2::~Matrix2x2()
{

}

Matrix2x2 Matrix2x2::IdentityMatrix()
{
	return Matrix2x2();
}

Matrix2x2 Matrix2x2::RotateMatrix(float theta)
{
	return Matrix2x2(
		(cos(theta)), (sin(theta)),
		(-sin(theta)), (cos(theta))
	);
}

Matrix2x2 Matrix2x2::ScaleMatrix(float scaleX, float scaleY)
{
	return Matrix2x2(
		(scaleX), (0),
		(0), (scaleY)
	);
}

Matrix2x2 Matrix2x2::ScaleRotate(float scaleX, float scaleY, float theta)
{
	return Matrix2x2(
		(scaleX * cos(theta)), (scaleX * sin(theta)),
		(-scaleY * sin(theta)), (scaleY * cos(theta))
	);
}

Matrix2x2 Matrix2x2::operator+(const Matrix2x2& other) const
{
	return Matrix2x2(
		(this->e[0][0] + other.e[0][0]), (this->e[0][1] + other.e[0][1]),
		(this->e[1][0] + other.e[1][0]), (this->e[1][1] + other.e[1][1])
	);
}

Matrix2x2 Matrix2x2::operator-(const Matrix2x2& other) const
{
	return Matrix2x2(
		(this->e[0][0] - other.e[0][0]), (this->e[0][1] - other.e[0][1]),
		(this->e[1][0] - other.e[1][0]), (this->e[1][1] - other.e[1][1])
	);
}

Matrix2x2 Matrix2x2::operator*(const Matrix2x2& other) const
{
	return Matrix2x2(
		(this->e[0][0] * other.e[0][0] + this->e[0][1] * other.e[1][0]),
		(this->e[0][0] * other.e[0][1] + this->e[0][1] * other.e[1][1]),

		(this->e[1][0] * other.e[0][0] + this->e[1][1] * other.e[1][0]),
		(this->e[1][0] * other.e[0][1] + this->e[1][1] * other.e[1][1])
	);
}

Matrix2x2& Matrix2x2::operator=(const Matrix2x2& other)
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
	}

	return *this;
}

Matrix2x2& Matrix2x2::operator=(const Matrix2x2&& other) noexcept
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
	}

	return *this;
}