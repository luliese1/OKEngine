#pragma once

class Device;

class SamplerBuffer
{
public:
	SamplerBuffer() : m_registerSlot(128), m_BufferName() {}

	void Initialize(UINT registerSlot, const std::string& name);

	std::wstring GetBufferName() const { return m_BufferName; }
	UINT GetRegisterSlot() const { return m_registerSlot; }

private:
	UINT m_registerSlot;
	std::wstring m_BufferName;
};

