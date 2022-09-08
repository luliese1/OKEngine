#pragma once
#include "ASEParser.h"

class IGraphicsEngine;
class GameObject;

/// <summary>
/// 
/// ase 파일의 포맷을 들고올 녀석입니다.
/// 
/// </summary>


class ModelLoader
{
public:
	static void LoadModel(const ModelManageInfo& infos, std::shared_ptr<IGraphicsEngine> graphics);
	static void AttachMeshs(ObjectGUID id, std::shared_ptr<GameObject> targetObj);

	static void Finalize();

private:
	static ObjectGUID LoadASEFile(const std::wstring& path, ObjectGUID id);
	static ObjectGUID LoadFBXFile(const std::wstring& path, ObjectGUID id);

	//버텍스 정보와 텍스쳐생성을 그래픽스에게 맡기는 함수
	static bool SendToASEInfosToGraphics(ObjectGUID id, ModelManageInfo& modelManageInfo, std::shared_ptr<IGraphicsEngine> graphics);
	//게임엔진단에서 필요한 정보들을 로드한다
	static bool LoadModelNodesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	//메테리얼 정보를 
	static bool LoadMaterialInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	static bool LoadAnimaitonInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	static bool LoadBonesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);

	static std::map<ObjectGUID, ModelManageInfo> m_ModelInfos;
	static std::map<ObjectGUID, ModelMaterialInfo> m_ModelMaterialInfo;
	static std::unique_ptr<ASEParser> m_ASEParser;

	ModelLoader();
};

