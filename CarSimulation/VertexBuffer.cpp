#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	m_buffer = NULL;
	m_inputLayout = NULL;
}

VertexBuffer::~VertexBuffer()
{
	if (m_buffer) m_buffer->Release();
	if (m_inputLayout) m_inputLayout->Release();
}

void VertexBuffer::Init(ID3D11Device *device, void* verticesList, int vertexSize, int listSize, void* shaderByteCode, int shaderByteSize)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = vertexSize * listSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = verticesList;

	m_vertexSize = vertexSize;
	m_listSize = listSize;

	device->CreateBuffer(&bufferDesc, &initData, &m_buffer);
	
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(layout, ARRAYSIZE(layout), shaderByteCode, shaderByteSize, &m_inputLayout);
}

void VertexBuffer::Update(ID3D11DeviceContext *deviceContext, void* verticesList, int listSize)
{
	D3D11_MAPPED_SUBRESOURCE resource = { 0 };
	deviceContext->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, verticesList, listSize);
	deviceContext->Unmap(m_buffer, 0);
}

ID3D11Buffer* VertexBuffer::GetBuffer()
{
	return m_buffer;
}

ID3D11InputLayout* VertexBuffer::GetInputLayout()
{
	return m_inputLayout;
}

int VertexBuffer::GetVertexSize()
{
	return m_vertexSize;
}

int VertexBuffer::GetListSize()
{
	return m_listSize;
}