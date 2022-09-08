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

void RenderInfoManager::PushLightInfo(size_t ID, LightInfo infos)
{
	auto data = m_LightInfo.find(ID);

	if (data == m_LightInfo.end())
	{
		//���� �������Դϴ�. ���� �־��ݽô�!
		m_LightInfo.emplace(ID, infos);
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

LightInfo* RenderInfoManager::GetLightInfo(size_t ID)
{
	auto data = m_LightInfo.find(ID);
	return((data == m_LightInfo.end()) ? nullptr : &data->second);
}

void RenderInfoManager::Clear()
{

}

void RenderInfoManager::DeleteMeshInfo(size_t ID)
{
	m_MeshInfo.erase(ID);
}


void RenderInfoManager::DeleteLightInfo(size_t ID)
{
	m_LightInfo.erase(ID);
}

