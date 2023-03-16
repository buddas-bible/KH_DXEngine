#pragma once

class Matrix2x2;
class Matrix3x3;
class Matrix4x4;
class Vector3D;
class Vector4D;

class Vector2D
{
public:
	Vector2D();
	Vector2D(float, float);
	Vector2D(const Vector2D& other);			// ���� ������
	Vector2D(const Vector2D&& other) noexcept;	// �̵� ������
	Vector2D(const Vector3D& other);			// ���� ������
	Vector2D(const Vector3D&& other) noexcept;	// �̵� ������
	~Vector2D();

public:
	float x, y;

public:
	/// ���� ���� �Լ�
	float Size() const;									// ������ ũ�⸦ ��ȯ
	float SizeSquare() const;							// ������ ũ�� ������ ��ȯ
	float Cross(const Vector2D&) const;					// ���� ���� ��ȯ
	float Dot(const Vector2D&) const;					// ���� ���� ��ȯ

	float FastInvSqrt(float number) const;				// ��� �� ������

	float Projection(const Vector2D&);					// ���� ���� (��Į�� �� ��ȯ)
	float FindTheta(const Vector2D&) const;				// other ���Ϳ��� radian ��ȯ
	Vector2D Normalize() const;							// ����ȭ�� ���͸� ��ȯ

public:
	Vector2D& operator = (const Vector2D&);				// ���� ���� ������
	Vector2D& operator = (const Vector2D&&) noexcept;			// �̵� ���� ������
	
	Vector2D& operator = (const Vector3D&);				// ���� ���� ������
	Vector2D& operator = (const Vector3D&&) noexcept;	// �̵� ���� ������

public:
	/// ������ �����ε�
	Vector2D operator + (const Vector2D&) const;		// ���� ����
	Vector2D& operator += (const Vector2D&);
	Vector2D operator - () const;								// ���� ���̳ʽ� �ٿ��ֱ�
	Vector2D operator - (const Vector2D&) const;		// ���� ����
	Vector2D& operator -= (const Vector2D&);
	bool operator == (const Vector2D&) const;			// ���� �����ΰ�?
	bool operator < (const Vector2D&) const;			// 
	bool operator > (const Vector2D&) const;			// 

	/// ������ �Ű澲�� �ʰ� �ϱ� ���� ������� �����ε�
	friend Vector2D operator * (float, const Vector2D&);	// ���� ��Į�� ��
	friend Vector2D operator * (const Vector2D&, float);	// ���� ��Į�� ��
	Vector2D& operator*= (float);						// ���� ��Į�� ��
	
	friend Vector2D operator / (float, const Vector2D&);
	friend Vector2D operator / (const Vector2D&, float);
	Vector2D& operator /= (float);						// ���� ��Į�� ��

	Vector2D operator * (const Matrix2x2& other) const;	// ���� * ��� ������
	Vector2D operator * (const Matrix3x3& other) const;	// ���� * ��� ������
};

Vector2D Cross(const Vector2D& vec, float a);
Vector2D Cross(float a, const Vector2D& vec);