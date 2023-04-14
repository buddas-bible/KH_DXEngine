#pragma once

#include <sal.h>

class Matrix2x2;
class Matrix3x3;
class Matrix4x4;
class Vector2D;
class Vector3D;

class Vector4D
{
public:
	Vector4D() noexcept;
	constexpr Vector4D(float, float, float, float = 0.f) noexcept;
	Vector4D(_In_reads_(4) const float* pArr) noexcept;
	Vector4D(const Vector4D& other) noexcept;						// 복사 생성자
	Vector4D(const Vector4D&& other) noexcept;						// 이동 생성자
	Vector4D(const Vector3D& other, const float _w = 0) noexcept;	// 복사 생성자
	Vector4D(const Vector3D&& other, const float _w = 0) noexcept;	// 이동 생성자
	~Vector4D() noexcept;

public:
	float x, y, z, w;

public:
	Vector4D& operator = (const Vector4D&) noexcept;				// 복사 대입 연산자
	Vector4D& operator = (const Vector4D&&) noexcept;			// 이동 대입 연산자

	Vector4D& operator = (const Vector3D&) noexcept;				// 복사 대입 연산자
	Vector4D& operator = (const Vector3D&&) noexcept;				// 복사 대입 연산자

public:
	float FastInvSqrt(float) const;
	Vector4D Normalize3();
	Vector4D Normalize();
	void DivideW();
	float Dot(const Vector4D&);
	Vector4D Cross(const Vector4D&);

public:
	/// 연산자 오버로딩
	Vector4D operator + (const Vector4D&) const;		// 벡터 덧셈
	Vector4D& operator += (const Vector4D&);
	Vector4D operator - () const;						// 벡터 마이너스 붙여주기
	Vector4D operator - (const Vector4D&) const;		// 벡터 뺄셈
	Vector4D& operator -= (const Vector4D&);
	bool operator == (const Vector4D&) const;			// 같은 벡터인가?
	bool operator < (const Vector4D&) const;			// 
	bool operator > (const Vector4D&) const;			// 

	/// 순서를 신경쓰지 않게 하기 위해 프렌드로 오버로딩
	friend Vector4D operator * (float, const Vector4D&);	// 벡터 스칼라 곱
	friend Vector4D operator * (const Vector4D&, float);	// 벡터 스칼라 곱
	Vector4D& operator *= (float);

	friend Vector4D operator/ (float, const Vector4D&);	// 벡터 스칼라 곱
	friend Vector4D operator/ (const Vector4D&, float);	// 벡터 스칼라 곱
	Vector4D& operator/= (float);

	Vector4D operator*(const Matrix4x4& other) const;	// 벡터 * 행렬 연산자
};

