#pragma once

//https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-buffers-intro
//https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-buffers-constant-how-to

class Device;


enum class CONSTANTBUFFER_UPDATE_CLASSFICATION
{
	CONSTANTBUFFER_PERFRAME,
	CONSTANTBUFFER_PEROBJECT,
	CONSTANTBUFFER_UNKNOWN
};

struct ConstantBufferLayout
{
	std::string m_VariableName;
	UINT m_DataSize;
	UINT m_StartOffset; //데이터의 시작 위치
};

struct ConstantBuffer_Desc //ConstantBufferInfo
{
	std::vector<ConstantBufferLayout> m_ConstantBufferType;
};


class ConstantBuffer
{
public:
	void Initialize(UINT bufferSize, UINT registerSlot, const std::string& name, CONSTANTBUFFER_UPDATE_CLASSFICATION updateType, std::shared_ptr<Device> device);
	void Release();
	void PushConstantBufferLayout(ConstantBufferLayout& constbufferlayout);

	ComPtr<ID3D11Buffer> GetConstantBuffer(){ return m_ConstantBuffer;}
	const std::vector<ConstantBufferLayout>& GetConstantBufferLayout(){ return m_ConstantBufferDesc.m_ConstantBufferType;}

	UINT GetRegisterSlot() const { return m_registerSlot; }
	int GetBufferSize() const { return m_BufferSize; }

private:
	CONSTANTBUFFER_UPDATE_CLASSFICATION m_UpdateType;
	ConstantBuffer_Desc m_ConstantBufferDesc;
	ComPtr<ID3D11Buffer> m_ConstantBuffer;

	UINT m_registerSlot;
	std::string m_BufferName;
	int m_BufferSize;

	//template<typename T>
	//void Initialize(UINT slotidx, std::shared_ptr<Device> device);
};

//
//template<typename T>
//void ConstantBuffer::Initialize(UINT slotidx, std::shared_ptr<Device> device)
//{
//	T TempConstData;
//	
//	D3D11_BUFFER_DESC ConstantBufferDesc;
//
//	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	ConstantBufferDesc.ByteWidth = sizeof(T);
//	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	ConstantBufferDesc.MiscFlags = 0;
//	ConstantBufferDesc.StructureByteStride = 0;
//	
//	D3D11_SUBRESOURCE_DATA ConstantInitData;
//	ConstantInitData.pSysMem = &TempConstData;
//	ConstantInitData.SysMemPitch = 0;
//	ConstantInitData.SysMemSlicePitch = 0;
//
//	HRESULT hr = device->GetDevice()->CreateBuffer(&ConstantBufferDesc, &ConstantInitData, m_ConstantBuffer.GetAddressOf());
//
//	HR(hr);
//	
//	m_BufferSize = sizeof(T);
//	m_SlotIdx = slotidx;
//}
//
