#include "pch.h"
#include "IndexBuffer.h"
#include "Device.h"

void IndexBuffer::Initialize(void* indices, int indexSize, std::shared_ptr<Device> device)
{
	m_IndexCount = indexSize;

	D3D11_BUFFER_DESC IndexBuffeDesc;

	IndexBuffeDesc.Usage = D3D11_USAGE_IMMUTABLE;
	IndexBuffeDesc.ByteWidth = sizeof(uint32) * indexSize;
	IndexBuffeDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBuffeDesc.CPUAccessFlags = 0;
	IndexBuffeDesc.MiscFlags = 0;
	IndexBuffeDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA IndexInitData;

	IndexInitData.pSysMem = indices;

	HR(device->GetDevice()->CreateBuffer(&IndexBuffeDesc, &IndexInitData, m_IndexBuffer.GetAddressOf()));
#if defined(DEBUG) || defined(_DEBUG)
	m_IndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("IndexBuffer") - 1, "IndexBuffer");
#endif
}

void IndexBuffer::Release()
{

}
