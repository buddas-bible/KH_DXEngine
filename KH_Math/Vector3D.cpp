#include "Vector3D.h"
#include "Matrix3x3.h"
#include <cmath>
#include <utility>

#include "Vector2D.h"
#include "Vector4D.h"

Vector3D::Vector3D() noexcept :
	x(0.f), y(0.f), z(0.f)
{

}

constexpr Vector3D::Vector3D(float _x, float _y, float _z) noexcept :
	x(_x), y(_y), z(_z)
{

}

Vector3D::Vector3D(const Vector3D& other) noexcept :
	x(other.x), y(other.y), z(other.z)
{

}

Vector3D::Vector3D(const Vector3D&& other) noexcept :
	x(other.x), y(other.y), z(other.z)
{

}

Vector3D::Vector3D(const Vector2D& other) noexcept :
	x(other.x), y(other.y), z(0.f)
{

}

Vector3D::Vector3D(const Vector2D&& other) noexcept :
	x(other.x), y(other.y), z(0)
{

}

Vector3D::Vector3D(const Vector4D& other) noexcept :
	x(other.x), y(other.y), z(other.z)
{

}

Vector3D::Vector3D(const Vector4D&& other) noexcept :
	x(other.x), y(other.y), z(other.z)
{

}

Vector3D::Vector3D(_In_reads_(3) const float* pArr) noexcept :
	x(pArr[0]), y(pArr[1]), z(pArr[2])
{

}

Vector3D::~Vector3D() noexcept
{

}

/// <summary>
/// º¤ÅÍ Å©±â ¹ÝÈ¯ÇÏ´Â ÇÔ¼ö
/// </summary>
/// <returns></returns>
float Vector3D::Size() const
{
	return powf((x * x) + (y * y) + (z * z), 0.5);
}

float Vector3D::SizeSquare() const
{
	return (x * x) + (y * y) + (z * z);
}

/// <summary>
/// º¤ÅÍ ¿ÜÀû
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::Cross(const Vector3D& other) const
{
	return Vector3D(
		(this->y * other.z - this->z * other.y),
		(this->z * other.x - this->x * other.z),
		(this->x * other.y - this->y * other.x));
}

/// <summary>
/// º¤ÅÍ ³»Àû
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::Dot(const Vector3D& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

/// <summary>
/// °í¼Ó ¿ª Á¦°ö±Ù
/// from.À§Å°ÇÇµð¾Æ
/// </summary>
/// <param name="number"></param>
/// <returns></returns>
float Vector3D::FastInvSqrt(float number) const
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

/// <summary>
/// º¤ÅÍ Åõ¿µ
/// other º¤ÅÍ¿¡ Åõ¿µ
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::Projection(const Vector3D& other)
{
	Vector3D v2 = other.Normalize();

	return Dot(v2);
}

/// <summary>
/// »çÀÌ°¢
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
float Vector3D::FindTheta(const Vector3D& other) const
{
	Vector3D v1 = Normalize();
	Vector3D v2 = other.Normalize();

	float dot = v1.Dot(v2);

	if (dot > 1.0f)
		dot = 1.0f;
	if (dot < -1.0f)
		dot = -1.0f;

	dot = floor(dot * 10000000) / 10000000;

	return acos(dot);
}

/// <summary>
/// Á¤±ÔÈ­
/// </summary>
/// <returns></returns>
Vector3D Vector3D::Normalize() const &
{
	float temp = x * x + y * y + z * z;
	if (temp == 0)
	{
		return Vector3D(0.f, 0.f, 0.f);
	}
	else
	{
		float invSqrt = FastInvSqrt(temp);
		return Vector3D(x * invSqrt, y * invSqrt, z * invSqrt);
	}
}

Vector3D Vector3D::Normalize() &&
{
	float temp = x * x + y * y + z * z;
	if (temp == 0)
	{
		return Vector3D(0.f, 0.f, 0.f);
	}
	else
	{
		float invSqrt = FastInvSqrt(temp);
		x *= invSqrt;
		y *= invSqrt;
		z *= invSqrt;
		return std::move(*this);
	}
}

Vector3D& Vector3D::operator=(const Vector3D& other) noexcept
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	return *this;
}

Vector3D& Vector3D::operator=(const Vector3D&& other) noexcept
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	return *this;
}


Vector3D& Vector3D::operator=(const Vector2D& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = 0.f;

	return *this;
}

Vector3D& Vector3D::operator=(const Vector2D&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = 0.f;

	return *this;
}

Vector3D& Vector3D::operator=(const Vector4D& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}

Vector3D& Vector3D::operator=(const Vector4D&& other) noexcept
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;

	return *this;
}

/// <summary>
/// º¤ÅÍ µ¡¼À
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator+(const Vector3D& other) const
{
	return Vector3D(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3D& Vector3D::operator+=(const Vector3D& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return *this;
}

/// <summary>
/// º¤ÅÍ »¬¼À
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator-() const
{
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
	return Vector3D(this->x - other.x, this->y - other.y, this->z - other.z);
}

Vector3D& Vector3D::operator-=(const Vector3D& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;

	return *this;
}

/// <summary>
/// º¤ÅÍ ºñ±³
/// </summary>
/// <param name="other"></param>
/// <returns></returns>
bool Vector3D::operator==(const Vector3D& other) const
{
	return (this->x == other.x && this->y == other.y && this->z == other.z);
}

bool Vector3D::operator<(const Vector3D& other) const
{
	return (this->x < other.x && this->y < other.y && this->z < other.z);
}

bool Vector3D::operator>(const Vector3D& other) const
{
	return (this->x > other.x && this->y > other.y && this->z > other.z);
}

/// <summary>
/// º¤ÅÍ ½Ç¼ö¹è
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
Vector3D operator*(float n, const Vector3D& v)
{
	Vector3D temp;
	temp.x = n * v.x;
	temp.y = n * v.y;
	temp.z = n * v.z;

	return temp;
}

Vector3D operator*(const Vector3D& v, float n)
{
	Vector3D temp;
	temp.x = v.x * n;
	temp.y = v.y * n;
	temp.z = v.z * n;

	return temp;
}

Vector3D& Vector3D::operator*=(float n)
{
	this->x *= n;
	this->y *= n;
	this->z *= n;

	return *this;
}

Vector3D operator/(float n, const Vector3D& v)
{
	Vector3D temp;
	temp.x = n / v.x;
	temp.y = n / v.y;
	temp.z = n / v.z;

	return temp;
}

Vector3D operator/(const Vector3D& v, float n)
{
	Vector3D temp;
	temp.x = v.x / n;
	temp.y = v.y / n;
	temp.z = v.z / n;

	return temp;
}

Vector3D& Vector3D::operator/=(float n)
{
	this->x /= n;
	this->y /= n;
	this->z /= n;

	return *this;
}

/// <summary>
/// º¤ÅÍ * Çà·Ä
/// </summary>
/// <param name="v"></param>
/// <param name="other"></param>
/// <returns></returns>
Vector3D Vector3D::operator*(const Matrix3x3& other) const
{
	return Vector3D(
		(this->x * other.e[0][0] + this->y * other.e[1][0] + this->z * other.e[2][0]),
		(this->x * other.e[0][1] + this->y * other.e[1][1] + this->z * other.e[2][1]),
		(this->x * other.e[0][2] + this->y * other.e[1][2] + this->z * other.e[2][2])
	);
}

/*
Vector3D Vector3D::operator*(const Matrix4x4& other) const
{
	return Vector3D
}
*/

Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c)
{
	return a.Cross(b).Cross(c);
}

