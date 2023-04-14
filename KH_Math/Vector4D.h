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
	Vector4D(const Vector4D& other) noexcept;						// ���� ������
	Vector4D(const Vector4D&& other) noexcept;						// �̵� ������
	Vector4D(const Vector3D& other, const float _w = 0) noexcept;	// ���� ������
	Vector4D(const Vector3D&& other, const float _w = 0) noexcept;	// �̵� ������
	~Vector4D() noexcept;

public:
	float x, y, z, w;

public:
	Vector4D& operator = (const Vector4D&) noexcept;				// ���� ���� ������
	Vector4D& operator = (const Vector4D&&) noexcept;			// �̵� ���� ������

	Vector4D& operator = (const Vector3D&) noexcept;				// ���� ���� ������
	Vector4D& operator = (const Vector3D&&) noexcept;				// ���� ���� ������

public:
	float FastInvSqrt(float) const;
	Vector4D Normalize3();
	Vector4D Normalize();
	void DivideW();
	float Dot(const Vector4D&);
	Vector4D Cross(const Vector4D&);

public:
	/// ������ �����ε�
	Vector4D operator + (const Vector4D&) const;		// ���� ����
	Vector4D& operator += (const Vector4D&);
	Vector4D operator - () const;						// ���� ���̳ʽ� �ٿ��ֱ�
	Vector4D operator - (const Vector4D&) const;		// ���� ����
	Vector4D& operator -= (const Vector4D&);
	bool operator == (const Vector4D&) const;			// ���� �����ΰ�?
	bool operator < (const Vector4D&) const;			// 
	bool operator > (const Vector4D&) const;			// 

	/// ������ �Ű澲�� �ʰ� �ϱ� ���� ������� �����ε�
	friend Vector4D operator * (float, const Vector4D&);	// ���� ��Į�� ��
	friend Vector4D operator * (const Vector4D&, float);	// ���� ��Į�� ��
	Vector4D& operator *= (float);

	friend Vector4D operator/ (float, const Vector4D&);	// ���� ��Į�� ��
	friend Vector4D operator/ (const Vector4D&, float);	// ���� ��Į�� ��
	Vector4D& operator/= (float);

	Vector4D operator*(const Matrix4x4& other) const;	// ���� * ��� ������
};

