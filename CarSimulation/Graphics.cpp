#include "Graphics.h"

Graphics::Graphics() 
{
	m_device = NULL;
	m_deviceContext = NULL;

	m_dxgiDevice = NULL;
	m_dxgiAdapter = NULL;
	m_dxgiFactory = NULL;

	m_swapChain = NULL;

	m_renderTargetView = NULL;

	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_shaderBlob = NULL;

	m_constantBuffer = NULL;

	d2dFactory = NULL;
	d2dRenderTarget = NULL;
	brush = NULL;
	writeFactory = NULL;
	textFormat = NULL;
}

Graphics::~Graphics()
{
	if (m_device) m_device->Release();
	if (m_deviceContext) m_deviceContext->Release();
	
	if (m_dxgiDevice) m_dxgiDevice->Release();
	if (m_dxgiAdapter) m_dxgiAdapter->Release();
	if (m_dxgiFactory) m_dxgiFactory->Release();

	if (m_swapChain) m_swapChain->Release();

	if (m_renderTargetView) m_renderTargetView->Release();

	if (m_vertexShader) m_vertexShader->Release();
	if (m_pixelShader) m_pixelShader->Release();
	if (m_shaderBlob) m_shaderBlob->Release();

	if (m_constantBuffer) m_constantBuffer->Release();

	if (d2dFactory) d2dFactory->Release();
	if (d2dRenderTarget) d2dRenderTarget->Release();
	if (brush) brush->Release();
	if (writeFactory) writeFactory->Release();
	if (textFormat) textFormat->Release();
}

void Graphics::Init2D(HWND windowHandle)
{
	RECT r;
	GetClientRect(windowHandle, &r);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
	d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(r.right, r.bottom)), &d2dRenderTarget);

	d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &brush);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&writeFactory);
}

void Graphics::D2DClearScreen(float r, float g, float b)
{
	d2dRenderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::D2DDrawText(int x, int y, std::string text)
{
	writeFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 30.0f, L"en-us", &textFormat);

	std::wstring t(text.begin(), text.end());
	d2dRenderTarget->DrawTextW(t.c_str(), text.size(), textFormat, D2D1::RectF(x, y, x + sizeof(text)*text.size(), y), brush);
}

void Graphics::Begin() 
{
	d2dRenderTarget->BeginDraw();
}
void Graphics::End()
{
	d2dRenderTarget->EndDraw();
}

void Graphics::Init(HWND windowHandle)
{
	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE };
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	for (int i = 0; i < ARRAYSIZE(driverTypes); i++) {
		if (D3D11CreateDevice(NULL, driverTypes[i], NULL, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext)==S_OK) {
			break;
		}
	}

	m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
	m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

	InitSwapChain(windowHandle);
}

void Graphics::InitSwapChain(HWND windowHandle)
{
	RECT r;
	GetClientRect(windowHandle, &r);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = r.right;
	swapChainDesc.BufferDesc.Height = r.bottom;
	swapChainDesc.OutputWindow = windowHandle;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	m_dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);

	ID3D11Texture2D *buffer = NULL;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	m_device->CreateRenderTargetView(buffer, NULL, &m_renderTargetView);
}

void Graphics::PresentSwapChain(bool vsync)
{
	m_swapChain->Present(vsync, NULL);
}

void Graphics::SetVertexBuffer(VertexBuffer *vertexBuffer)
{
	unsigned int stride = vertexBuffer->GetVertexSize();
	unsigned int offset = 0;

	auto temp = vertexBuffer->GetBuffer();
	m_deviceContext->IASetVertexBuffers(0, 1, &temp, &stride, &offset);
	m_deviceContext->IASetInputLayout(vertexBuffer->GetInputLayout());
}

void Graphics::SetIndexBuffer(IndexBuffer *indexBuffer)
{
	m_deviceContext->IASetIndexBuffer(indexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
}

void Graphics::SetViewportSize(int width, int height)
{
	D3D11_VIEWPORT vp = { 0 };
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &vp);
}

// VERTEX SHADER
void Graphics::InitVertexShader(void* shaderByteCode, int shaderByteSize)
{
	m_device->CreateVertexShader(shaderByteCode, shaderByteSize, NULL, &m_vertexShader);
}

void Graphics::CompileVertexShader(const wchar_t* filename, void** shaderByteCode, int* shaderByteSize)
{
	D3DCompileFromFile(filename, NULL, NULL, "vsmain", "vs_5_0", 0, 0, &m_shaderBlob, NULL);

	*shaderByteCode = m_shaderBlob->GetBufferPointer();
	*shaderByteSize = (int)m_shaderBlob->GetBufferSize();
}

void Graphics::SetVertexShader()
{
	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
}

// PIXEL SHADER
void Graphics::InitPixelShader(void* shaderByteCode, int shaderByteSize)
{
	m_device->CreatePixelShader(shaderByteCode, shaderByteSize, NULL, &m_pixelShader);
}

void Graphics::CompilePixelShader(const wchar_t* filename, void** shaderByteCode, int* shaderByteSize)
{
	D3DCompileFromFile(filename, NULL, NULL, "psmain", "ps_5_0", 0, 0, &m_shaderBlob, NULL);

	*shaderByteCode = m_shaderBlob->GetBufferPointer();
	*shaderByteSize = (int)m_shaderBlob->GetBufferSize();
}

void Graphics::SetPixelShader()
{
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
}

// CONSTANT BUFFER
void Graphics::InitConstantBuffer(void* constantBuffer, int size)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = size;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = constantBuffer;

	m_device->CreateBuffer(&desc, &initData, &m_constantBuffer);
}

void Graphics::SetConstantBuffer()
{
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void Graphics::UpdateConstantBuffer(void* newBuffer)
{
	m_deviceContext->UpdateSubresource(m_constantBuffer, NULL, NULL, newBuffer, NULL, NULL);
}

// OTHERS
void Graphics::ClearScreen(float r, float g, float b)
{
	float color[] = { r,g,b,1.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
}

void Graphics::DrawList(int vertexCount)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->DrawIndexed(vertexCount, 0, 0);
}

ID3D11Device* Graphics::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return m_deviceContext;
}