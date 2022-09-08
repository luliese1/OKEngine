#include "pch.h"
#include "dllRenderStruct.h"

GRAPHICSENGINE_SHADER_DESC::GRAPHICSENGINE_SHADER_DESC(const std::wstring& shaderSetName, const std::wstring& VSPath, const std::wstring& PSPath, GRAPHICSENGINE_SHADER_MACRO_DESC* ShaderMacro, UINT ShaderMacroCnt)
: m_ShaderSetName(shaderSetName), m_VertexShaderPath(VSPath), m_PixelShaderPath(PSPath), m_ShaderMacro(ShaderMacro), m_ShaderMacroCnt(ShaderMacroCnt)
{

}
