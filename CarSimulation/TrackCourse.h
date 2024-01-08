#pragma once

#include "Graphics.h"
#include "Math.h"

#define SIZE 3.0f

class TrackCourse {
public:
	TrackCourse(Graphics *graphics, void* shaderByteCode, int shaderByteSize);
	~TrackCourse();

	void Run();

	void MoveZ(float speed);

	void RotateY(float angle);

	void MakeTrack();
	void AddTile(bool side, int h, int v);

	std::vector<Math::Vertex> GetVertices();
	std::vector<Math::Vertex> GetFinishLine();

private:
	Graphics *graphics;

	VertexBuffer *vBuffer;
	IndexBuffer *iBuffer;

	Math::Matrix *rotMatrix;

	std::vector<Math::Vertex> finishLine = {
		{-3.0f, 0.0f, 0.0f},
		{-3.0f, 0.0f, 0.1f},
		{3.0f, 0.0f, 0.1f},
		{3.0f, 0.0f, 0.0f},
	};
	std::vector<Math::Vertex> leftSquare = {
		{-6.0f, 0.0f, 0.0f},
		{-6.0f, 0.0f, 3.0f},
		{-3.0f, 0.0f, 3.0f},
		{-3.0f, 0.0f, 0.0f},
	};
	std::vector<Math::Vertex> rightSquare = {
		{3.0f, 0.0f, 0.0f},
		{3.0f, 0.0f, 3.0f},
		{6.0f, 0.0f, 3.0f},
		{6.0f, 0.0f, 0.0f},
	};

	std::vector<Math::Vertex> vertices = {};

	std::vector<Math::Vertex> boundaries = {};

	std::vector<int> indices = {
		0,1,2,
		2,3,0,
	};
};