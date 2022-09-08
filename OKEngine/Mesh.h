#pragma once
#include "ComponentBase.h"

///새롭게 태어난 메쉬친구

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
	//그래픽스서 받을 데이터.
	MeshInfo m_MeshInfo;
	bool m_IsChanged;

	//size_t  m_MeshId;
	//size_t  m_MaterialID;
	//size_t  m_VertexShaderId;
	//size_t  m_PixelShaderId;
	//////텍스쳐 개수는 어쩌냐.. 흠.. 동적으로 생성해줄까
	//size_t  m_TextureID[4];

	//UINT8 m_RasterizerState;

	//MeshType m_MeshType;
};

