#include "Parserpch.h"
#include "ASEParser.h"
#include "ASEData.h"
#include "ASEFile.h"

using namespace ASEData;

ASEParser::ASEParser() : m_TokenString(), m_tempFaceNormal(0)
{
	m_MaterialCnt = 0;
	m_ParsingMode = ObjectType_None;
	m_TargetInfo = nullptr;
	m_TargetAnimation = nullptr;
	m_TargetMaterial = nullptr;
	m_mesh_id = 0;
	m_vertexNormalCount = 0;
	m_IsNegativeScale = false;

	m_tempBoneCount = 0;
	m_tempWeightCount = 0;
	m_TargetMaterialMap = nullptr;
}

ASEParser::~ASEParser()
{
	for (auto& oi : m_ObjectInfos)
	{
		for (auto& infos : oi.second)
		{
			delete(infos);
		}
	}

	for (auto& mat : m_Materials)
	{
		for (auto& infos : mat.second)
		{
			delete(infos);
		}
	}

	for (auto& anim : m_Animations)
	{
		for (auto& infos : anim.second)
		{
			delete(infos);
		}
	}

}

bool ASEParser::Load(LPSTR filepath, size_t mesh_id)
{
	if (!m_Lexer.Open(filepath))
	{
		TRACE(" 파일 오픈 실패");
		return false;
	}

	m_mesh_id = mesh_id;
	m_ObjectInfos.insert({ m_mesh_id, std::vector<ASEData::BaseObjectInfo*>() });
	m_Animations.insert({ m_mesh_id, std::vector<ASEData::Animation*>() });

	ParseASE();

	return true;
}

bool ASEParser::ConvertOptimize()
{
	for (auto& IterInfos : m_ObjectInfos.find(m_mesh_id)->second)
	{
		//GeomObject 파싱
		if (IterInfos->m_type == ObjectType::ObjectType_Geomobject)
		{
			ASEData::GeomObject* castedInfo = static_cast<ASEData::GeomObject*>(IterInfos);

			//if (castedInfo->m_IsthereUV)
			//{
			//	for (auto& tangent : castedInfo->m_mesh_tangent)
			//	{
			//		tangent.m_bitangent /= tangent.m_count;
			//	}
			//}


			if (castedInfo->m_IsSkinningMesh)
			{
				//중복된 버텍스들을 체크할 것. 
				vector<vector<int>> _VertexInfos;
				int vertexNum = castedInfo->m_mesh_numvertex;

				//일단 베이스 크기는 버텍스의 크기
				_VertexInfos.resize(vertexNum);

				auto& OptimizeVertices = castedInfo->m_optskinned_vertex;
				auto& OptimizeIndices = castedInfo->m_opt_index;

				int faceNum = castedInfo->m_mesh_numfaces;

				OptimizeVertices.resize(vertexNum);
				OptimizeIndices.resize(faceNum);

				for (int faceIndex = 0; faceIndex < faceNum; faceIndex++)
				{
					for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
					{
						int& vindex = castedInfo->m_mesh_face.at(faceIndex).m_vertexindex[vertexIndex];
						//버텍스 pos에 대한 인덱스
						int& tvindex = castedInfo->m_mesh_tvfaces.at(faceIndex).m_vertexindex[vertexIndex];
						//버텍스 tex에 대한 인덱스
						auto& vertexNormal = castedInfo->m_mesh_facenormal.at(faceIndex).m_vertexNormal[vertexIndex];
						//버텍스 Normal 바로 가져옴.

						//데이터를 일단 temp에 저장한다.
						ASEData::OptimizedSkinnedVertex vertexTemp;

						vertexTemp.m_pos = castedInfo->m_mesh_vertex.at(vindex).m_pos;
						vertexTemp.m_normal = vertexNormal;

						auto& wvt = castedInfo->m_WVertexs.at(vindex);

						for (int i = 0; i < wvt.m_bone_blending_weight.size(); i++)
						{
							vertexTemp.m_weight[i] = wvt.m_bone_blending_weight.at(i).m_bone_weight;
							vertexTemp.m_boneindices[i] = wvt.m_bone_blending_weight.at(i).m_bone_number;
						}

						float* puv;
						float* puv2;

						if (castedInfo->m_mesh_numtvertex != 0)
						{
							puv = castedInfo->m_mesh_tvertex.at(tvindex).m_uvPos;
							puv2 = (castedInfo->m_mesh_tvertex.at(tvindex).m_uvPos + 1);

							vertexTemp.u = *(puv + 0);
							vertexTemp.v = puv[1];
						}
						// *(puv + 2); w는 아직 안쓰니까... 패스해둠.

						//배열에 삽입 준비를 하자.
						// 
						//이미 복제된 버텍스인지 검증한다.
						int duplicatedVetexSize = _VertexInfos.at(vindex).size();

						//어라.. 0이네? 새로운 버텍스임! 그대로 박아주자.
						if (duplicatedVetexSize == 0)
						{
							OptimizeVertices.at(vindex) = vertexTemp;
							OptimizeIndices.at(faceIndex).index[vertexIndex] = vindex;

							_VertexInfos.at(vindex).push_back(vindex);
						}
						else
						{
							bool IsThereSameVertex = false;
							int newVindex = 0;

							//어라.. 뭔가가 있나보다.. 검증해보자
							for (auto& duplicatedvertexindex : _VertexInfos.at(vindex))
							{
								auto& duplicatedvertex = OptimizeVertices.at(duplicatedvertexindex);

								//텍스처 uv를 비교해요
								float dupliu = duplicatedvertex.u;
								float dupliv = duplicatedvertex.v;
								if (!(dupliu == vertexTemp.u && dupliv == vertexTemp.v)) // 두개가 같지 않으면...
								{
									//넘겨.. 같은 버텍스가 아니네
									continue;
								}

								auto& duplinormal = duplicatedvertex.m_normal;
								//노말을 비교해요
								if (!(duplinormal.x == vertexTemp.m_normal.x && duplinormal.y == vertexTemp.m_normal.y && duplinormal.z == vertexTemp.m_normal.z)) // 노말이 같지않으면...
								{
									//넘겨.. 같은 버텍스가 아니네
									continue;
								}

								//둘다 같으면?.. 넌 똑같은 놈이 있음! 인덱스 번호를 수정해주자!
								IsThereSameVertex = true;
								//auto t1 = &duplicatedvertex;
								//auto t2 = &OptimizeVertices.front();
								// t3 = t1 - t2;

								//int e = sizeof(ASEData::OptimizedVertex);

								newVindex = duplicatedvertexindex;
								break;
							}

							if (IsThereSameVertex == false)
							{
								//넌 새로운놈이다.. 새로 넣어주고 인덱스 번호를 계산해, 수정해줘.
								OptimizeVertices.push_back(vertexTemp);
								newVindex = OptimizeVertices.size() - 1;

								_VertexInfos.at(vindex).push_back(newVindex);
							}

							//같은놈이 있음. vertexIndex를 뽑아왔으니 적용만이 남았다.
							OptimizeIndices.at(faceIndex).index[vertexIndex] = newVindex;
						}
					}
				}
			}
			else
			{
				//중복된 버텍스들을 체크할 것. 
				vector<vector<int>> _VertexInfos;
				int vertexNum = castedInfo->m_mesh_numvertex;

				//일단 베이스 크기는 버텍스의 크기
				_VertexInfos.resize(vertexNum);

				auto& OptimizeVertices = castedInfo->m_opt_vertex;
				auto& OptimizeIndices = castedInfo->m_opt_index;

				int faceNum = castedInfo->m_mesh_numfaces;

				OptimizeVertices.resize(vertexNum);
				OptimizeIndices.resize(faceNum);

				for (int faceIndex = 0; faceIndex < faceNum; faceIndex++)
				{
					for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
					{
						int& vindex = castedInfo->m_mesh_face.at(faceIndex).m_vertexindex[vertexIndex];
						//버텍스 pos에 대한 인덱스
						int& tvindex = castedInfo->m_mesh_tvfaces.at(faceIndex).m_vertexindex[vertexIndex];
						//버텍스 tex에 대한 인덱스
						auto& vertexNormal = castedInfo->m_mesh_facenormal.at(faceIndex).m_vertexNormal[vertexIndex];
						//버텍스 Normal 바로 가져옴.

						//데이터를 일단 temp에 저장한다.
						ASEData::OptimizedVertex vertexTemp;

						vertexTemp.m_pos = castedInfo->m_mesh_vertex.at(vindex).m_pos;


						//그람슈미트 가즈아

						vertexTemp.m_normal = vertexNormal;

						auto& tan = castedInfo->m_mesh_tangent.at(vindex).m_tangent;

						vertexTemp.m_tangent = tan - (vertexNormal * tan.Dot(vertexNormal));

						float* puv;

						if (castedInfo->m_mesh_numtvertex != 0)
						{
							puv = castedInfo->m_mesh_tvertex.at(tvindex).m_uvPos;

							vertexTemp.u = puv[0];
							vertexTemp.v = puv[1];
						}
						// *(puv + 2); w는 아직 안쓰니까... 패스해둠.

						//배열에 삽입 준비를 하자.
						// 
						//이미 복제된 버텍스인지 검증한다.
						int duplicatedVetexSize = _VertexInfos.at(vindex).size();

						//어라.. 0이네? 새로운 버텍스임! 그대로 박아주자.
						if (duplicatedVetexSize == 0)
						{
							OptimizeVertices.at(vindex) = vertexTemp;
							OptimizeIndices.at(faceIndex).index[vertexIndex] = vindex;

							_VertexInfos.at(vindex).push_back(vindex);
						}
						else
						{
							bool IsThereSameVertex = false;
							int newVindex = 0;

							//어라.. 뭔가가 있나보다.. 검증해보자
							for (auto& duplicatedvertexindex : _VertexInfos.at(vindex))
							{
								auto& duplicatedvertex = OptimizeVertices.at(duplicatedvertexindex);

								//텍스처 uv를 비교해요
								float dupliu = duplicatedvertex.u;
								float dupliv = duplicatedvertex.v;
								if (!(dupliu == vertexTemp.u && dupliv == vertexTemp.v)) // 두개가 같지 않으면...
								{
									//넘겨.. 같은 버텍스가 아니네
									continue;
								}

								auto& duplinormal = duplicatedvertex.m_normal;
								//노말을 비교해요
								if (!(duplinormal.x == vertexTemp.m_normal.x && duplinormal.y == vertexTemp.m_normal.y && duplinormal.z == vertexTemp.m_normal.z)) // 노말이 같지않으면...
								{
									//넘겨.. 같은 버텍스가 아니네
									continue;
								}

								//둘다 같으면?.. 넌 똑같은 놈이 있음! 인덱스 번호를 수정해주자!
								IsThereSameVertex = true;
								//auto t1 = &duplicatedvertex;
								//auto t2 = &OptimizeVertices.front();
								// t3 = t1 - t2;

								//int e = sizeof(ASEData::OptimizedVertex);

								newVindex = duplicatedvertexindex;
								break;
							}

							if (IsThereSameVertex == false)
							{
								//넌 새로운놈이다.. 새로 넣어주고 인덱스 번호를 계산해, 수정해줘.
								OptimizeVertices.push_back(vertexTemp);
								newVindex = OptimizeVertices.size() - 1;

								_VertexInfos.at(vindex).push_back(newVindex);
							}

							//같은놈이 있음. vertexIndex를 뽑아왔으니 적용만이 남았다.
							OptimizeIndices.at(faceIndex).index[vertexIndex] = newVindex;
						}
					}
				}
			}
		}
	}

	return false;
}

void ASEParser::Finalize()
{
	for (auto objectInfo : m_ObjectInfos)
	{
		for (auto baseObjectInfo : objectInfo.second)
		{
			delete(baseObjectInfo);
		}
	}
	m_ObjectInfos.clear();

	for (auto material : m_Materials)
	{
		for (auto ASEMaterial : material.second)
		{
			delete(ASEMaterial);
		}
	}
	m_Materials.clear();

	for (auto anims : m_Animations)
	{
		for (auto anim : anims.second)
		{
			delete(anim);
		}
	}
	m_Animations.clear();
}

std::vector<ASEData::BaseObjectInfo*>& ASEParser::GetObject(size_t meshid)
{
	auto info = m_ObjectInfos.find(meshid);

	return info->second;
}

std::vector<ASEData::ASEMaterial*>& ASEParser::GetMaterial(size_t meshid)
{
	return m_Materials.find(meshid)->second;
}

std::vector<ASEData::Animation*>& ASEParser::GetAnimation(size_t meshid)
{
	return m_Animations.find(meshid)->second;
}

void ASEParser::ParseASE()
{
	LONG nowtoken;

	m_MaterialCnt = 0;
	m_ParsingMode = ObjectType_None;
	m_TargetInfo = nullptr;
	m_TargetMaterial = nullptr;
	m_vertexNormalCount = 0;

	//Matrix TM1, TM2;
	//Quaternion tempQT;
	//Quaternion prevQT;
	//Quaternion resultQT;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_Lexer.GetToken(m_TokenString), m_Lexer.GetNowLoaded() != 0)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.
		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:
			{
			}
			break;
		case TOKEND_END:
			{
				// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

				//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			}
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;
			break;
		default:
			ClassifyParseMode(nowtoken);
			break;
		}
	}

	m_ObjectInfos.at(m_mesh_id).push_back(m_TargetInfo);
	m_TargetInfo = nullptr;

	m_ParsingMode = ObjectType::ObjectType_None;

	ConvertOptimize();
}

void ASEParser::ClassifyParseMode(LONG nowToken)
{

	switch (nowToken)
	{
	case TOKENR_3DSMAX_ASCIIEXPORT:
		{
		}
		break;
	case TOKENR_GEOMOBJECT:
		{
			if (m_TargetInfo != nullptr)
			{
				m_ObjectInfos.find(m_mesh_id)->second.push_back(m_TargetInfo);
			}

			m_TargetInfo = new ASEData::GeomObject;
			m_TargetInfo->m_type = ObjectType_Geomobject;
			m_ParsingMode = ObjectType_Geomobject;
			m_IsNegativeScale = false;
		}
		break;
	case TOKENR_HELPEROBJECT:
		{
			if (m_TargetInfo != nullptr)
			{
				m_ObjectInfos.find(m_mesh_id)->second.push_back(m_TargetInfo);
			}

			m_TargetInfo = new ASEData::HelperObject;
			m_TargetInfo->m_type = ObjectType_HelperObject;
			m_ParsingMode = ObjectType_HelperObject;
			m_IsNegativeScale = false;

		}
		break;
	case TOKENR_MATERIAL_LIST:
		{
			m_ParsingMode = ObjectType_Material;
			m_Materials.insert({ m_mesh_id, vector<ASEData::ASEMaterial*>() });
			m_IsNegativeScale = false;

		}
		break;
	case TOKENR_SHAPEOBJECT:
		{
			if (m_TargetInfo != nullptr)
			{
				m_ObjectInfos.find(m_mesh_id)->second.push_back(m_TargetInfo);
			}

			m_TargetInfo = new ASEData::ShapeObject;
			m_TargetInfo->m_type = ObjectType_Shape;
			m_ParsingMode = ObjectType_Shape;
			m_IsNegativeScale = false;

		}
		break;
	default:
		{
			//default로 들어온 토큰들은 오브젝트들을 정의하는 녀석들이 아니기 떄문에 모드에 따라 파싱을 정의해준다.
			if (!ParseBaseObject(nowToken))
			{
				switch (m_ParsingMode)
				{
				case ObjectType_Geomobject:
					{
						ParseGeomObject(nowToken);
					}
					break;
				case ObjectType_HelperObject:
					{
						ParseHelperObject(nowToken);
					}
				case ObjectType_Material:
					{
						ParseMaterial(nowToken);
					}
					break;
				case ObjectType_Bone:
					{
						ParseGeomObject(nowToken);
					}
					break;
				case ObjectType_Shape:
					{
						ParseMaterial(nowToken);
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	}
}

bool ASEParser::ParseBaseObject(LONG nowToken)
{
	bool IsParsed = true;

	switch (nowToken)
	{
	case TOKENR_NODE_NAME:
		{

			m_TargetInfo->m_nodename = ParseTokenToString();
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
		}
		break;
	case TOKENR_NODE_PARENT:
		{
			m_TargetInfo->m_nodeparent = ParseTokenToString();
			// 현 노드의 부모 노드의 정보.
			// 일단 입력을 하고, 나중에 정리하자.
		}
		break;
		/// NODE_TM
	case TOKENR_NODE_TM:
		{

			//m_parsingmode	=	eGeomobject;
			// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
			// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

		}
		break;
	case TOKENR_INHERIT_POS:
		{
			m_TargetInfo->m_inherit_pos = ParseTokenToVector3();

			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
		}
		break;
	case TOKENR_INHERIT_ROT:
		{
			m_TargetInfo->m_inherit_rot = ParseTokenToVector3();
		}
		break;
	case TOKENR_INHERIT_SCL:
		{
			m_TargetInfo->m_inherit_scl = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROW0:
		{
			m_TargetInfo->m_NodeTm._11 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._13 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._12 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._14 = 0.f;

			//m_TargetInfo->m_tm_row0 = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROW1:
		{
			m_TargetInfo->m_NodeTm._31 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._33 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._32 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._24 = 0.f;

			//m_TargetInfo->m_tm_row1 = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROW2:
		{
			m_TargetInfo->m_NodeTm._21 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._23 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._22 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._34 = 0.f;

			//m_TargetInfo->m_tm_row2 = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROW3:
		{
			m_TargetInfo->m_NodeTm._41 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._43 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._42 = ParseTokenToFloat();
			m_TargetInfo->m_NodeTm._44 = 1.f;

			XMVECTOR det = XMMatrixDeterminant(m_TargetInfo->m_NodeTm);

			if (det.m128_f32[0] < 0)
			{
				//for (int i = 0; i < 3; i++)
				//{
				//	for (int j = 0; j < 3; j++)
				//	{
				//		float* p = reinterpret_cast<float*>(&m_TargetInfo->m_NodeTm);

				//		p[i + j] *= -1;
				//	}
				//}

				XMVECTOR _scale;

				XMVECTOR _rotationQuat;

				XMVECTOR _translation;

				XMMatrixDecompose(&_scale, &_rotationQuat, &_translation, m_TargetInfo->m_NodeTm);

				_scale *= -1;
				m_TargetInfo->m_NodeTm = XMMatrixScalingFromVector(_scale) * XMMatrixRotationQuaternion(_rotationQuat)
					* XMMatrixTranslation(_translation.m128_f32[0], _translation.m128_f32[1], _translation.m128_f32[2]);

				m_IsNegativeScale = true;
			}

			m_TargetInfo->m_InverseNodeTm = m_TargetInfo->m_NodeTm.Invert();


			break;
		}
	case TOKENR_TM_POS:
		{
			m_TargetInfo->m_tm_pos = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROTAXIS:
		{
			m_TargetInfo->m_tm_rotaxis = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_ROTANGLE:
		{
			m_TargetInfo->m_tm_rotangle = ParseTokenToFloat();
		}
		break;
	case TOKENR_TM_SCALE:
		{
			m_TargetInfo->m_tm_scale = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_SCALEAXIS:
		{
			m_TargetInfo->m_tm_scaleaxis = ParseTokenToVector3();
		}
		break;
	case TOKENR_TM_SCALEAXISANG:
		{
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
			m_TargetInfo->m_tm_scaleaxisang = ParseTokenToFloat();
		}
		break;
	case TOKENR_TM_ANIMATION:
		{
			m_TargetInfo->m_isAnimated = true;
			//애니메이션을 파싱해라
			m_TargetAnimation = new ASEData::Animation;

			m_Animations.find(m_mesh_id)->second.push_back(m_TargetAnimation);
			m_TargetAnimation->m_nodeIndex = m_ObjectInfos.find(m_mesh_id)->second.size();
		}
		break;
	case TOKENR_CONTROL_POS_SAMPLE:
		{
			ASEData::Animation_Control_Pos tempPos;
			tempPos.m_time = ParseTokenToInt();
			tempPos.m_pos = ParseTokenToVector3();

			m_TargetAnimation->m_position.push_back(tempPos);
		}
		break;
	case TOKENR_CONTROL_ROT_SAMPLE:
		{
			ASEData::Animation_Control_Rot tempRot;
			tempRot.m_time = ParseTokenToInt();
			tempRot.m_rot = ParseTokenToVector3();
			tempRot.m_angle = ParseTokenToFloat();

			XMVECTOR rotQT = XMQuaternionRotationAxis(tempRot.m_rot, tempRot.m_angle);

			if (m_TargetAnimation->m_rotation.size() > 0)
			{
				//이거.. 순서 중요하다... back 먼저.. 순서대로 하자 
				//쿼터니언은 덧셈만 교환법칙이 성립하고 곱셈은 성립하지 않는다.
				tempRot.m_rotQT_accumulation = XMQuaternionMultiply(m_TargetAnimation->m_rotation.back().m_rotQT_accumulation, rotQT);
			}
			else
			{
				tempRot.m_rotQT_accumulation = rotQT;
			}

			m_TargetAnimation->m_rotation.push_back(tempRot);

		}
		break;
	case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			ASEData::Animation_Control_Scl tempScl;
			tempScl.m_time = ParseTokenToInt();
			tempScl.m_scale = ParseTokenToVector3();

			m_TargetAnimation->m_scale.push_back(tempScl);
		}
		break;
	default:
		IsParsed = false;
		break;
	}

	return IsParsed;
}

void ASEParser::ParseGeomObject(LONG nowToken)
{
	ASEData::GeomObject* TargetInfo = static_cast<ASEData::GeomObject*>(m_TargetInfo);

	switch (nowToken)
	{
		///----------------------------------------------
		/// MESH
		///----------------------------------------------

	case TOKENR_MESH:
		{
			//메쉬파트 시작
		}
		break;
	case TOKENR_TIMEVALUE:
		{

			TargetInfo->m_timevalue = ParseTokenToFloat();
		}
		break;
	case TOKENR_MESH_NUMVERTEX:
		{
			TargetInfo->m_mesh_numvertex = ParseTokenToInt();
			TargetInfo->m_mesh_vertex.resize(TargetInfo->m_mesh_numvertex);
			TargetInfo->m_mesh_tangent.resize(TargetInfo->m_mesh_numvertex);
		}
		break;
	case TOKENR_MESH_NUMFACES:
		{
			TargetInfo->m_mesh_numfaces = ParseTokenToInt();
			TargetInfo->m_mesh_face.resize(TargetInfo->m_mesh_numfaces);
			TargetInfo->m_mesh_tvfaces.resize(TargetInfo->m_mesh_numfaces);
			TargetInfo->m_mesh_facenormal.resize(TargetInfo->m_mesh_numfaces);
		}
		break;
		/// MESH_VERTEX_LIST
	case TOKENR_MESH_VERTEX_LIST:
		{
			//
			// 버텍스의 값들을 집어넣어야 하는데
			// 이미 벡터로 선언이 돼 있으므로 그냥 넣으면 된다.
		}
		break;
	case TOKENR_MESH_VERTEX:
		{
			// 데이터 입력
			int index = ParseTokenToInt();
			auto& pos = TargetInfo->m_mesh_vertex.at(index).m_pos;

			pos.x = ParseTokenToFloat();
			pos.z = ParseTokenToFloat();
			pos.y = ParseTokenToFloat();

			pos = XMVector3Transform(pos, TargetInfo->m_InverseNodeTm);

			if (TargetInfo->m_BoundingBox_Min.x > pos.x)
			{
				TargetInfo->m_BoundingBox_Min.x = pos.x;
			}
			if (TargetInfo->m_BoundingBox_Min.y > pos.y)
			{
				TargetInfo->m_BoundingBox_Min.y = pos.y;
			}
			if (TargetInfo->m_BoundingBox_Min.z > pos.z)
			{
				TargetInfo->m_BoundingBox_Min.z = pos.z;
			}

			if (TargetInfo->m_BoundingBox_Max.x < pos.x)
			{
				TargetInfo->m_BoundingBox_Max.x = pos.x;
			}
			if (TargetInfo->m_BoundingBox_Max.y < pos.y)
			{
				TargetInfo->m_BoundingBox_Max.y = pos.y;
			}
			if (TargetInfo->m_BoundingBox_Max.z < pos.z)
			{
				TargetInfo->m_BoundingBox_Max.z = pos.z;
			}


		}
	case TOKENR_MESH_FACE_LIST:
		{

		}
		break;
	case TOKENR_MESH_FACE:
		{
			int index = ParseTokenToInt();

			int vertexIndex[3];
			ParseTokenToInt();
			vertexIndex[0] = ParseTokenToInt();
			ParseTokenToInt();
			vertexIndex[1] = ParseTokenToInt();
			ParseTokenToInt();
			vertexIndex[2] = ParseTokenToInt();


			//오른손 좌표계니까... 왼손좌표계로 바꾸면서.. ABC -> ACB
			TargetInfo->m_mesh_face.at(index).m_vertexindex[0] = vertexIndex[0];
			TargetInfo->m_mesh_face.at(index).m_vertexindex[1] = vertexIndex[2];
			TargetInfo->m_mesh_face.at(index).m_vertexindex[2] = vertexIndex[1];

			// Face의 번호인데...

			// A:를 읽고
			// B:
			// C:

			/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
			// 벡터에 넣어준다.
		}
		break;
	case TOKENR_MESH_NUMTVERTEX:
		{
			if (m_MeshMappingCount != 0)
			{
				break;
			}
			TargetInfo->m_mesh_numtvertex = ParseTokenToInt();
			TargetInfo->m_mesh_tvertex.resize(TargetInfo->m_mesh_numtvertex);
		}
		break;
		/// MESH_TVERTLIST
	case TOKENR_MESH_MAPPINGCHANNEL:
		{
			m_MeshMappingCount = ParseTokenToInt();
		}
		break;
	case TOKENR_MESH_TVERTLIST:
		{
			TargetInfo->m_IsthereUV = true;
		}
		break;
	case TOKENR_MESH_TVERT:
		{
			if (m_MeshMappingCount != 0)
			{
				break;
			}
			int index = ParseTokenToInt();

			float vertexIndex[3];
			vertexIndex[0] = ParseTokenToFloat(); //u
			vertexIndex[1] = ParseTokenToFloat(); //v
			vertexIndex[2] = ParseTokenToFloat(); //w

			auto& tvert = TargetInfo->m_mesh_tvertex.at(index);
			tvert.m_uvPos[0] = vertexIndex[0];
			tvert.m_uvPos[1] = 1.0f - vertexIndex[1]; // 1-v를 해줘야하나?
			//이미지를 뒤집으면 1-를 안해줘도 됨. 예제이미지는 뒤집힌 이미지가 있으므로... 안뺴준다
			tvert.m_uvPos[2] = vertexIndex[2];

			// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
			// 새로운 TVertex를 만들어서 벡터에 넣는다
		}
		break;
	case TOKENR_MESH_TFACE:
		{
			int index = ParseTokenToInt();

			int tvertexIndex[3];
			tvertexIndex[0] = ParseTokenToInt();
			tvertexIndex[1] = ParseTokenToInt();
			tvertexIndex[2] = ParseTokenToInt();

			//오른손 좌표계니까... 왼손좌표계로 바꾸면서.. ABC -> ACB
			TargetInfo->m_mesh_tvfaces.at(index).m_vertexindex[0] = tvertexIndex[0];
			TargetInfo->m_mesh_tvfaces.at(index).m_vertexindex[1] = tvertexIndex[2];
			TargetInfo->m_mesh_tvfaces.at(index).m_vertexindex[2] = tvertexIndex[1];
		}
		break;
	case TOKENR_MESH_NORMALS:
		{
			TargetInfo->m_mesh_facenormal.resize(TargetInfo->m_mesh_numfaces);
		}
		break;

	case TOKENR_MESH_FACENORMAL:
		{
			m_tempFaceNormal = ParseTokenToInt();
			m_vertexNormalCount = 0;

			float faceNormal[3];
			if (m_IsNegativeScale)
			{
				faceNormal[0] = ParseTokenToFloat() * -1.0f; //x
				faceNormal[2] = ParseTokenToFloat() * -1.0f; //y
				faceNormal[1] = ParseTokenToFloat() * -1.0f; //z
			}
			else
			{
				faceNormal[0] = ParseTokenToFloat(); //x
				faceNormal[2] = ParseTokenToFloat(); //y
				faceNormal[1] = ParseTokenToFloat(); //z
			}

		}
		break;
	case TOKENR_MESH_VERTEXNORMAL:
		{
			//ACB
			int vnum;

			switch (m_vertexNormalCount)
			{
			case 0:
				{
					vnum = 0;
				}
				break;
			case 1:
				{
					vnum = 2;
				}
				break;
			case 2:
				{
					vnum = 1;
				}
				break;
			default:
				break;
			}

			auto& vertexNomal = TargetInfo->m_mesh_facenormal.at(m_tempFaceNormal).m_vertexNormal[vnum];

			TargetInfo->m_mesh_facenormal.at(m_tempFaceNormal).m_vertexIndex[vnum] = ParseTokenToInt();

			vertexNomal.x = ParseTokenToFloat();
			vertexNomal.z = ParseTokenToFloat();
			vertexNomal.y = ParseTokenToFloat();

			if (m_IsNegativeScale)
				vertexNomal *= -1.0f;

			m_vertexNormalCount++;
			//faceNomal.x = ;

			//여기서 탄젠트 스페이스를 계산하여 넣어줄 것이다.
			if (TargetInfo->m_IsthereUV)
			{
				if (m_vertexNormalCount == 3)
				{
					auto& vnums = TargetInfo->m_mesh_facenormal.at(m_tempFaceNormal);

					Vector3 worldvec1 = TargetInfo->m_mesh_vertex[vnums.m_vertexIndex[1]].m_pos - TargetInfo->m_mesh_vertex[vnums.m_vertexIndex[0]].m_pos;
					Vector3 worldvec2 = TargetInfo->m_mesh_vertex[vnums.m_vertexIndex[2]].m_pos - TargetInfo->m_mesh_vertex[vnums.m_vertexIndex[0]].m_pos;

					auto& uvInfo = TargetInfo->m_mesh_tvfaces.at(m_tempFaceNormal);

					float x1 = TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[1]].m_uvPos[0] - TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[0]].m_uvPos[0];
					float y1 = TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[1]].m_uvPos[1] - TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[0]].m_uvPos[1];

					float x2 = TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[2]].m_uvPos[0] - TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[0]].m_uvPos[0];
					float y2 = TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[2]].m_uvPos[1] - TargetInfo->m_mesh_tvertex[uvInfo.m_vertexindex[0]].m_uvPos[1];

					float r = (x1 * y2 - x2 * y1);

					if (r == 0.00000f)
						r = 0.000000001f;

					float determinant = 1.0f / (x1 * y2 - x2 * y1);



					Vector3 tangent;
					Vector3 bitangent;

					tangent.x = worldvec1.x * y2 - worldvec2.x * y1;
					tangent.y = worldvec1.y * y2 - worldvec2.y * y1;
					tangent.z = worldvec1.z * y2 - worldvec2.z * y1;

					bitangent.x = -worldvec1.x * x2 + worldvec2.x * x1;
					bitangent.y = -worldvec1.y * x2 + worldvec2.y * x1;
					bitangent.z = -worldvec1.z * x2 + worldvec2.z * x1;

					tangent *= determinant;
					bitangent *= determinant;

					for (int i = 0; i < 3; i++)
					{
						TargetInfo->m_mesh_tangent[vnums.m_vertexIndex[i]].m_tangent += tangent;
						TargetInfo->m_mesh_tangent[vnums.m_vertexIndex[i]].m_bitangent += bitangent;
					}
				}
			}
		}
		break;
		/// 스키닝준비
	case TOKENR_MESH_NUMBONE:
		{
			//이녀석이 geom에 있으면 스키닝 메쉬이다.
			TargetInfo->m_numbone = ParseTokenToInt();
			TargetInfo->m_IsSkinningMesh = true;
		}
		break;
	case TOKENR_BONE_LIST:
		{
			TargetInfo->m_BoneList.resize(TargetInfo->m_numbone);
		}
		break;
	case TOKENR_BONE:
		{
			m_tempBoneCount = ParseTokenToInt();
		}
		break;
	case TOKENR_BONE_NAME:
		{
			auto& TargetBone = TargetInfo->m_BoneList.at(m_tempBoneCount);
			TargetBone.m_bone_name = ParseTokenToString();
		}
		break;
	case TOKENR_MESH_WVERTEXS:
		{
			TargetInfo->m_WVertexs.resize(TargetInfo->m_mesh_numvertex);
		}
		break;
	case TOKENR_MESH_WEIGHT:
		{
			m_tempWeightCount = ParseTokenToInt();
		}
		break;
	case TOKENR_BONE_BLENGING_WEIGHT:
		{
			ASEData::Weight tempWeight;
			tempWeight.m_bone_number = ParseTokenToInt();
			tempWeight.m_bone_weight = ParseTokenToFloat();

			TargetInfo->m_WVertexs.at(m_tempWeightCount).m_bone_blending_weight.push_back(tempWeight);
		}
		break;
	case TOKENR_MATERIAL_REF:
		{
			TargetInfo->m_material_ref = ParseTokenToInt();
		}
		break;
	default:
		break;
	}

}
void ASEParser::ParseHelperObject(LONG nowToken)
{
}
void ASEParser::ParseMaterial(LONG nowToken)
{
	switch (nowToken)
	{
		//--------------------
			// MATERIAL_LIST
			//--------------------
			// 

	case TOKENR_MATERIAL_COUNT:
		{
			m_Materials.find(m_mesh_id)->second.resize(ParseTokenToInt());
		}
		break;
	case TOKENR_MATERIAL:
		{
			m_TargetMaterial = new ASEData::ASEMaterial;

			m_TargetMaterial->m_materialnumber = ParseTokenToInt();
			m_Materials.find(m_mesh_id)->second.at(m_TargetMaterial->m_materialnumber) = m_TargetMaterial;
		}
		break;
	case TOKENR_MATERIAL_AMBIENT:
		{
			m_TargetMaterial->m_material_ambient.x = ParseTokenToFloat();
			m_TargetMaterial->m_material_ambient.y = ParseTokenToFloat();
			m_TargetMaterial->m_material_ambient.z = ParseTokenToFloat();

		}
		break;
	case TOKENR_MATERIAL_DIFFUSE:
		{
			m_TargetMaterial->m_material_diffuse.x = ParseTokenToFloat();
			m_TargetMaterial->m_material_diffuse.y = ParseTokenToFloat();
			m_TargetMaterial->m_material_diffuse.z = ParseTokenToFloat();
		}
		break;
	case TOKENR_MATERIAL_SPECULAR:
		{
			m_TargetMaterial->m_material_specular.x = ParseTokenToFloat();
			m_TargetMaterial->m_material_specular.y = ParseTokenToFloat();
			m_TargetMaterial->m_material_specular.z = ParseTokenToFloat();
		}
		break;
	case TOKENR_MAP_DIFFUSE:
		{
			m_TargetMaterialMap = new ASEData::MaterialMap;
			m_TargetMaterial->m_map_diffuse = m_TargetMaterialMap;
		}
		break;
	case TOKENR_MAP_BUMP:
		{
			m_TargetMaterialMap = new ASEData::MaterialMap;
			m_TargetMaterial->m_map_bump = m_TargetMaterialMap;
		}
		break;
	case TOKENR_MAP_NAME:
		{
			m_TargetMaterialMap->m_map_name = ParseTokenToString();
		}
		break;
	case TOKENR_BITMAP:
		{
			m_TargetMaterialMap->m_bitmap = ParseTokenToString();
		}
		break;
	default:
		break;
	}


}

void ASEParser::ParseShapeObject(LONG nowToken)
{

}

/// <summary>
/// 토큰을 형변환한다.
/// </summary>
/// <returns></returns>
int ASEParser::ParseTokenToInt()
{
	LONG token;
	int result;

	token = m_Lexer.GetToken(m_TokenString);
	result = atoi(m_TokenString);

	return result;
}
float ASEParser::ParseTokenToFloat()
{
	LONG token;
	float result;

	token = m_Lexer.GetToken(m_TokenString);
	result = (float)atof(m_TokenString);

	return result;
}
std::string ASEParser::ParseTokenToString()
{
	LONG token;
	token = m_Lexer.GetToken(m_TokenString);

	return m_TokenString;
}
DirectX::SimpleMath::Vector3 ASEParser::ParseTokenToVector3()
{
	LONG token;
	Vector3 result;

	token = m_Lexer.GetToken(m_TokenString);
	result.x = (float)atof(m_TokenString);

	token = m_Lexer.GetToken(m_TokenString);
	result.z = (float)atof(m_TokenString);

	token = m_Lexer.GetToken(m_TokenString);
	result.y = (float)atof(m_TokenString);

	return result;
}
