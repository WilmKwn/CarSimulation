#include "Math.h"

Math::Math() {}
Math::~Math() {}

// 1 2
// 0 3
bool Math::CollidedRect(std::vector<Vertex> rect, Vertex p)
{
	return (p.x <= rect.at(2).x && p.x >= rect.at(1).x && p.z >= rect.at(0).z && p.z <= rect.at(2).z);
}

Math::Matrix::Matrix()
{
	memset(matrix, 0, sizeof(float)*16);
}

Math::Matrix::~Matrix() {}

void Math::Matrix::SetIdentity()
{
	memset(matrix, 0, sizeof(float) * 16);
	matrix[0][0] = 1.0f;
	matrix[1][1] = 1.0f;
	matrix[2][2] = 1.0f;
	matrix[3][3] = 1.0f;
}

void Math::Matrix::SetProjection(float fov, float aspectRatio, float zNear, float zFar)
{
	SetIdentity();
	float yScale = 1.0f / tanf(fov / 2.0f);
	matrix[0][0] = yScale;
	matrix[1][1] = yScale / aspectRatio;
	matrix[2][2] = zFar / (zFar - zNear);
	matrix[2][3] = 1.0f;
	matrix[3][2] = (-zNear * zFar) / (zFar - zNear);
}

Math::Vertex Math::Matrix::SetRotateY(Math::Vertex v, float angle)
{
	SetIdentity();
	matrix[0][0] = cosf(angle);
	matrix[0][2] = sinf(angle);
	matrix[1][1] = 1.0f;
	matrix[2][0] = -sinf(angle);
	matrix[2][2] = cosf(angle);

	Vertex newV;
	newV.x = v.x * matrix[0][0] + v.y * matrix[0][1] + v.z * matrix[0][2];
	newV.y = v.x * matrix[1][0] + v.y * matrix[1][1] + v.z * matrix[1][2];
	newV.z = v.x * matrix[2][0] + v.y * matrix[2][1] + v.z * matrix[2][2];
	return newV;
}