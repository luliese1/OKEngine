#pragma once

/// <summary>
/// 
/// RenderInfoManager
/// 게임엔진의 오브젝트들의 정보를 들고 있을 것이다.
/// 저장해두는 이유는 게임엔진<->그래픽스엔진 간의 데이터의 전송을 줄이기 위하여.
/// 게임엔진쪽에서 렌더 인포가 변경되었을때만, 데이터를 전송하고 그 오브젝트의 guid 값을 통하여 값을 수정한다.
/// 
/// </summary>

class RenderInfoManager
{
	public:
		//데이터 넣기 or 업데이트
		void PushMeshInfo(size_t ID, MeshInfo infos);
		void PushLightInfo(size_t ID, LightInfo infos);

		//데이터 가져오기 있으면 오브젝트의 포인터 반환, 아니면 nullptr
		MeshInfo* GetMeshInfo(size_t ID);
		LightInfo* GetLightInfo(size_t ID);

		//게임오브젝트에서 해제가 되었을때, 그래픽스에서도 똑같이 해제해줍시다.
		void DeleteMeshInfo(size_t ID);
		void DeleteLightInfo(size_t ID);

		//씬이 바뀌었을 때에는 모두다 이전 씬의 데이터는 제거해줘야함!..
		void Clear();

	private:

		//size_t는 오브젝트의 GUID 입니다.
		std::unordered_map<size_t, MeshInfo> m_MeshInfo;
		std::unordered_map<size_t, LightInfo> m_LightInfo;
};

