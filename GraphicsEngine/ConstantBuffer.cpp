#include "pch.h"
#include "ConstantBuffer.h"
#include "Device.h"

void ConstantBuffer::Initialize(UINT bufferSize, UINT registerSlot, const std::string& name, CONSTANTBUFFER_UPDATE_CLASSFICATION updateType, std::shared_ptr<Device> device)
{
	m_registerSlot = registerSlot;
	m_UpdateType = updateType;
	m_BufferName = name;
	m_BufferSize = bufferSize;

	CD3D11_BUFFER_DESC cBufferDesc(bufferSize, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

	HR(device->GetDevice()->CreateBuffer(&cBufferDesc, nullptr, m_ConstantBuffer.GetAddressOf()));

#if defined(DEBUG) || defined(_DEBUG)
	device->GetDevice()->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("ConstantBuffer") - 1, "ConstantBuffer");
#endif

}


void ConstantBuffer::Release()
{

}

void ConstantBuffer::PushConstantBufferLayout(ConstantBufferLayout& constbufferlayout)
{
	m_ConstantBufferDesc.m_ConstantBufferType.push_back(constbufferlayout);
}

