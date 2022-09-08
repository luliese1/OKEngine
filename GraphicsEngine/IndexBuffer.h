#pragma once

//인덱스 버퍼
class Device;

class IndexBuffer
{
public:
	void Initialize(void* indices, int indexSize, std::shared_ptr<Device> device);
	void Release();

	ComPtr<ID3D11Buffer> GetIndexBuffer() const { return m_IndexBuffer; }
	UINT GetIndexCount() const { return m_IndexCount; }

private:
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	UINT m_IndexCount;
};

