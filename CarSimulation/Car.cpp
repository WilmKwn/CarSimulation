#include "Car.h"

Car::Car(Graphics *graphics, void* shaderByteCode, int shaderByteSize)
{
	this->graphics = graphics;
	rotMatrix = new Math::Matrix();

	angle = 0.0f;

	int numParts = vertices.size() / 8;
	int numIndices = indices.size();
	for (int i = 1; i <= numParts-1; i++) {
		for (int j = 0; j < numIndices; j++) {
			indices.push_back(indices.at(j) + 8*i);
		}
	}

	vBuffer = new VertexBuffer();
	vBuffer->Init(graphics->GetDevice(), static_cast<void*>(vertices.data()), sizeof(Math::Vertex), vertices.size(), shaderByteCode, shaderByteSize);

	iBuffer = new IndexBuffer();
	iBuffer->Init(graphics->GetDevice(), static_cast<void*>(indices.data()), indices.size());
}

Car::~Car()
{
}

void Car::Run()
{
	graphics->SetVertexBuffer(vBuffer);
	vBuffer->Update(graphics->GetDeviceContext(), static_cast<void*>(vertices.data()), vertices.size() * sizeof(Math::Vertex));
	
	graphics->SetIndexBuffer(iBuffer);
	graphics->DrawList(indices.size());
}

void Car::MoveZ(float speed)
{
	for (int i = 0; i < carBoundary.size(); i++) {
		carBoundary.at(i).x += speed*sinf(angle);
		carBoundary.at(i).z += speed*cosf(angle);
	}
	center.x += speed * sinf(angle);
	center.z += speed * cosf(angle);
}

void Car::RotateY(float angle)
{
	this->angle += angle;

	float xDis = center.x;
	float zDis = center.z;

	for (int i = 0; i < carBoundary.size(); i++) {
		carBoundary.at(i).x -= xDis;
		carBoundary.at(i).z -= zDis;
	}
	for (int i = 0; i < carBoundary.size(); i++) {
		carBoundary.at(i) = rotMatrix->SetRotateY(carBoundary.at(i), angle);
	}
	for (int i = 0; i < carBoundary.size(); i++) {
		carBoundary.at(i).x += xDis;
		carBoundary.at(i).z += zDis;
	}
}

std::vector<Math::Vertex> Car::GetBoundary()
{
	return carBoundary;
}