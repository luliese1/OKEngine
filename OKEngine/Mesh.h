#pragma once
#include "ComponentBase.h"

///���Ӱ� �¾ �޽�ģ��

class Mesh : public ComponentBase
{
public:
	MeshInfo GetMeshInfo() const { return m_MeshInfo; }

	void SetMeshInfo(MeshInfo val) { m_MeshInfo = std::move(val); }
	bool GetIsChanged() const { return m_IsChanged; }


public:
	Mesh();
	virtual ~Mesh();

	void Update() override;
	void Init() override;

private:
	//�׷��Ƚ��� ���� ������.
	MeshInfo m_MeshInfo;
	bool m_IsChanged;

	//size_t  m_MeshId;
	//size_t  m_MaterialID;
	//size_t  m_VertexShaderId;
	//size_t  m_PixelShaderId;
	//////�ؽ��� ������ ��¼��.. ��.. �������� �������ٱ�
	//size_t  m_TextureID[4];

	//UINT8 m_RasterizerState;

	//MeshType m_MeshType;
};

