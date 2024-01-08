#pragma once

#include <iostream>
#include <vector>
#include <math.h>

class Math
{
public:
	Math();
	~Math();

	struct Vertex {
		float x, y, z;
	};

	class Matrix {
	public:
		Matrix();
		~Matrix();

		void SetIdentity();
		void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
		Vertex SetRotateY(Vertex v, float angle);
	
	private:
		float matrix[4][4];
	};

	static bool CollidedRect(std::vector<Vertex> rect, Vertex p);
};