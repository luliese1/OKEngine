#pragma once
#include "ASEParser.h"

class IGraphicsEngine;
class GameObject;

/// <summary>
/// 
/// ase ������ ������ ���� �༮�Դϴ�.
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

	//���ؽ� ������ �ؽ��Ļ����� �׷��Ƚ����� �ñ�� �Լ�
	static bool SendToASEInfosToGraphics(ObjectGUID id, ModelManageInfo& modelManageInfo, std::shared_ptr<IGraphicsEngine> graphics);
	//���ӿ����ܿ��� �ʿ��� �������� �ε��Ѵ�
	static bool LoadModelNodesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	//���׸��� ������ 
	static bool LoadMaterialInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	static bool LoadAnimaitonInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);
	static bool LoadBonesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo);

	static std::map<ObjectGUID, ModelManageInfo> m_ModelInfos;
	static std::map<ObjectGUID, ModelMaterialInfo> m_ModelMaterialInfo;
	static std::unique_ptr<ASEParser> m_ASEParser;

	ModelLoader();
};

