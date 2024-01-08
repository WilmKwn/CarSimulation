#include "Window.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "Math.h"

#include "Car.h"
#include "TrackCourse.h"

#include <ctime>

#define ACCELERATION 0.0015f

struct constantBuffer {
	Math::Matrix projection;
};

bool Collided(std::vector<Math::Vertex> track, std::vector<Math::Vertex> car)
{
	for (int i = 0; i < track.size(); i += 4) {
		std::vector<Math::Vertex> square = std::vector<Math::Vertex>(track.begin()+i, track.begin()+i+4);
		for (int j = 0; j < car.size(); j++) {
			if (Math::CollidedRect(square, car.at(j))) {
				return true;
			}
		}
	}
	return false;
}

bool Finished(std::vector<Math::Vertex> finishLine, std::vector<Math::Vertex> car)
{
	for (int i = 0; i < car.size(); i++) {
		if (Math::CollidedRect(finishLine, car.at(i))) {
			return true;
		}
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
	Window *window = new Window(740, 480, hInstance);
	Graphics *graphics = new Graphics();
	graphics->Init(window->GetHandle());
	graphics->Init2D(window->GetHandle());

	void* shaderByteCode = NULL;
	int shaderByteSize = 0;

	char temp[100];
	GetModuleFileNameA(NULL, temp, sizeof(temp));
	std::string path(temp);
	path = path.substr(0, path.size() - 27);

	std::string vTemp = path + "CarSimulation\\VertexShader.hlsl";
	MessageBoxA(NULL, vTemp.c_str(), "", MB_OK);
	std::wstring vPath(vTemp.begin(), vTemp.end());
	graphics->CompileVertexShader(vPath.c_str(), &shaderByteCode, &shaderByteSize);
	graphics->InitVertexShader(shaderByteCode, shaderByteSize);

	TrackCourse *trackCourse = new TrackCourse(graphics, shaderByteCode, shaderByteSize);
	Car *car = new Car(graphics, shaderByteCode, shaderByteSize);

	std::string pTemp = path + "CarSimulation\\PixelShader.hlsl";
	std::wstring pPath(pTemp.begin(), pTemp.end());
	graphics->CompilePixelShader(pPath.c_str(), &shaderByteCode, &shaderByteSize);
	graphics->InitPixelShader(shaderByteCode, shaderByteSize);

	constantBuffer cBuffer;
	cBuffer.projection.SetProjection(1.57f, window->GetSize().first/window->GetSize().second, -1.0f, 1.0f);
	graphics->InitConstantBuffer(&cBuffer, sizeof(constantBuffer));

	float angle = 0.0f;
	float speed = 0.0f;
	int disTraveled = 0;

	bool gameStart = false;

	std::clock_t time, startTime;

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		if (!gameStart) {
			graphics->Begin();
			graphics->D2DClearScreen(0.5f, 0.0f, 0.5f);

			graphics->D2DDrawText(window->GetSize().first / 4, 50, "UP    ARROW - Accelerate");
			graphics->D2DDrawText(window->GetSize().first / 4, 100, "DOWN  ARROW - Decelerate");
			graphics->D2DDrawText(window->GetSize().first / 4, 150, "RIGHT ARROW - Curve right");
			graphics->D2DDrawText(window->GetSize().first / 4, 200, "LEFT  ARROW - Curve left");
			graphics->D2DDrawText(window->GetSize().first / 4, 400, "[Spacebar] 2 START");

			graphics->End();

			if (Input::GetKeyDown(VK_SPACE)) {
				gameStart = true;

				trackCourse = new TrackCourse(graphics, shaderByteCode, shaderByteSize);
				car = new Car(graphics, shaderByteCode, shaderByteSize);
				speed = angle = 0.0f;
				disTraveled = 0;

				startTime = clock();
			}
		}
		else {
			graphics->ClearScreen(0.0f, 0.35f, 0.35f);
			graphics->SetViewportSize(window->GetSize().first, window->GetSize().second);

			graphics->UpdateConstantBuffer(&cBuffer);
			graphics->SetConstantBuffer();

			graphics->SetVertexShader();
			graphics->SetPixelShader();

			// ------------------------------------
			trackCourse->Run();
			car->Run();

			trackCourse->RotateY(-angle);
			car->RotateY(angle);
			angle = 0.0f;

			if (!Collided(trackCourse->GetVertices(), car->GetBoundary())) {
				time = std::clock();

				if (Finished(trackCourse->GetFinishLine(), car->GetBoundary()) && disTraveled > window->GetSize().first) {
					std::stringstream ss; ss << (time - startTime) / CLOCKS_PER_SEC << " sec";
					MessageBoxA(NULL, ss.str().c_str(), "Finished", MB_OK);
					gameStart = false;
				}

				if (Input::GetKeyDown(VK_RIGHT)) {
					angle += -speed * 0.2f;
				}
				else if (Input::GetKeyDown(VK_LEFT)) {
					angle -= -speed * 0.2f;
				}

				if (Input::GetKeyDown(VK_UP)) {
					disTraveled++;

					speed += -ACCELERATION;
					speed = max(speed, -0.1f);
				}
				else if (Input::GetKeyDown(VK_DOWN)) {
					speed += ACCELERATION;
					speed = min(speed, 0.1f);
				}
				if (!Input::GetKeyDown(VK_UP) && !Input::GetKeyDown(VK_DOWN)) {
					speed *= 0.98f;
				}
				trackCourse->MoveZ(speed);
				car->MoveZ(-speed);

				// ------------------------------------
				graphics->PresentSwapChain(true);
			}
			else {
				MessageBox(NULL, L"", L"y u crashin", MB_OK);
				trackCourse = new TrackCourse(graphics, shaderByteCode, shaderByteSize);
				car = new Car(graphics, shaderByteCode, shaderByteSize);
				speed = angle = 0.0f;
				disTraveled = 0;

				startTime = clock();
			}
		}
			
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}
	}
	return (int)msg.wParam;
}