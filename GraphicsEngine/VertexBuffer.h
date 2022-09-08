#pragma once
#include "Device.h"

class VertexBuffer 
{
public:
	//일단 이녀석을 위주로 
	//버텍스의 포인터, 구조체 크기, 개수
	void Initialize(void* vertices, int vertexStructSize, int vertexCount, std::shared_ptr<Device>& device);
	//template<typename T>
	//void Initialize(std::vector<T>& vertices, int vertexSize, std::shared_ptr<Device>& device);

	void Release();

	UINT GetVertexStructSize() const { return m_VertexStructSize; }
	UINT GetVertexCount() const { return m_VertexCount; }

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() const { return m_VertexBuffer; }

private:
	ComPtr<ID3D11Buffer> m_VertexBuffer;

	UINT m_VertexStructSize;
	UINT m_VertexCount;

};
//
//template<typename T>
//void VertexBuffer::Initialize(std::vector<T>& vertices, int VertexCount, std::shared_ptr<Device>& device)
//{
//	m_VertexStructSize = sizeof(T);
//	m_VertexCount = VertexCount;
//
//	D3D11_BUFFER_DESC VertexBufferDesc;
//
//	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
//	VertexBufferDesc.ByteWidth = GetVertexStructSize() * GetVertexCount();
//	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	VertexBufferDesc.CPUAccessFlags = 0;
//	VertexBufferDesc.MiscFlags = 0;
//	VertexBufferDesc.StructureByteStride = 0;
//
//	D3D11_SUBRESOURCE_DATA VertexInitData;
//
//	VertexInitData.pSysMem = vertices;
//
//	HR(device->GetDevice()->CreateBuffer(&VertexBufferDesc, &VertexInitData, &GetVertexBuffer()));
//}


