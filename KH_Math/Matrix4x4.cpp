#include "Matrix4x4.h"
#include <cmath>

Matrix4x4::Matrix4x4() :
	e{
	(1), (0), (0), (0),
	(0), (1), (0), (0),
	(0), (0), (1), (0),
	(0), (0), (0), (1) }
{

}

Matrix4x4::Matrix4x4(
	float e00, float e01, float e02, float e03, 
	float e10, float e11, float e12, float e13, 
	float e20, float e21, float e22, float e23, 
	float e30, float e31, float e32, float e33) :
	e{
	(e00), (e01), (e02), (e03),
	(e10), (e11), (e12), (e13),
	(e20), (e21), (e22), (e23),
	(e30), (e31), (e32), (e33) }
{

}

Matrix4x4::Matrix4x4(const Matrix4x4& mat) :
	e{
	(mat.e[0][0]), (mat.e[0][1]), (mat.e[0][2]), (mat.e[0][3]),
	(mat.e[1][0]), (mat.e[1][1]), (mat.e[1][2]), (mat.e[1][3]),
	(mat.e[2][0]), (mat.e[2][1]), (mat.e[2][2]), (mat.e[2][3]),
	(mat.e[3][0]), (mat.e[3][1]), (mat.e[3][2]), (mat.e[3][3]) }
{

}

Matrix4x4::Matrix4x4(const Matrix4x4&& mat) noexcept :
	e{
	(mat.e[0][0]), (mat.e[0][1]), (mat.e[0][2]), (mat.e[0][3]),
	(mat.e[1][0]), (mat.e[1][1]), (mat.e[1][2]), (mat.e[1][3]),
	(mat.e[2][0]), (mat.e[2][1]), (mat.e[2][2]), (mat.e[2][3]),
	(mat.e[3][0]), (mat.e[3][1]), (mat.e[3][2]), (mat.e[3][3]) }
{

}

Matrix4x4::~Matrix4x4()
{

}

Matrix4x4 Matrix4x4::IdentityMatrix()
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::TransformMatrix(
	float moveX, float moveY, float moveZ, 
	float thetaX, float thetaY, float thetaZ,
	float scaleX, float scaleY, float scaleZ)
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::TranslateMatrix(float moveX, float moveY, float moveZ)
{
	return Matrix4x4(
		(1), (0), (0), (0),
		(0), (1), (0), (0),
		(0), (0), (1), (0),
		(moveX), (moveY), (moveZ), (1)
	);
}

Matrix4x4 Matrix4x4::RotateMatrix(float thetaX, float thetaY, float thetaZ)
{
	return Matrix4x4(
		(cos(thetaZ) * cos(thetaY)), 
		(cos(thetaZ) * sin(thetaY) * sin(thetaX) - sin(thetaZ) * cos(thetaX)), 
		(cos(thetaZ) * sin(thetaY) * cos(thetaX) + sin(thetaZ) * sin(thetaX)),
		(0),
		
		(sin(thetaZ) * cos(thetaY)), 
		(sin(thetaZ) * sin(thetaY) * sin(thetaX) + cos(thetaY) * cos(thetaX)), 
		(sin(thetaZ) * sin(thetaY) * cos(thetaX) - cos(thetaY) * sin(thetaX)), 
		(0),

		(-sin(thetaY)), 
		(cos(thetaY) * cos(thetaX)), 
		(cos(thetaY) * cos(thetaX)), 
		(0),

		(0), 
		(0), 
		(0), 
		(1)
	);
}

Matrix4x4 Matrix4x4::RotateMatrix(const Quaternion& q)
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::ScaleMatrix(float scaleX, float scaleY, float scaleZ)
{
	return Matrix4x4(
		(scaleX), (0), (0), (0),
		(0), (scaleY), (0), (0),
		(0), (0), (scaleZ), (0),
		(0), (0), (0), (1)
	);
}

Matrix4x4 Matrix4x4::ScaleRotate(
	float scaleX, float scaleY, float scaleZ, 
	float thetaX, float thetaY, float thetaZ)
{
	return Matrix4x4();
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
	return Matrix4x4(
		(this->e[0][0] + other.e[0][0]), 
		(this->e[0][1] + other.e[0][1]), 
		(this->e[0][2] + other.e[0][2]),
		(this->e[0][3] + other.e[0][3]),

		(this->e[1][0] + other.e[1][0]), 
		(this->e[1][1] + other.e[1][1]), 
		(this->e[1][2] + other.e[1][2]),
		(this->e[1][3] + other.e[1][3]),

		(this->e[2][0] + other.e[2][0]), 
		(this->e[2][1] + other.e[2][1]), 
		(this->e[2][2] + other.e[2][2]),
		(this->e[2][3] + other.e[2][3]),

		(this->e[3][0] + other.e[3][0]),
		(this->e[3][1] + other.e[3][1]),
		(this->e[3][2] + other.e[3][2]),
		(this->e[3][3] + other.e[3][3])
	);
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
	return Matrix4x4(
		(this->e[0][0] - other.e[0][0]),
		(this->e[0][1] - other.e[0][1]),
		(this->e[0][2] - other.e[0][2]),
		(this->e[0][3] - other.e[0][3]),

		(this->e[1][0] - other.e[1][0]),
		(this->e[1][1] - other.e[1][1]),
		(this->e[1][2] - other.e[1][2]),
		(this->e[1][3] - other.e[1][3]),

		(this->e[2][0] - other.e[2][0]),
		(this->e[2][1] - other.e[2][1]),
		(this->e[2][2] - other.e[2][2]),
		(this->e[2][3] - other.e[2][3]),

		(this->e[3][0] - other.e[3][0]),
		(this->e[3][1] - other.e[3][1]),
		(this->e[3][2] - other.e[3][2]),
		(this->e[3][3] - other.e[3][3])
	);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	return Matrix4x4(
		(this->e[0][0] * other.e[0][0] + this->e[0][1] * other.e[1][0] + this->e[0][2] * other.e[2][0] + this->e[0][3] * other.e[3][0]),
		(this->e[0][0] * other.e[0][1] + this->e[0][1] * other.e[1][1] + this->e[0][2] * other.e[2][1] + this->e[0][3] * other.e[3][1]),
		(this->e[0][0] * other.e[0][2] + this->e[0][1] * other.e[1][2] + this->e[0][2] * other.e[2][2] + this->e[0][3] * other.e[3][2]),
		(this->e[0][0] * other.e[0][3] + this->e[0][1] * other.e[1][3] + this->e[0][2] * other.e[2][3] + this->e[0][3] * other.e[3][3]),

		(this->e[1][0] * other.e[0][0] + this->e[1][1] * other.e[1][0] + this->e[1][2] * other.e[2][0] + this->e[1][3] * other.e[3][0]),
		(this->e[1][0] * other.e[0][1] + this->e[1][1] * other.e[1][1] + this->e[1][2] * other.e[2][1] + this->e[1][3] * other.e[3][1]),
		(this->e[1][0] * other.e[0][2] + this->e[1][1] * other.e[1][2] + this->e[1][2] * other.e[2][2] + this->e[1][3] * other.e[3][2]),
		(this->e[1][0] * other.e[0][3] + this->e[1][1] * other.e[1][3] + this->e[1][2] * other.e[2][3] + this->e[1][3] * other.e[3][3]),

		(this->e[2][0] * other.e[0][0] + this->e[2][1] * other.e[1][0] + this->e[2][2] * other.e[2][0] + this->e[2][3] * other.e[3][0]),
		(this->e[2][0] * other.e[0][1] + this->e[2][1] * other.e[1][1] + this->e[2][2] * other.e[2][1] + this->e[2][3] * other.e[3][1]),
		(this->e[2][0] * other.e[0][2] + this->e[2][1] * other.e[1][2] + this->e[2][2] * other.e[2][2] + this->e[2][3] * other.e[3][2]),
		(this->e[2][0] * other.e[0][3] + this->e[2][1] * other.e[1][3] + this->e[2][2] * other.e[2][3] + this->e[2][3] * other.e[3][3]),

		(this->e[3][0] * other.e[0][0] + this->e[3][1] * other.e[1][0] + this->e[3][2] * other.e[2][0] + this->e[3][3] * other.e[3][0]),
		(this->e[3][0] * other.e[0][1] + this->e[3][1] * other.e[1][1] + this->e[3][2] * other.e[2][1] + this->e[3][3] * other.e[3][1]),
		(this->e[3][0] * other.e[0][2] + this->e[3][1] * other.e[1][2] + this->e[3][2] * other.e[2][2] + this->e[3][3] * other.e[3][2]),
		(this->e[3][0] * other.e[0][3] + this->e[3][1] * other.e[1][3] + this->e[3][2] * other.e[2][3] + this->e[3][3] * other.e[3][3])
	);
}

Matrix4x4 Matrix4x4::operator=(const Matrix4x4& other)
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];
		this->e[0][2] = other.e[0][2];
		this->e[0][3] = other.e[0][3];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
		this->e[1][2] = other.e[1][2];
		this->e[1][3] = other.e[1][3];

		this->e[2][0] = other.e[2][0];
		this->e[2][1] = other.e[2][1];
		this->e[2][2] = other.e[2][2];
		this->e[2][3] = other.e[2][3];

		this->e[3][0] = other.e[3][0];
		this->e[3][1] = other.e[3][1];
		this->e[3][2] = other.e[3][2];
		this->e[3][3] = other.e[3][3];
	}

	return *this;
}

Matrix4x4 Matrix4x4::operator=(const Matrix4x4&& other) noexcept
{
	if (this != &other)
	{
		this->e[0][0] = other.e[0][0];
		this->e[0][1] = other.e[0][1];
		this->e[0][2] = other.e[0][2];
		this->e[0][3] = other.e[0][3];

		this->e[1][0] = other.e[1][0];
		this->e[1][1] = other.e[1][1];
		this->e[1][2] = other.e[1][2];
		this->e[1][3] = other.e[1][3];

		this->e[2][0] = other.e[2][0];
		this->e[2][1] = other.e[2][1];
		this->e[2][2] = other.e[2][2];
		this->e[2][3] = other.e[2][3];

		this->e[3][0] = other.e[3][0];
		this->e[3][1] = other.e[3][1];
		this->e[3][2] = other.e[3][2];
		this->e[3][3] = other.e[3][3];
	}

	return *this;
}