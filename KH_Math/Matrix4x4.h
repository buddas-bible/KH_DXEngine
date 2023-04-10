#pragma once

class Quaternion;

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(
		float e00, float e01, float e02, float e03,
		float e10, float e11, float e12, float e13,
		float e20, float e21, float e22, float e23,
		float e30, float e31, float e32, float e33
	);
	Matrix4x4(const Matrix4x4& mat);
	Matrix4x4(const Matrix4x4&& mat) noexcept;
	~Matrix4x4();

	float e[4][4];

	static Matrix4x4 IdentityMatrix();

	static Matrix4x4 TransformMatrix(											// 변환행렬
		float moveX, float moveY, float moveZ,
		float thetaX, float thetaY, float thetaZ,
		float scaleX, float scaleY, float scaleZ);
	static Matrix4x4 TranslateMatrix(float moveX, float moveY, float moveZ);	// 이동 변환 행렬
	static Matrix4x4 RotateMatrix(float thetaX, float thetaY, float thetaZ);	// 회전 변환 행렬
	static Matrix4x4 RotateMatrix(const Quaternion& q);	// 회전 변환 행렬
	static Matrix4x4 ScaleMatrix(float scaleX, float scaleY, float scaleZ);		// 축척 변환 행렬
	static Matrix4x4 ScaleRotate(
		float scaleX, float scaleY, float scaleZ,
		float thetaX, float thetaY, float thetaZ);

	Matrix4x4 operator * (const Matrix4x4& other) const;
	Matrix4x4 operator + (const Matrix4x4& other) const;
	Matrix4x4 operator - (const Matrix4x4& other) const;
	Matrix4x4 operator = (const Matrix4x4& other);
	Matrix4x4 operator = (const Matrix4x4&& other) noexcept;
};