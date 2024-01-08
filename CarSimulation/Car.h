#pragma once

#include "Graphics.h"
#include "Math.h"

class Car
{
public:
	Car(Graphics *graphics, void* shaderByteCode, int shaderByteSize);
	~Car();

	void Run();

	void MoveZ(float speed);

	void RotateY(float angle);

	std::vector<Math::Vertex> GetBoundary();

private:
	Graphics *graphics;

	VertexBuffer *vBuffer;
	IndexBuffer *iBuffer;

	Math::Matrix *rotMatrix;

	float angle;

	Math::Vertex center = { 0.0f, 0.0f, 0.0f };
	std::vector<Math::Vertex> carBoundary = {
		{-0.5f, 0.0f, 0.0f},
		{-0.5f, 0.0f, 1.0f},
		{0.5f, 0.0f, 1.0f},
		{0.5f, 0.0f, 0.0f},
	};

	std::vector<Math::Vertex> vertices = {
		// Front left wheel
		{-0.4f, 0.0f, 1.0f},
		{-0.4f, 0.3f, 1.0f},
		{-0.5f, 0.3f, 1.0f},
		{-0.5f, 0.0f, 1.0f},

		{-0.4f, 0.0f, 0.8f},
		{-0.4f, 0.3f, 0.8f},
		{-0.5f, 0.3f, 0.8f},
		{-0.5f, 0.0f, 0.8f},

		// Front right wheel
		{0.5f, 0.0f, 1.0f},
		{0.5f, 0.3f, 1.0f},
		{0.4f, 0.3f, 1.0f},
		{0.4f, 0.0f, 1.0f},

		{0.5f, 0.0f, 0.9f},
		{0.5f, 0.3f, 0.9f},
		{0.4f, 0.3f, 0.9f},
		{0.4f, 0.0f, 0.9f},

		// Back left wheel
		{-0.3f, 0.0f, 0.0f},
		{-0.3f, 0.3f, 0.0f},
		{-0.5f, 0.3f, 0.0f},
		{-0.5f, 0.0f, 0.0f},

		{-0.3f, 0.0f, 0.1f},
		{-0.3f, 0.3f, 0.1f},
		{-0.5f, 0.3f, 0.1f},
		{-0.5f, 0.0f, 0.1f},

		// Back right wheel
		{0.5f, 0.0f, 0.0f},
		{0.5f, 0.3f, 0.0f},
		{0.3f, 0.3f, 0.0f},
		{0.3f, 0.0f, 0.0f},

		{0.5f, 0.0f, 0.1f},
		{0.5f, 0.3f, 0.1f},
		{0.3f, 0.3f, 0.1f},
		{0.3f, 0.0f, 0.1f},

		// Body of car
		{0.4f, 0.3f, 1.0f},
		{0.4f, 0.5f, 1.0f},
		{-0.4f, 0.5f, 1.0f},
		{-0.4f, 0.3f, 1.0f},

		{-0.4f, 0.3f, 0.0f},
		{-0.4f, 0.5f, 0.0f},
		{0.4f, 0.5f, 0.0f},
		{0.4f, 0.3f, 0.0f},

		// Top part of car
		{0.2f, 0.5f, 0.3f},
		{0.2f, 0.7f, 0.3f},
		{-0.2f, 0.7f, 0.3f},
		{-0.2f, 0.5f, 0.3f},

		{-0.2f, 0.5f, 0.7f},
		{-0.2f, 0.7f, 0.7f},
		{0.2f, 0.7f, 0.7f},
		{0.2f, 0.5f, 0.7f},
	};

	std::vector<int> indices = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,6,5,
		5,2,1,

		7,0,3,
		3,4,7,

		3,2,5,
		5,4,3,

		7,6,1,
		1,0,7,
	};
};