// 파서 내부에서 쓰는 데이터형
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
	ObjectType_PointSprite = 14,			// CMesh상속..
	ObjectType_Material = 15,
	ObjectType_None = 16,
};

// 파서 내부에서 특정 모드를 가지고 작동한다. (파서의 재귀구조 때문)
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

///교수님의 렉서를 사용해서 파서를 돌려보자.
class ASEParser
{
public:
	ASEParser();
	~ASEParser();

	/// <summary>
	/// 파일을 읽겠다.
	/// </summary>
	bool Load(LPSTR filepath, size_t mesh_metaData);
	bool ConvertOptimize();
	void Finalize();
	/// <summary>
	/// 엔진에 맞게 데이터를 제공한다?
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
	//페이스 노멀을 넣을때 쓸 변수이다..
	int m_tempFaceNormal;
	// 버텍스 노말은 세개.
	int m_vertexNormalCount;

	//스키닝데이터
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

	//일단 이것만 하자.
	void ParseASE();

	/// 파싱할 오브젝트? (종류에 따라서..)
	/// 오브젝트 파싱하는 방법을 다르게 해줘야 한다.
	void ClassifyParseMode(LONG nowToken);

	bool ParseBaseObject(LONG nowToken);
	void ParseGeomObject(LONG nowToken);
	//void ParseBone(LONG nowToken); //본은 GEOMOBJECT에 합병
	void ParseHelperObject(LONG nowToken);
	void ParseMaterial(LONG nowToken);
	void ParseShapeObject(LONG nowToken);

	//함수포인터를 써보자.

	/// <summary>
	/// 파싱하즈아
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