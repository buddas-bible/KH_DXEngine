#pragma once

class Matrix2x2
{
public:
	Matrix2x2();
	Matrix2x2(
		float e00, float e01,
		float e10, float e11);
	Matrix2x2(const Matrix2x2& mat);
	Matrix2x2(const Matrix2x2&& mat) noexcept;
	~Matrix2x2();

	float e[2][2];

	static Matrix2x2 IdentityMatrix();
	static Matrix2x2 RotateMatrix(float theta);						// ȸ�� ��ȯ ���
	static Matrix2x2 ScaleMatrix(float scaleX, float scaleY);		// ��ô ��ȯ ���
	static Matrix2x2 ScaleRotate(float scaleX, float scaleY, float theta);

	Matrix2x2 operator * (const Matrix2x2& other) const;
	Matrix2x2 operator + (const Matrix2x2& other) const;
	Matrix2x2 operator - (const Matrix2x2& other) const;
	Matrix2x2& operator = (const Matrix2x2& other);
	Matrix2x2& operator = (const Matrix2x2&& other) noexcept;
};

