#pragma once

#include <d3d11.h>

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Init(ID3D11Device *device, void* list, int listSize);

	ID3D11Buffer* GetBuffer();

private:
	ID3D11Buffer *m_buffer;
};