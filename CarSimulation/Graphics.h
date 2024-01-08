#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include <d2d1.h>
#include <dwrite.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Init(HWND windowHandle);

	void ClearScreen(float r, float g, float b);
	void DrawList(int vertexCount);

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetDeviceContext();

public:
	void Init2D(HWND windowHandle);

	void D2DClearScreen(float r, float g, float b);
	void D2DDrawText(int x, int y, std::string text);
	void Begin();
	void End();

public:
	void InitSwapChain(HWND windowHandle);
	void PresentSwapChain(bool vsync);

	void SetVertexBuffer(VertexBuffer *vertexBuffer);
	void SetIndexBuffer(IndexBuffer *indexBuffer);
	
	void SetViewportSize(int width, int height);

public:
	void InitVertexShader(void* shaderByteCode, int shaderByteSize);
	void CompileVertexShader(const wchar_t* filename, void** shaderByteCOde, int* shaderByteSize);
	void SetVertexShader();

	void InitPixelShader(void* shaderByteCode, int shaderByteSize);
	void CompilePixelShader(const wchar_t* filename, void** shaderByteCOde, int* shaderByteSize);
	void SetPixelShader();

public:
	void InitConstantBuffer(void* constantBuffer, int size);
	void SetConstantBuffer();
	void UpdateConstantBuffer(void* newBuffer);

private:
	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;
	D3D_FEATURE_LEVEL m_featureLevel;

	IDXGIDevice *m_dxgiDevice;
	IDXGIAdapter *m_dxgiAdapter;
	IDXGIFactory *m_dxgiFactory;

	IDXGISwapChain *m_swapChain;

	ID3D11RenderTargetView *m_renderTargetView;

	ID3D11VertexShader *m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3DBlob *m_shaderBlob;

	ID3D11Buffer *m_constantBuffer;

private:
	ID2D1Factory *d2dFactory;
	ID2D1HwndRenderTarget *d2dRenderTarget;
	ID2D1SolidColorBrush *brush;

	IDWriteFactory *writeFactory;
	IDWriteTextFormat *textFormat;
};