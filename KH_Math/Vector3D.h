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
	Vector3D(const Vector3D& other) noexcept;			// ���� ������
	Vector3D(const Vector3D&& other) noexcept;		// �̵� ������
	Vector3D(const Vector2D& other, const float _z = 0) noexcept;					// ���� ������
	Vector3D(const Vector2D&& other, const float _z = 0) noexcept;		// �̵� ������
	Vector3D(const Vector4D& other) noexcept;			// ���� ������
	Vector3D(const Vector4D&& other) noexcept;		// �̵� ������
	~Vector3D() noexcept;

public:
	float x, y, z;

public:
	/// ���� ���� �Լ�
	float Size() const;									// ������ ũ�⸦ ��ȯ
	float SizeSquare() const;							// ������ ũ�� ������ ��ȯ
	Vector3D Cross(const Vector3D&) const;				// ���� ���� ��ȯ
	float Dot(const Vector3D&) const;					// ���� ���� ��ȯ

	float FastInvSqrt(float number) const;				// ��� �� ������

	float Projection(const Vector3D&);					// ���� ���� (��Į�� �� ��ȯ)
	float FindTheta(const Vector3D&) const;				// other ���Ϳ��� radian ��ȯ
	Vector3D Normalize() const &;							// ����ȭ�� ���͸� ��ȯ
	Vector3D Normalize() &&;							// ����ȭ�� ���͸� ��ȯ

public:
	Vector3D& operator = (const Vector3D&) noexcept;				// ���� ���� ������
	Vector3D& operator = (const Vector3D&&) noexcept;			// �̵� ���� ������
	
	Vector3D& operator = (const Vector2D&) noexcept;				// ���� ���� ������
	Vector3D& operator = (const Vector2D&&) noexcept;				// ���� ���� ������

	Vector3D& operator = (const Vector4D&) noexcept;				// ���� ���� ������
	Vector3D& operator = (const Vector4D&&) noexcept;			// �̵� ���� ������

public:
	/// ������ �����ε�
	Vector3D operator + (const Vector3D&) const;		// ���� ����
	Vector3D& operator += (const Vector3D&);
	Vector3D operator - () const;						// ���� ���̳ʽ� �ٿ��ֱ�
	Vector3D operator - (const Vector3D&) const;		// ���� ����
	Vector3D& operator -= (const Vector3D&);
	bool operator == (const Vector3D&) const;			// ���� �����ΰ�?
	bool operator < (const Vector3D&) const;			// 
	bool operator > (const Vector3D&) const;			// 

	/// ������ �Ű澲�� �ʰ� �ϱ� ���� ������� �����ε�
	friend Vector3D operator * (float, const Vector3D&);	// ���� ��Į�� ��
	friend Vector3D operator * (const Vector3D&, float);	// ���� ��Į�� ��
	Vector3D& operator *= (float);

	friend Vector3D operator / (float, const Vector3D&);	// ���� ��Į�� ��
	friend Vector3D operator / (const Vector3D&, float);	// ���� ��Į�� ��
	Vector3D& operator /= (float);

	Vector3D operator * (const Matrix3x3& other) const;	// ���� * ��� ������
	// Vector3D operator * (const Matrix4x4& other) const;	// ���� * ��� ������
};

Vector3D TripleProduct(const Vector3D& a, const Vector3D& b, const Vector3D& c);