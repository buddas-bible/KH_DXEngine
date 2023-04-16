#pragma once

#include <sal.h>

class Matrix3x3;
class Matrix4x4;
class Vector2D;
class Vector4D;

class Vector3D
{
public:
	Vector3D() noexcept;
	constexpr Vector3D(float, float, float) noexcept;
	Vector3D(_In_reads_(3) const float* pArr) noexcept;
	Vector3D(const Vector3D& other) noexcept;			// 복사 생성자
	Vector3D(const Vector3D&& other) noexcept;		// 이동 생성자
	Vector3D(const Vector2D& other, const float _z = 0) noexcept;					// 복사 생성자
	Vector3D(const Vector2D&& other, const float _z = 0) noexcept;		// 이동 생성자
	Vector3D(const Vector4D& other) noexcept;			// 복사 생성자
	Vector3D(const Vector4D&& other) noexcept;		// 이동 생성자
	~Vector3D() noexcept;

public:
	float x, y, z;

public:
	/// 벡터 관련 함수
	float Size() const;									// 벡터의 크기를 반환
	float SizeSquare() const;							// 벡터의 크기 제곱을 반환
	Vector3D Cross(const Vector3D&) const;				// 외적 값을 반환
	float Dot(const Vector3D&) const;					// 내적 값을 반환

	float FastInvSqrt(float number) const;				// 고속 역 제곱근

	float Projection(const Vector3D&);					// 벡터 투영 (스칼라 값 반환)
	float FindTheta(const Vector3D&) const;				// other 벡터와의 radian 반환
	Vector3D Normalize() const &;							// 정규화된 벡터를 반환
	Vector3D Normalize() &&;							// 정규화된 벡터를 반환

public:
	Vector3D& operator = (const Vector3D&) noexcept;				// 복사 대입 연산자
	Vector3D& operator = (const Vector3D&&) noexcept;			// 이동 대입 연산자
	
	Vector3D& operator = (const Vector2D&) noexcept;				// 복사 대입 연산자
	Vector3D& operator = (const Vector2D&&) noexcept;				// 복사 대입 연산자

	Vector3D& operator = (const Vector4D&) noexcept;				// 복사 대입 연산자
	Vector3D& operator = (const Vector4D&&) noexcept;			// 이동 대입 연산자

public:
	/// 연산자 오버로딩
	Vector3D operator + (const Vector3D&) const;		// 벡터 덧셈
	Vector3D& operator += (const Vector3D&);
	Vector3D operator - () const;						// 벡터 마이너스 붙여주기
	Vector3D operator - (const Vector3D&) const;		// 벡터 뺄셈
	Vector3D& operator -= (const Vector3D&);
	bool operator == (const Vector3D&) const;			// 같은 벡터인가?
	bool operator < (const Vector3D&) const;			// 
	bool operator > (const Vector3D&) const;			// 

	/// 순서를 신경쓰지 않게 하기 위해 프렌드로 오버로딩
	friend Vector3D operator * (float, const Vector3D&);	// 벡터 스칼라 곱
	friend Vector3D operator * (const Vector3D&, float);	// 벡터 스칼라 곱
	Vector3D& operator *= (float);

	friend Vector3D operator / (float, const Vector3D&);	// 벡터 스칼라 곱
	friend Vector3D operator / (const Vector3D&, float);	// 벡터 스칼라 곱
	Vector3D& operator /= (float);

	Vector3D operator * (const Matrix3x3& other) const;	// 벡터 * 행렬 연산자
	// Vector3D operator * (const Matrix4x4& other) const;	// 벡터 * 행렬 연산자
};

Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c);