#include "Vector4D.h"

#include "Vector3D.h"
#include "Matrix4x4.h"

Vector4D::Vector4D() : 
	x(0), y(0), z(0), w(0)
{

}

Vector4D::Vector4D(float _x, float _y, float _z, float _w) :
	x(_x), y(_y), z(_z), w(_w)
{

}

Vector4D::Vector4D(const Vector4D& other) :
	x(other.x), y(other.y), z(other.z), w(other.w)
{

}

Vector4D::Vector4D(const Vector4D&& other) noexcept :
	x(other.x), y(other.y), z(other.z), w(other.w)
{

}

Vector4D::Vector4D(const Vector3D& other, const float _w) :
	x(other.x), y(other.y), z(other.z), w(_w)
{

}

Vector4D::Vector4D(const Vector3D&& other, const float _w) noexcept :
	x(other.x), y(other.y), z(other.y), w(_w)
{

}

Vector4D::~Vector4D()
{

}

float Vector4D::FastInvSqrt(float number) const
{
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
	//	i = 0x5f3759df - (i >> 1);
	i = 0x5f375a86 - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	//	y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

	return y;
}

Vector4D Vector4D::Normalize3()
{
	float size = FastInvSqrt(x * x + y * y + z * z);

	return Vector4D(this->x * size, this->y * size, this->z * size, 0.f);
}

Vector4D Vector4D::Normalize()
{
	float size = FastInvSqrt(x * x + y * y + z * z);

	return Vector4D(this->x * size, this->y * size, this->z * size, this->w);
}

void Vector4D::DivideW()
{
	this->x = this->x / this->w;
	this->y = this->y / this->w;
	this->z = this->z / this->w;
	this->w = this->w / this->w;
}

float Vector4D::Dot(const Vector4D& other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector4D Vector4D::Cross(const Vector4D& other)
{
	return Vector4D(
		(this->y * other.z - this->z * other.y),
		(this->z * other.x - this->x * other.z),
		(this->x * other.y - this->y * other.x));
}

Vector4D Vector4D::operator+(const Vector4D& other) const
{
	if (this->w + other.w >= 2.f)
	{
		return Vector4D();
	}

	return Vector4D(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
}

Vector4D& Vector4D::operator+=(const Vector4D& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	this->w += other.w;

	return *this;
}

Vector4D Vector4D::operator-() const
{
	return Vector4D();
}

Vector4D Vector4D::operator-(const Vector4D& other) const
{
	return Vector4D(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
}

Vector4D& Vector4D::operator-=(const Vector4D& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
	this->w -= other.w;

	return *this;
}

Vector4D& Vector4D::operator=(const Vector4D& other)
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	return *this;
}

Vector4D& Vector4D::operator=(const Vector4D&& other) noexcept
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	return *this;
}

Vector4D& Vector4D::operator=(const Vector3D& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = 0.f;

	return *this;
}

Vector4D& Vector4D::operator=(const Vector3D&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	this->w = 0.f;

	return *this;
}

/// <summary>
/// w 부분 참고해야함.
/// </summary>
/// <param name="n"></param>
/// <param name="other"></param>
/// <returns></returns>
Vector4D operator * (float n, const Vector4D& other)
{
	return Vector4D(n * other.x, n * other.y, n * other.z, 0.f);
}

Vector4D operator * (const Vector4D& other, float n)
{
	return Vector4D(n * other.x, n * other.y, n * other.z, 0.f);
}

Vector4D& Vector4D::operator*=(float n)
{
	this->x *= n;
	this->y *= n;
	this->z *= n;
	this->w = w;

	return *this;
}

Vector4D operator / (float n, const Vector4D& other)
{
	return Vector4D(n / other.x, n / other.y, n / other.z, 0.f);
}

Vector4D operator / (const Vector4D& other, float n)
{
	return Vector4D(n / other.x, n / other.y, n / other.z, 0.f);
}

Vector4D& Vector4D::operator /= (float n)
{
	this->x /= n;
	this->y /= n;
	this->z /= n;
	this->w = 0.f;

	return *this;
}

Vector4D Vector4D::operator*(const Matrix4x4& other) const
{
	return Vector4D(
		(this->x * other.e[0][0] + this->y * other.e[1][0] + this->z * other.e[2][0] + this->w * other.e[3][0]),
		(this->x * other.e[0][1] + this->y * other.e[1][1] + this->z * other.e[2][1] + this->w * other.e[3][1]),
		(this->x * other.e[0][2] + this->y * other.e[1][2] + this->z * other.e[2][2] + this->w * other.e[3][2]),
		(this->x * other.e[0][3] + this->y * other.e[1][3] + this->z * other.e[2][3] + this->w * other.e[3][3])
	);
}