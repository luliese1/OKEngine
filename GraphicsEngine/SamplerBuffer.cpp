#include "pch.h"
#include "SamplerBuffer.h"

void SamplerBuffer::Initialize(UINT registerSlot, const std::string& name)
{
	m_BufferName = std::wstring(name.begin(), name.end());
	m_registerSlot = registerSlot;
}
