#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
	m_buffer = NULL;
}

IndexBuffer::~IndexBuffer()
{
	if (m_buffer) m_buffer->Release();
}

void IndexBuffer::Init(ID3D11Device *device, void* list, int listSize)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = 4 * listSize;
	desc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA init = { 0 };
	init.pSysMem = list;

	device->CreateBuffer(&desc, &init, &m_buffer);
}

ID3D11Buffer* IndexBuffer::GetBuffer()
{
	return m_buffer;
}