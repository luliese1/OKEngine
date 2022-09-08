#include "pch.h"
#include "VertexBuffer.h"



void VertexBuffer::Initialize(void* vertices, int vertexStructSize, int vertexCount, std::shared_ptr<Device>& device)
{
	m_VertexStructSize = vertexStructSize;
	m_VertexCount = vertexCount;

	D3D11_BUFFER_DESC VertexBufferDesc;

	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = vertexStructSize * vertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA VertexInitData;

	VertexInitData.pSysMem = vertices;

	HR(device->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexInitData, m_VertexBuffer.GetAddressOf()));
#if defined(DEBUG) || defined(_DEBUG)
	m_VertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("VertexBuffer") - 1, "VertexBuffer");
#endif
}

void VertexBuffer::Release()
{

}
