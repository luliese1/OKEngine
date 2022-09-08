#pragma once
#include "RenderPassBase.h"

/// <summary>
/// 
///  쉐이더에서 공통적인
///	 인자를 뽑아낸다.
/// 
/// </summary>

class Device;
class ConstantBuffer;
class TextureResourceBuffer;
class SamplerBuffer;

class ShaderBase
{
public:
	virtual ~ShaderBase();
	virtual bool Initialize(std::wstring path, GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt ,std::shared_ptr<Device>& device) abstract;
	void SetConstantBufferSize(int size) { m_ConstantBuffers.resize(size); }

	ComPtr<ID3DBlob>& GetBlob() { return m_ShaderBlob; }
	//std::vector<ComPtr<ID3D11ClassInstance>>& GeClassInstances(){ return m_ClassInstaces; } 
	//std::shared_ptr<ConstantBuffer>& GetConstantBuffer(){ return m_ConstantBuffer; }

	std::vector<std::shared_ptr<ConstantBuffer>>& GetConstantBuffer() { return m_ConstantBuffers; }
	std::vector<std::shared_ptr<TextureResourceBuffer>>& GetTextureResourceBuffer() { return m_TextureResourceBuffers; }
	std::vector<std::shared_ptr<SamplerBuffer>>& GetSamplerBuffer() { return m_SamplerBuffers; }

	UINT GetTextureResourceBufferSize(){ return m_TextureResourceBuffers.size();}

	static std::shared_ptr<ConstantBuffer> GetPerFrameBuffer(){return m_PerFrameBuffer;}
	static void ReleasePerFrameBuffer();


protected:
	void CreateShaderBindBuffers(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<Device>& device);
	void CreateConstantBuffer(UINT registerSlot, ID3D11ShaderReflectionConstantBuffer* BufferInfo, std::shared_ptr<Device>& device);
	void CreateTextureResourceBuffer(UINT registerSlot, const std::string& name);
	void CreateSamplerBuffer(UINT registerSlot, const std::string& name);

	std::vector<D3D_SHADER_MACRO> CreateD3DShaderMacro(GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt);

	static std::shared_ptr<ConstantBuffer> m_PerFrameBuffer;

	ComPtr<ID3DBlob> m_ShaderBlob;

	std::vector<std::shared_ptr<ConstantBuffer>> m_ConstantBuffers;
	std::vector<std::shared_ptr<TextureResourceBuffer>> m_TextureResourceBuffers;
	std::vector<std::shared_ptr<SamplerBuffer>> m_SamplerBuffers;

};