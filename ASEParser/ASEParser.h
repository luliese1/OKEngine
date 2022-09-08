// �ļ� ���ο��� ���� ��������
#include "ASEFile.h"
#include <vector>
#include <map>

namespace ASEData
{
	struct BaseObjectInfo;
	struct ASEMaterial;
	struct Animation;
	struct MaterialMap;
}

enum ObjectType
{
	ObjectType_ObjectRoot = 0,
	ObjectType_Geomobject = 1,
	ObjectType_Shape = 2,
	ObjectType_Camera = 3,
	ObjectType_Light = 4,
	ObjectType_HelperObject = 5,
	ObjectType_MarkerObject = 6,
	ObjectType_SkinnedMesh = 7,
	ObjectType_Bone = 8,
	ObjectType_CMeshPlane = 9,
	ObjectType_CBullet_Trace = 10,
	ObjectType_BasicSphere = 11,
	ObjectType_Missile = 12,
	ObjectType_MeshBillboard = 13,
	ObjectType_PointSprite = 14,			// CMesh���..
	ObjectType_Material = 15,
	ObjectType_None = 16,
};

// �ļ� ���ο��� Ư�� ��带 ������ �۵��Ѵ�. (�ļ��� ��ͱ��� ����)
enum class Parsingmode
{
	None,
	Geomobject,
	Animation,
	Shape,
	Camera,
	Light,
	HelperObject,
	Skin_InitTM,
	Bone_List,
	Material,
	End,
};

///�������� ������ ����ؼ� �ļ��� ��������.
class ASEParser
{
public:
	ASEParser();
	~ASEParser();

	/// <summary>
	/// ������ �аڴ�.
	/// </summary>
	bool Load(LPSTR filepath, size_t mesh_metaData);
	bool ConvertOptimize();
	void Finalize();
	/// <summary>
	/// ������ �°� �����͸� �����Ѵ�?
	/// </summary>
	/// 

	std::vector<ASEData::BaseObjectInfo*>& GetObject(size_t meshid);
	std::vector<ASEData::ASEMaterial*>& GetMaterial(size_t meshid);
	std::vector<ASEData::Animation*>& GetAnimation(size_t meshid);

private:
	ObjectType m_ParsingMode;

	ASE::CASELexer m_Lexer;

	char m_TokenString[256];
	size_t m_mesh_id;

	bool m_IsNegativeScale;

	int m_MeshMappingCount = 0;
	//���̽� ����� ������ �� �����̴�..
	int m_tempFaceNormal;
	// ���ؽ� �븻�� ����.
	int m_vertexNormalCount;

	//��Ű�׵�����
	int m_tempBoneCount;
	int m_tempWeightCount;

	std::map<size_t, std::vector<ASEData::BaseObjectInfo*>> m_ObjectInfos;
	ASEData::BaseObjectInfo* m_TargetInfo;

	int m_MaterialCnt;

	std::map<size_t, std::vector<ASEData::ASEMaterial*>> m_Materials;
	ASEData::ASEMaterial* m_TargetMaterial;
	ASEData::MaterialMap* m_TargetMaterialMap;

	std::map<size_t, std::vector<ASEData::Animation*>> m_Animations;
	ASEData::Animation* m_TargetAnimation;

	//�ϴ� �̰͸� ����.
	void ParseASE();

	/// �Ľ��� ������Ʈ? (������ ����..)
	/// ������Ʈ �Ľ��ϴ� ����� �ٸ��� ����� �Ѵ�.
	void ClassifyParseMode(LONG nowToken);

	bool ParseBaseObject(LONG nowToken);
	void ParseGeomObject(LONG nowToken);
	//void ParseBone(LONG nowToken); //���� GEOMOBJECT�� �պ�
	void ParseHelperObject(LONG nowToken);
	void ParseMaterial(LONG nowToken);
	void ParseShapeObject(LONG nowToken);

	//�Լ������͸� �Ẹ��.

	/// <summary>
	/// �Ľ������
	/// </summary>
	int ParseTokenToInt();
	float ParseTokenToFloat();
	std::string ParseTokenToString();
	Vector3 ParseTokenToVector3();
};

////
//template <typename T>
//T* CastInfoType(BaseObjectInfo* obj)
//{
//	T* castedData = dynamic_cast<T*>(obj);
//	return castedData;
//};