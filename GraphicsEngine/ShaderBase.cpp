#include "pch.h"
#include "ShaderBase.h"
#include "ConstantBuffer.h"
#include "TextureResourceBuffer.h"
#include "SamplerBuffer.h"

std::shared_ptr<ConstantBuffer> ShaderBase::m_PerFrameBuffer = nullptr;

ShaderBase::~ShaderBase()
{
	m_ConstantBuffers.clear();
	m_TextureResourceBuffers.clear();
	m_SamplerBuffers.clear();
}

void ShaderBase::ReleasePerFrameBuffer()
{
	m_PerFrameBuffer.reset();
}

void ShaderBase::CreateConstantBuffer(UINT registerSlot, ID3D11ShaderReflectionConstantBuffer* BufferInfo, std::shared_ptr<Device>& device)
{
	//참조한 링크
	//https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0

	D3D11_SHADER_BUFFER_DESC bufferDesc;

	if (SUCCEEDED(BufferInfo->GetDesc(&bufferDesc)))
	{
		CONSTANTBUFFER_UPDATE_CLASSFICATION bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN;

		if (strcmp(bufferDesc.Name, "cbPerObject") == 0)
		{
			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PEROBJECT;
		}
		else if (strcmp(bufferDesc.Name, "cbPerFrame") == 0)
		{
	
			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PERFRAME;
		}
		else
		{
			bufferUpdateType = CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN;
		}


		//버퍼의 성분에 따라 들어가는 데이터가 다르니까, fixed는 고정돼있는 데이터 타입에따라 데이터를 집어 넣어준다. 
		switch (bufferUpdateType)
		{
		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PERFRAME:
			{
				if(m_PerFrameBuffer != nullptr)
					break;

				m_PerFrameBuffer = std::make_shared<ConstantBuffer>();
				m_PerFrameBuffer->Initialize(bufferDesc.Size, registerSlot, bufferDesc.Name, bufferUpdateType, device);

				for (int i = 0; i < bufferDesc.Variables; i++)
				{
					ID3D11ShaderReflectionVariable* BufferVariable = BufferInfo->GetVariableByIndex(i);
					D3D11_SHADER_VARIABLE_DESC variableDesc;

					BufferVariable->GetDesc(&variableDesc);

					ConstantBufferLayout bufferLayout;

					bufferLayout.m_DataSize = variableDesc.Size;
					bufferLayout.m_StartOffset = variableDesc.StartOffset;
					bufferLayout.m_VariableName = variableDesc.Name;

					m_PerFrameBuffer->PushConstantBufferLayout(bufferLayout);
					//성분들을 규약할 수 있으려나..? 
				}
			}
			break;
		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_PEROBJECT:
			{
				std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBuffer>();
				m_ConstantBuffers.emplace_back(buffer);
				buffer->Initialize(bufferDesc.Size, registerSlot, bufferDesc.Name, bufferUpdateType, device);

				for (int i = 0; i < bufferDesc.Variables; i++)
				{
					ID3D11ShaderReflectionVariable* BufferVariable = BufferInfo->GetVariableByIndex(i);
					D3D11_SHADER_VARIABLE_DESC variableDesc;

					BufferVariable->GetDesc(&variableDesc);

					ConstantBufferLayout bufferLayout;

					bufferLayout.m_DataSize = variableDesc.Size;
					bufferLayout.m_StartOffset = variableDesc.StartOffset;
					bufferLayout.m_VariableName = variableDesc.Name;
					buffer->PushConstantBufferLayout(bufferLayout);
					//성분들을 규약할 수 있으려나..? 
				}
			}
			break;
		case CONSTANTBUFFER_UPDATE_CLASSFICATION::CONSTANTBUFFER_UNKNOWN:
			break;
		default:
			break;
		}
	}
}

void ShaderBase::CreateTextureResourceBuffer(UINT registerSlot, const std::string& name)
{
	std::shared_ptr<TextureResourceBuffer> tempBuffer = std::make_shared<TextureResourceBuffer>();
	tempBuffer->Initialize(registerSlot, name);

	m_TextureResourceBuffers.push_back(tempBuffer);
}

void ShaderBase::CreateSamplerBuffer(UINT registerSlot, const std::string& name)
{
	std::shared_ptr<SamplerBuffer> tempBuffer = std::make_shared<SamplerBuffer>();
	tempBuffer->Initialize(registerSlot, name);

	m_SamplerBuffers.push_back(tempBuffer);
}

void ShaderBase::CreateShaderBindBuffers(ComPtr<ID3D11ShaderReflection> ShaderReflector, std::shared_ptr<Device>& device)
{
	D3D11_SHADER_DESC shaderDesc;
	ShaderReflector->GetDesc(&shaderDesc);

	for (int i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC InputBindDesc;
		ShaderReflector->GetResourceBindingDesc(i, &InputBindDesc);

		//할일
		//desc로 인자를 넣어서 만들 수 있도록 하기.
		//sampler 함수 채우기

		switch (InputBindDesc.Type)
		{
		case D3D_SIT_CBUFFER:
			{
				ID3D11ShaderReflectionConstantBuffer* BufferInfo = ShaderReflector->GetConstantBufferByName(InputBindDesc.Name);
				CreateConstantBuffer(InputBindDesc.BindPoint, BufferInfo, device);
			}
			break;
		case D3D_SIT_TEXTURE:
			{
				CreateTextureResourceBuffer(InputBindDesc.BindPoint, InputBindDesc.Name);
				//srvRegisterSlot = resource_desc.BindPoint;
				//_shaderResourceViewData[resource_desc.Name] = make_shared<ShaderResourceBuffer>(resource_desc.Name, srvRegisterSlot);
			}
			break;
		case D3D_SIT_SAMPLER:
			{
				CreateSamplerBuffer(InputBindDesc.BindPoint, InputBindDesc.Name);
				//CreateSamplerBuffer(ShaderReflector, ShaderBase, InputBindDesc.Name);
				//samplerRegisterSlot = resource_desc.BindPoint;
				//_samplerStateData[resource_desc.Name] = make_shared<SamplerState>(resource_desc.Name, samplerRegisterSlot);
			}
			break;
		default:
			break;
		}
	}
}

std::vector<D3D_SHADER_MACRO> ShaderBase::CreateD3DShaderMacro(GRAPHICSENGINE_SHADER_MACRO_DESC* macro, UINT macroCnt)
{
	std::vector<D3D_SHADER_MACRO> d3dmacro;

	for (int macroidx = 0; macroidx < macroCnt; macroidx++)
	{
		D3D_SHADER_MACRO temp = { macro[macroidx].Name,  macro[macroidx].Definition};
		d3dmacro.push_back(temp);
	}

	d3dmacro.push_back(D3D_SHADER_MACRO{NULL, NULL});

	return d3dmacro;
}


