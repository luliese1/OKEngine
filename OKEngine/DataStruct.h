#pragma once

struct StaticMesh
{
	Matrix World;
	Matrix WorldInvTranspose;
	Matrix WorldViewProjection;
	Matrix Material;
};
struct StaticMeshVertex
{
	Vector3 Pos;
	Vector3 Normal;
	Vector3 Tangent;
	Vector2 Tex0;
	Vector2 Tex1;
};

struct ModelInfo
{
	ModelInfo() {};
	enum class ModelMeshType
	{
		NONMESH,
		STATIC,
		SKINNING
	};

	std::wstring m_name;
	Matrix m_nodeTM;

	// -1�̸� �θ� ����
	// subMeshInfo�� ����Ǻ�.
	int m_parent;

	ModelMeshType m_ModelType;
	int m_SubMeshInfo;
	ObjectGUID m_MateriaGUID;
	int	m_MateriaRef;

};
struct ModelMaterialInfo
{
	std::wstring m_name;
	ObjectGUID m_GUID;

	Vector4 m_Ambient;
	Vector4 m_Diffuse;
	Vector4 m_Specular;
	Vector4 m_Reflect;

	bool m_UsingAlbedo;
	bool m_UsingNormalMap;

	//albedo...�븻��... ���� ����. ���⿡�� id�� �����Ѵ�. 
	ObjectGUID m_Texture[MAXIMUM_SHADER_RESOURCE_PER_MESH];
	std::wstring m_TeturePath[MAXIMUM_SHADER_RESOURCE_PER_MESH];

	UINT m_TextureCnt;

	bool m_LoadState = false;
	UINT m_referCount;
	double m_NoneUsingTime;

};
//�ļ����� �����͸� �ε��ϰ� ������ ������
struct ModelManageInfo
{
	enum class fileType
	{
		ase,
		fbx
	};

	fileType m_filetype;
	std::wstring m_path;
	std::wstring m_name;
	//�ؽ��� ���� �̾ƿ� GUID�Դϴ�..
	ObjectGUID m_GUID;

	//�������������� ��������� �ʿ��� ������ ������ ������.

	//�ʿ��Ѱ� �Ľ��� ���� �� ���� �޽��� ����1
	UINT m_ModelCount;
	//�Ľ��� ���� ������. 
	std::vector<ModelInfo> m_ModelInfo;

	bool m_LoadState;
	UINT m_referCount;
	double m_NoneUsingTime;

};

struct LightDataBase
{
	Matrix LightProjectionTM;
};

struct DirectionalLightInfo
{
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector4 Direction;
	Matrix LightProjectionTM;
};

struct PointLightInfo
{
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector4 PositionRange;
	Vector4 Attenuation;
	Matrix LightProjectionTM;
};

struct SpotLightInfo
{
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
	Vector4 PositionRange;
	Vector4 DirectionSpot;
	Vector4 Attenuation;
	Matrix LightProjectionTM;
};

struct CameraInfo
{
	Matrix m_ViewMatrix;
	Matrix m_ProjMatrix;
	Matrix m_ViewProjectionMatrix;
	Matrix m_ViewProjectionMatrixInverseTranspose;
};

