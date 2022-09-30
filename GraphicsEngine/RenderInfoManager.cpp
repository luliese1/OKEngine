#include "pch.h"
#include "RenderInfoManager.h"

void RenderInfoManager::PushMeshInfo(size_t ID, MeshInfo infos)
{
	auto data = m_MeshInfo.find(ID);
	

	if (data == m_MeshInfo.end())
	{
		//���� �������Դϴ�. ���� �־��ݽô�!
		m_MeshInfo.emplace(ID, infos);
	}
	else
	{
		//�ִ� �������̹Ƿ� ������ ���Ƴ����
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

