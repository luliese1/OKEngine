#include "EnginePCH.h"
#include "ModelLoader.h"
#include "IGraphicsEngine.h"
#include <concurrent_queue.h>
#include "ResourceHashTable.h"

#include "ASEData.h"

#include "GameObject.h"
#include "Scene.h"

#include "Transform.h"
#include "Mesh.h"

std::map<ObjectGUID, ModelManageInfo> ModelLoader::m_ModelInfos;
std::map<ObjectGUID, ModelMaterialInfo> ModelLoader::m_ModelMaterialInfo;
std::unique_ptr<ASEParser> ModelLoader::m_ASEParser = nullptr;

void ModelLoader::LoadModel(const ModelManageInfo& infos, shared_ptr<IGraphicsEngine> graphics)
{
	ModelManageInfo tempInfo;

	tempInfo = infos;
	std::wstring fileextension;

	if (m_ModelInfos.find(infos.m_GUID) == m_ModelInfos.end())
	{
		for (auto iter = infos.m_name.end() - 1; iter != infos.m_name.begin(); iter--)
		{
			if (*iter == '.')
			{
				fileextension.assign(iter + 1, infos.m_name.end());
				break;
			}
		}

		if (fileextension == L"ASE" || fileextension == L"ase")
		{
			LoadASEFile(infos.m_path, infos.m_GUID);
			LoadModelNodesInfoFromASE(infos.m_GUID, tempInfo);
			//문제가 될 코드이다.............. 어케수정하지 흠..
			LoadMaterialInfoFromASE(infos.m_GUID, tempInfo);

			SendToASEInfosToGraphics(infos.m_GUID, tempInfo, graphics);
		}
		else if (fileextension == L"fbx" || fileextension == L"FBX")
		{
			LoadFBXFile(infos.m_path, infos.m_GUID);
			//SendToFBXInfosToGraphics(infos.m_GUID, graphics);
		}

		m_ModelInfos.emplace(infos.m_GUID, tempInfo);
	}

	return;
}

ObjectGUID ModelLoader::LoadASEFile(const std::wstring& path, ObjectGUID id)
{
	if (m_ASEParser == nullptr)
	{
		m_ASEParser = std::make_unique<ASEParser>();
	}
	m_ASEParser->Load((LPSTR)ws2s(path).c_str(), id);

	return id;
}
ObjectGUID ModelLoader::LoadFBXFile(const std::wstring& path, ObjectGUID id)
{
	return 0;
}

bool ModelLoader::SendToASEInfosToGraphics(ObjectGUID id, ModelManageInfo& modelManageInfo, std::shared_ptr<IGraphicsEngine> graphics)
{
	std::vector<ASEData::BaseObjectInfo*>* objectData = nullptr;

	if (m_ASEParser->GetObject(id, objectData))
	{
		auto infos = *objectData;

		//지금은 ASE에서 바로 가져오고있으나,
		//나중엔 파싱한 데이터 (ModelLoaderInfo)에서 보관하여, 그 데이터를 통하여 데이터를 전송해야한다.
		for (int subMeshIndex = 0; subMeshIndex < infos.size(); subMeshIndex++)
		{
			if (infos[subMeshIndex]->m_type == ObjectType_Geomobject)
			{
				ASEData::GeomObject* castedInfo = static_cast<ASEData::GeomObject*>(infos[subMeshIndex]);

				graphics->CreateVertexBuffer((void*)&castedInfo->m_opt_vertex[0], sizeof(StaticMeshVertex), castedInfo->m_opt_vertex.size(), id, subMeshIndex);
				graphics->CreateIndexBuffer((void*)&castedInfo->m_opt_index[0], castedInfo->m_opt_index.size() * 3, id, subMeshIndex);

				if (modelManageInfo.m_ModelInfo[subMeshIndex].m_MateriaRef != -1)
				{
					auto find = m_ModelMaterialInfo.find(modelManageInfo.m_ModelInfo[subMeshIndex].m_MateriaGUID);
					if (find == m_ModelMaterialInfo.end())
					{
						assert(" SendToASEInfoToGraphics :: ASE 파싱과정에서 메테리얼을 발견하지 못했습니다.");
						break;
					}

					auto& material = find->second;
					if (material.m_LoadState == false)
					{
						material.m_LoadState = true;
						for (int textureidx = 0; textureidx < material.m_TextureCnt; textureidx++)
						{
							graphics->CreateTexture(material.m_TeturePath[textureidx], material.m_Texture[textureidx]);
						}
					}
				}
			}
		}

		return true;
	}

	return false;
}
bool ModelLoader::LoadModelNodesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo)
{
	std::vector<ASEData::BaseObjectInfo*>* objectData = nullptr;

	if (m_ASEParser->GetObject(id, objectData))
	{
		auto InfoList = *objectData;

		modelManageInfo.m_ModelCount = InfoList.size();

		bool IsThereSameObj = false;
		modelManageInfo.m_ModelInfo.resize(modelManageInfo.m_ModelCount);

		int subMeshCnt = 0;

		//정보들을 순회하면서 노드들의 정보를 기록한다
		for (UINT nodeIdx = 0; nodeIdx < modelManageInfo.m_ModelCount; nodeIdx++)
		{
			auto& nowModelInfo = modelManageInfo.m_ModelInfo[nodeIdx];

			//부모 자식관계를 정리한다.
			IsThereSameObj = false;
			if (InfoList[nodeIdx]->m_nodeparent == "")
			{
				nowModelInfo.m_parent = -1;
			}
			for (UINT parentIdx = 0; parentIdx < modelManageInfo.m_ModelCount; parentIdx)
			{
				if (InfoList[nodeIdx]->m_nodeparent == InfoList[parentIdx]->m_nodename)
				{
					nowModelInfo.m_parent = parentIdx;
					IsThereSameObj = true;
					break;
				}
				parentIdx++;
			}
			if (!IsThereSameObj)
			{
				nowModelInfo.m_parent = -1;
			}

			//노드가 메쉬를 갖고있는지 판단한다.
			if (InfoList[nodeIdx]->m_type == ObjectType_Geomobject)
			{
				ASEData::GeomObject* castedInfo = static_cast<ASEData::GeomObject*>(InfoList[nodeIdx]);

				nowModelInfo.m_nodeTM = InfoList[nodeIdx]->m_NodeTm;
				nowModelInfo.m_MateriaRef = castedInfo->m_material_ref;
				nowModelInfo.m_SubMeshInfo = subMeshCnt;
				subMeshCnt++;

				if (castedInfo->m_IsSkinningMesh)
				{
					nowModelInfo.m_ModelType = ModelInfo::ModelMeshType::SKINNING;
					//본을 신경써야함
				}
				else
				{
					nowModelInfo.m_ModelType = ModelInfo::ModelMeshType::STATIC;
				}
			}
			else if (InfoList[nodeIdx]->m_type == ObjectType_HelperObject)
			{
				nowModelInfo.m_ModelType = ModelInfo::ModelMeshType::NONMESH;
			}
			else
			{
				assert("error! 정의되지 않은 ase object 타입입니다");
			}
		}
	}

	return true;
}
bool ModelLoader::LoadMaterialInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo)
{
	std::vector<ASEData::ASEMaterial*>* materialData = nullptr;

	if (m_ASEParser->GetMaterial(id, materialData))
	{
		for (auto& pMat : *materialData)
		{
			ModelMaterialInfo tempMat;

			tempMat.m_Ambient = Vector4(pMat->m_material_ambient);
			tempMat.m_Ambient.w = 1.f;
			tempMat.m_Diffuse = Vector4(pMat->m_material_diffuse);
			tempMat.m_Diffuse.w = 1.f;
			tempMat.m_Specular = Vector4(pMat->m_material_specular);
			tempMat.m_Specular.w = 1.f;
			//reflect
			tempMat.m_Reflect = { 0.0f,0.0f,0.0f,1.f };


			tempMat.m_name = modelManageInfo.m_name + s2ws(pMat->m_material_name);
			tempMat.m_GUID = ResourceHashTable::GetHashKey(tempMat.m_name);
			tempMat.m_TextureCnt = 0;

			if (pMat->m_map_diffuse != nullptr)
			{
				std::wstring diffusePath = s2ws(pMat->m_map_diffuse->m_bitmap);
				std::string::size_type pos = std::wstring(diffusePath).find_last_of(L"/");

				std::wstring diffuseName;
				if (pos == -1)
					diffuseName = diffusePath;
				else
					diffuseName = diffusePath.substr(pos + 1, diffusePath.size());

				tempMat.m_UsingAlbedo = true;

				tempMat.m_Texture[0] = ResourceHashTable::GetHashKey(diffusePath);
				tempMat.m_TeturePath[0] = std::move(diffusePath);
				tempMat.m_TextureCnt++;
			}

			if (pMat->m_map_bump != nullptr)
			{
				std::wstring bumpPath = s2ws(pMat->m_map_bump->m_bitmap);
				std::string::size_type pos = std::wstring(bumpPath).find_last_of(L"/");

				std::wstring bumpName;
				if (pos == -1)
					bumpName = bumpPath;
				else
					bumpName = bumpPath.substr(pos + 1, bumpPath.size());

				tempMat.m_UsingAlbedo = false;

				tempMat.m_Texture[1] = ResourceHashTable::GetHashKey(bumpPath);
				tempMat.m_TeturePath[1] = std::move(bumpPath);
				tempMat.m_TextureCnt++;
			}

			//노드들의 머테리얼에 따라서 머테리얼 GUID 삽입
			//상당히 더러운 코드이다..
			for (auto& nodes : modelManageInfo.m_ModelInfo)
			{
				if (nodes.m_MateriaRef == pMat->m_materialnumber)
				{
					nodes.m_MateriaGUID = tempMat.m_GUID;
				}
			}

			m_ModelMaterialInfo.emplace(tempMat.m_GUID, tempMat);
		}

		return true;
	}

	return false;
}

bool ModelLoader::LoadBonesInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo)
{
	return true;
}
bool ModelLoader::LoadAnimaitonInfoFromASE(ObjectGUID id, ModelManageInfo& modelManageInfo)
{
	return true;
}

void ModelLoader::AttachMeshs(ObjectGUID meshid, shared_ptr<GameObject> targetObj)
{
	auto scene = targetObj->GetScene().lock();

	auto find = m_ModelInfos.find(meshid);
	if (find == m_ModelInfos.end())
	{
		assert("AttatchMesh: 찾은 데이터가 없읍니다");
		return;
	}

	auto& modelManageinfo = find->second;
	auto& modelinfo = modelManageinfo.m_ModelInfo;

	for (int submeshId = 0; submeshId < modelinfo.size(); submeshId++)
	{
		auto nowModelInfo = modelinfo[submeshId];
		auto tempObj = scene->MakeGameObject(nowModelInfo.m_name);
		tempObj->SetParent(targetObj);
		//트랜스폼 설정
		{
			shared_ptr<Transform> pTransform = tempObj->GetComponent<Transform>();
			Matrix nodeTM = nowModelInfo.m_nodeTM;
			Matrix localTM;
			if (nowModelInfo.m_parent == -1)
			{
				localTM = nodeTM;
			}
			else
			{
				localTM = nodeTM * modelinfo[nowModelInfo.m_parent].m_nodeTM.Invert();
			}

			Vector3 scale;
			Quaternion Quaternionrot;
			Vector3 pos;
			localTM.Decompose(scale, Quaternionrot, pos);
			Vector3 rot = Quaternionrot.ToEuler();

			////x 
			//float q2sqr = rot.m128_f32[0] * rot.m128_f32[0];

			////pitch
			//float t2 = 2.0f * (rot.m128_f32[0] * rot.m128_f32[3] - rot.m128_f32[2] * rot.m128_f32[1]);

			////yaw
			//float t0 = 2.0f * (rot.m128_f32[2] * rot.m128_f32[0] + rot.m128_f32[1] * rot.m128_f32[3]); // wz , xy
			//float t1 = 1.0f - 2.0f * (q2sqr + rot.m128_f32[1] * rot.m128_f32[1]); // yy , zz

			////roll
			//float t3 = 2.0f * (rot.m128_f32[2] * rot.m128_f32[3] + rot.m128_f32[0] * rot.m128_f32[1]); //w*x , y*z
			//float t4 = 1.0f - 2.0f * (rot.m128_f32[2] * rot.m128_f32[2] + q2sqr); //x*x y*y

			//t2 = t2 > 1.0f ? 1.0f : t2;
			//t2 = t2 < -1.0f ? -1.0f : t2;

			//float pitch = asinf(t2);

			//float yaw = atan2f(t0, t1);

			//float roll = atan2f(t3, t4);

			pTransform->SetScale({ scale.x, scale.y, scale.z });
			pTransform->SetPosition({ pos.x, pos.y, pos.z });
			pTransform->SetRotation({ rot.x, rot.y, rot.z });
		}

		//메쉬설정
		switch (nowModelInfo.m_ModelType)
		{
		case ModelInfo::ModelMeshType::SKINNING:
			{
				//auto tempMesh = tempObj->AddComponent<Mesh>();

			}
			break;
		case ModelInfo::ModelMeshType::STATIC:
			{
				auto tempMesh = tempObj->AddComponent<Mesh>();

				MeshInfo tempMeshInfo;

				tempMeshInfo.m_MeshId = meshid;
				tempMeshInfo.m_subMeshId = submeshId;

				//NoneMaterial, NoneTex, Tex, TexNormal, Normal

				auto find = m_ModelMaterialInfo.find(nowModelInfo.m_MateriaGUID);
				if (find == m_ModelMaterialInfo.end())
				{
					tempMeshInfo.m_PassName = L"NoneTexStatic";
					tempMeshInfo.m_TextureCnt = 0;
				}
				else
				{
					tempMeshInfo.m_TextureCnt = find->second.m_TextureCnt;
					for (int i = 0; i < tempMeshInfo.m_TextureCnt; i++)
					{
						tempMeshInfo.m_TextureID[i] = find->second.m_Texture[i];
					}

					if (find->second.m_UsingAlbedo || find->second.m_UsingNormalMap)
					{
						tempMeshInfo.m_PassName = L"TexNormalStatic";
					}
					else if (find->second.m_UsingAlbedo)
					{
						tempMeshInfo.m_PassName = L"TexStatic";
					}


				}

				tempMesh->SetMeshInfo(tempMeshInfo);
			}
			break;
		case ModelInfo::ModelMeshType::NONMESH:
		default:
			break;
		}
	}
	return;
}

void ModelLoader::Finalize()
{
	m_ASEParser->Finalize();
	m_ASEParser.reset();
}
