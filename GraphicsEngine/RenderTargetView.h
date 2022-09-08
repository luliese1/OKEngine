#pragma once

class Device;
class VertexBuffer;
class IndexBuffer;

class RenderTargetView
{
public:
	RenderTargetView();
	~RenderTargetView();

	void Initialize(DXGI_FORMAT format, ScreenInfo& SInfo, std::shared_ptr<Device> device);
	void OnResize(DXGI_FORMAT format, ScreenInfo& Sinfo, std::shared_ptr<Device> device);

	//void SetRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* depth);

	//��Ƽ���ø��� Ŀ������ ���... �� ������ ������!
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView();
	ComPtr<ID3D11RenderTargetView> GetRenderTargetView();

	void Finalize();
	void ResolveSubresource(std::shared_ptr<Device> device);
private:
	bool m_MSAAFlag;

	//��Ƽ����
	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11RenderTargetView> m_RTV;
	ComPtr<ID3D11ShaderResourceView> m_SRV;

	ComPtr<ID3D11Texture2D> m_SingleSampledTexture;
	ComPtr<ID3D11RenderTargetView> m_SingleSampledRTV;
	ComPtr<ID3D11ShaderResourceView> m_SingleSampledSRV;
};

//���ؽ����̴��� �� �����簢��. �ϳ��� ������ ��!
class ViewBox
{
public:
	ViewBox() : m_VertexBuffer(){};

	struct ViewBoxVertex
	{
		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT2 m_uv;
	};

public:
	void Initialize(std::shared_ptr<Device> device);
	void OnResize(std::shared_ptr<Device> device);

	void Finalize();


	std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return m_VertexBuffer; }
	std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return m_IndexBuffer; }

private:
	std::shared_ptr<VertexBuffer> m_VertexBuffer;
	std::shared_ptr<IndexBuffer> m_IndexBuffer;
};