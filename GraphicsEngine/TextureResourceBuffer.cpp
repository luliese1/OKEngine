#include "pch.h"
#include "TextureResourceBuffer.h"

void TextureResourceBuffer::Initialize(UINT registerSlot, const std::string& name)
{
	m_registerSlot = registerSlot;
	m_BufferName = name;

}
