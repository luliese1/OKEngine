#pragma once


class Device
{
public:
	void Initialize();

	void DebugObject();
	void Flush();

	void Finalize();

	ComPtr<ID3D11Device> GetDevice(){ return m_pD3DDevice; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_pD3DContext; }

private:
	ComPtr<ID3D11Device> m_pD3DDevice;
	ComPtr<ID3D11DeviceContext> m_pD3DContext;

};

