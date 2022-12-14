#include "pch.h"
#include "RenderInfoManager.h"

void RenderInfoManager::PushMeshInfo(size_t ID, MeshInfo infos)
{
	auto data = m_MeshInfo.find(ID);
	

	if (data == m_MeshInfo.end())
	{
		//없던 데이터입니다. 새로 넣어줍시다!
		m_MeshInfo.emplace(ID, infos);
	}
	else
	{
		//있던 데이터이므로 데이터 갈아끼우기
		data->second = infos;
	}
}

MeshInfo* RenderInfoManager::GetMeshInfo(size_t ID)
{
	auto data = m_MeshInfo.find(ID);
	return((data == m_MeshInfo.end()) ? nullptr : &data->second );
}

void RenderInfoManager::Clear()
{

}

void RenderInfoManager::DeleteMeshInfo(size_t ID)
{
	m_MeshInfo.erase(ID);
}

