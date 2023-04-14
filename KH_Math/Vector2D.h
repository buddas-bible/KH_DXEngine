#pragma once

#include <sal.h>

class Matrix2x2;
class Matrix3x3;
class Matrix4x4;
class Vector3D;
class Vector4D;

class Vector2D
{
public:
	Vector2D() noexcept;
	constexpr Vector2D(float, float) noexcept;
	Vector2D(_In_reads_(2) const float* pArr) noexcept;
	Vector2D(const Vector2D& other) noexcept;	// 복사 생성자
	Vector2D(const Vector2D&& other) noexcept;	// 이동 생성자
	Vector2D(const Vector3D& other) noexcept;	// 복사 생성자
	Vector2D(const Vector3D&& other) noexcept;	// 이동 생성자
	~Vector2D() noexcept;

public:
	float x, y;

public:
	/// 벡터 관련 함수
	float Size() const;									// 벡터의 크기를 반환
	float SizeSquare() const;							// 벡터의 크기 제곱을 반환
	float Cross(const Vector2D&) const;					// 외적 값을 반환
	float Dot(const Vector2D&) const;					// 내적 값을 반환

	float FastInvSqrt(float number) const;				// 고속 역 제곱근

	float Projection(const Vector2D&);					// 벡터 투영 (스칼라 값 반환)
	float FindTheta(const Vector2D&) const;				// other 벡터와의 radian 반환
	Vector2D Normalize() const;							// 정규화된 벡터를 반환

public:
	Vector2D& operator = (const Vector2D&) noexcept;				// 복사 대입 연산자
	Vector2D& operator = (const Vector2D&&) noexcept;			// 이동 대입 연산자
	
	Vector2D& operator = (const Vector3D&) noexcept;				// 복사 대입 연산자
	Vector2D& operator = (const Vector3D&&) noexcept;	// 이동 대입 연산자

public:
	/// 연산자 오버로딩
	Vector2D operator + (const Vector2D&) const;		// 벡터 덧셈
	Vector2D& operator += (const Vector2D&);
	Vector2D operator - () const;								// 벡터 마이너스 붙여주기
	Vector2D operator - (const Vector2D&) const;		// 벡터 뺄셈
	Vector2D& operator -= (const Vector2D&);
	bool operator == (const Vector2D&) const;			// 같은 벡터인가?
	bool operator < (const Vector2D&) const;			// 
	bool operator > (const Vector2D&) const;			// 

	/// 순서를 신경쓰지 않게 하기 위해 프렌드로 오버로딩
	friend Vector2D operator * (float, const Vector2D&);	// 벡터 스칼라 곱
	friend Vector2D operator * (const Vector2D&, float);	// 벡터 스칼라 곱
	Vector2D& operator*= (float);						// 벡터 스칼라 곱
	
	friend Vector2D operator / (float, const Vector2D&);
	friend Vector2D operator / (const Vector2D&, float);
	Vector2D& operator /= (float);						// 벡터 스칼라 곱

	Vector2D operator * (const Matrix2x2& other) const;	// 벡터 * 행렬 연산자
	Vector2D operator * (const Matrix3x3& other) const;	// 벡터 * 행렬 연산자
};

Vector2D Cross(const Vector2D& vec, float a);
Vector2D Cross(float a, const Vector2D& vec);