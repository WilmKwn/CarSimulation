#include "TrackCourse.h"

TrackCourse::TrackCourse(Graphics *graphics, void* shaderByteCode, int shaderByteSize)
{
	this->graphics = graphics;
	rotMatrix = new Math::Matrix();

	MakeTrack();

	boundaries = vertices;

	vertices.insert(vertices.end(), finishLine.begin(), finishLine.end());

	int numParts = vertices.size() / 4;
	int numIndices = indices.size();
	for (int i = 1; i <= numParts - 1; i++) {
		for (int j = 0; j < numIndices; j++) {
			indices.push_back(indices.at(j) + 4 * i);
		}
	}

	vBuffer = new VertexBuffer();
	vBuffer->Init(graphics->GetDevice(), static_cast<void*>(vertices.data()), sizeof(Math::Vertex), vertices.size(), shaderByteCode, shaderByteSize);

	iBuffer = new IndexBuffer();
	iBuffer->Init(graphics->GetDevice(), static_cast<void*>(indices.data()), indices.size());
}

TrackCourse::~TrackCourse()
{

}

void TrackCourse::Run()
{
	graphics->SetVertexBuffer(vBuffer);
	vBuffer->Update(graphics->GetDeviceContext(), static_cast<void*>(vertices.data()), vertices.size() * sizeof(Math::Vertex));
	
	graphics->SetIndexBuffer(iBuffer);
	graphics->DrawList(indices.size());
}

void TrackCourse::MoveZ(float speed)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i).z += speed;
	}
	for (int i = 0; i < finishLine.size(); i++) {
		finishLine.at(i).z += speed;
	}
}

void TrackCourse::RotateY(float angle)
{
	for (int i = 0; i < vertices.size(); i++) {
		vertices.at(i) = rotMatrix->SetRotateY(vertices.at(i), angle);
	}
	for (int i = 0; i < finishLine.size(); i++) {
		finishLine.at(i) = rotMatrix->SetRotateY(finishLine.at(i), angle);
	}
}

std::vector<Math::Vertex> TrackCourse::GetVertices()
{
	return boundaries;
}
std::vector<Math::Vertex> TrackCourse::GetFinishLine()
{
	return finishLine;
}

void TrackCourse::AddTile(bool side, int h, int v)
{
	if (side) {
		for (int i = 0; i < leftSquare.size(); i++) {
			leftSquare.at(i).x += h * SIZE;
			leftSquare.at(i).z += v * SIZE;
		}
	}
	else {
		for (int i = 0; i < rightSquare.size(); i++) {
			rightSquare.at(i).x += h * SIZE;
			rightSquare.at(i).z += v * SIZE;
		}
	}
	std::vector<Math::Vertex> square = (side) ? leftSquare : rightSquare;
	vertices.insert(vertices.end(), square.begin(), square.end());
}

void TrackCourse::MakeTrack()
{
	vertices.insert(vertices.end(), leftSquare.begin(), leftSquare.end());
	vertices.insert(vertices.end(), rightSquare.begin(), rightSquare.end());

	for (int i = 0; i < 3; i++) AddTile(true, 0, 1);
	for (int i = 0; i < 3; i++) AddTile(false, 0, 1);
	
	for (int i = 0; i < 2; i++) AddTile(true, 0, 1);
	for (int i = 0; i < 3; i++) AddTile(true, 1, 0);

	for (int i = 0; i < 2; i++) AddTile(false, 1, 0);
	for (int i = 0; i < 4; i++) AddTile(false, 0, 1);
	for (int i = 0; i < 2; i++) AddTile(true, 0, 1);

	for (int i = 0; i < 4; i++) AddTile(true, -1, 0);
	for (int i = 0; i < 2; i++) AddTile(false, 0, 1);

	for (int i = 0; i < 6; i++) AddTile(false, -1, 0);

	for (int i = 0; i < 2; i++) AddTile(true, -1, 0);
	for (int i = 0; i < 5; i++) AddTile(true, 0, 1);
	for (int i = 0; i < 3; i++) AddTile(false, 0, 1);

	for (int i = 0; i < 3; i++) AddTile(false, 0, 1);
	for (int i = 0; i < 8; i++) AddTile(false, -1, 0);

	for (int i = 0; i < 4; i++) AddTile(true, -1, 0);

	for (int i = 0; i < 5; i++) AddTile(false, 0, -1);
	for (int i = 0; i < 4; i++) AddTile(false, 1, 0);

	for (int i = 0; i < 12; i++) AddTile(false, 0, -1);
	for (int i = 0; i < 8; i++) AddTile(false, 1, 0);

	for (int i = 0; i < 1; i++) AddTile(false, 0, 1);
}