#pragma once

#include <d3d11.h>

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Init(ID3D11Device *device, void* verticesList, int vertexSize, int listSize, void* shaderByteCode, int shaderByteSize);

	void Update(ID3D11DeviceContext *deviceContext, void* verticesList, int listSize);
	
	ID3D11Buffer* GetBuffer();
	ID3D11InputLayout* GetInputLayout();

	int GetVertexSize();
	int GetListSize();

private:
	ID3D11Buffer *m_buffer;
	ID3D11InputLayout *m_inputLayout;

	int m_vertexSize;
	int m_listSize;
};