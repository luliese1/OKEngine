#pragma once

/// <summary>
/// 
/// RenderInfoManager
/// ���ӿ����� ������Ʈ���� ������ ��� ���� ���̴�.
/// �����صδ� ������ ���ӿ���<->�׷��Ƚ����� ���� �������� ������ ���̱� ���Ͽ�.
/// ���ӿ����ʿ��� ���� ������ ����Ǿ�������, �����͸� �����ϰ� �� ������Ʈ�� guid ���� ���Ͽ� ���� �����Ѵ�.
/// 
/// </summary>

class RenderInfoManager
{
	public:
		//������ �ֱ� or ������Ʈ
		void PushMeshInfo(size_t ID, MeshInfo infos);
		void PushLightInfo(size_t ID, LightInfo infos);

		//������ �������� ������ ������Ʈ�� ������ ��ȯ, �ƴϸ� nullptr
		MeshInfo* GetMeshInfo(size_t ID);
		LightInfo* GetLightInfo(size_t ID);

		//���ӿ�����Ʈ���� ������ �Ǿ�����, �׷��Ƚ������� �Ȱ��� �������ݽô�.
		void DeleteMeshInfo(size_t ID);
		void DeleteLightInfo(size_t ID);

		//���� �ٲ���� ������ ��δ� ���� ���� �����ʹ� �����������!..
		void Clear();

	private:

		//size_t�� ������Ʈ�� GUID �Դϴ�.
		std::unordered_map<size_t, MeshInfo> m_MeshInfo;
		std::unordered_map<size_t, LightInfo> m_LightInfo;
};

