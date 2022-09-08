#pragma once

class Device;

//쉐이더에서 텍스쳐를 바인딩할때 필요한 정보구조체.
//쉐이더 베이스에서 사용한다.

class TextureResourceBuffer
{
public:
	TextureResourceBuffer() : m_registerSlot(128), m_BufferName(){}

	void Initialize(UINT registerSlot, const std::string& name);

	std::string GetBufferName() const { return m_BufferName; }
	UINT GetRegisterSlot() const { return m_registerSlot; }

private:
	UINT m_registerSlot;
	std::string m_BufferName;
};

