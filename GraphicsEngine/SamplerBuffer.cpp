#include "pch.h"
#include "SamplerBuffer.h"

void SamplerBuffer::Initialize(UINT registerSlot, const std::string& name)
{
	m_registerSlot = registerSlot;
	m_BufferName = name;
}
