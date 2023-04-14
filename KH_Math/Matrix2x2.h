#pragma once

class Matrix2x2
{
public:
	Matrix2x2() noexcept;
	Matrix2x2(
		float e00, float e01,
		float e10, float e11) noexcept;
	Matrix2x2(const Matrix2x2& mat) noexcept;
	Matrix2x2(const Matrix2x2&& mat) noexcept;
	~Matrix2x2() noexcept;

	float e[2][2];

	static Matrix2x2 IdentityMatrix();
	static Matrix2x2 RotateMatrix(float theta);						// 회전 변환 행렬
	static Matrix2x2 ScaleMatrix(float scaleX, float scaleY);		// 축척 변환 행렬
	static Matrix2x2 ScaleRotate(float scaleX, float scaleY, float theta);

	Matrix2x2 operator * (const Matrix2x2& other) const;
	Matrix2x2 operator + (const Matrix2x2& other) const;
	Matrix2x2 operator - (const Matrix2x2& other) const;
	Matrix2x2& operator = (const Matrix2x2& other) noexcept;
	Matrix2x2& operator = (const Matrix2x2&& other) noexcept;
};

