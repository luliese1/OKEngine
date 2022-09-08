/// <summary>
/// CParsingDataClass.h
/// 
/// Ver 1.0
/// ASE������ �Ľ������� ������ �����͵��� ����ü�� �������.
/// 
/// �ϴ� �� �� �о���̴� ���̰�, �������� ���������� ������ �Ѵ�.
/// D3DX���ξ, XM���ξ �������� ���ڴ�.
/// 
/// (���� �ּ�)
/// �˴� struct�� public���� �ߴµ� ������ �ƹ�����
/// ���� ���⿡�� ���� �����ؾ��ϱ� ����.
/// 
/// ��� �� �ڷᱸ���� �Ѱܹ޴� D3D�� ������������
/// ĸ��ȭ�� �Ǿ��ִ�. ��� ��ȯ �Լ����� �������� �� ���� �ִ�.
/// (�׷��� �ʴٸ� ������ GetSet�޼ҵ���� ������ �Ѵ�)
/// (���� �ּ�)
/// 
/// 2021.04.11 LeHideOffice
/// </summary>

#pragma once

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

namespace ASEData {

	///--------------------------------------------------
	/// Animation Data�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------

	struct Animation_Control_Pos
	{
		int m_time;
		Vector3	m_pos;
	};

	struct Animation_Control_Rot
	{
		int				m_time;
		Vector3		m_rot;
		float			m_angle;
		//������ ã�Ҵ�.
		XMVECTOR	m_rotQT_accumulation;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
	};

	struct Animation_Control_Scl
	{
		int				m_time;
		Vector3		m_scale;
		Vector3		m_scaleaxis;
		float			m_scaleaxisang;
		Quaternion	m_scalerotQT_accumulation;
	};


	///--------------------------------------------------
	/// �� Ŭ������ ������ Animation Ŭ����
	///--------------------------------------------------
	struct Animation
	{
		// �ִϸ��̼� �������� �̸�. ������ ���������̴�
		int					m_nodeIndex;
		vector<Animation_Control_Pos>		m_position;
		vector<Animation_Control_Rot>		m_rotation;
		vector<Animation_Control_Scl>		m_scale;

		int		m_ticksperFrame;	// �� �������� ƽ ũ��. �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�.
		int		m_minKeyframe;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
	};

	///--------------------------------------------------
	/// �� ���� VertexPos
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct VertexPos
	{
		VertexPos();
		//VertexPos(VertexPos&& other);


		// ��ǥ���� ��ġ��
		Vector3 m_pos;

	};
	///--------------------------------------------------
	/// �� ���� Texture - VertexPos
	///
	/// �̰��� Raw�������̸� ����ȭ�� �̷����� �ᱹ��
	/// COneVertex�� ���� �ȴ�.
	///--------------------------------------------------
	struct TVertex
	{
		TVertex();

		float m_uvPos[3]; // u, v, w
	};

	///--------------------------------------------------
	/// �� ���� Face
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///
	/// �Ľ� �ʱ⿡ Face�� ������ ������ ������
	/// �ߺ��Ǵ� VertexNormal��, �ؽ��� �ڵ����Ʈ ����
	/// ���� �� �ִ�. �װ��� ������� �Ѵ�.
	///--------------------------------------------------
	struct Face
	{
		// �� Face�� �̷�� ���ؽ��� �ε���
		int				m_vertexindex[3];
		//// Face�� Normal��
		//Vector3			m_normal;
		//// Vertex�� Normal��
		//Vector3			m_normalvertex[3];
		//// Texture Coordinate
		//int				m_TFace[3];

		// �ʱ�ȭ�� ���� ������.
		Face();
	};
	///--------------------------------------------------
	/// �� ���� FaceNormal
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///
	/// �Ľ� �ʱ⿡ Face�� ������ ������ ������
	/// �ߺ��Ǵ� VertexNormal��, �ؽ��� �ڵ����Ʈ ����
	/// ���� �� �ִ�. �װ��� ������� �Ѵ�.
	///--------------------------------------------------
	struct VetexNormal
	{
		//facenormal.. �������?
		Vector3				m_faceNormal;

		int m_vertexIndex[3];
		//vertexNormal ����� �ڷ��!
		Vector3				m_vertexNormal[3];

		// �ʱ�ȭ�� ���� ������.
		VetexNormal();
	};

	/// <summary>
	/// optimize data�� ������ �༮���̴�.
	/// </summary>
	struct OptimizedVertex
	{
		OptimizedVertex();

		// ��ǥ���� ��ġ��
		Vector3 m_pos;

		// �븻��
		Vector3 m_normal;
		// ź��Ʈ�� ����
		Vector3 m_tangent;

		// �ؽ��� ��ǥ
		float u, v;
		float u2, v2;

	};

	/// <summary>
/// optimize data�� ������ �༮���̴�.
/// </summary>
	struct OptimizedSkinnedVertex
	{
		OptimizedSkinnedVertex();

		// ��ǥ���� ��ġ��
		Vector3 m_pos;

		// �븻��
		Vector3 m_normal;
		// ź��Ʈ
		Vector3 m_tangent;

		// �ؽ��� ��ǥ
		float u, v;
		float u2, v2;

		float m_weight[4];
		int m_boneindices[4];

	};

	struct OptimizedIndex
	{
		int index[3];
	};

	///--------------------------------------------------
	/// �� ���� Shape Vertex�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeVertex
	{
		int				m_indexnum;			// �ε��� �ѹ�. �׳� �Ϸù�ȣ��
		bool				m_isknot;			// KNOT�ΰ� INTERP�ΰ�?
		Vector3			m_pos;				// ������ ������
	};

	///--------------------------------------------------
	/// �� ���� Shape Object�� ������ �����ϱ� ���� ����ü
	///
	/// 3DSMAX�� ASE����� �״�� �������� ���� ���̴�.
	///--------------------------------------------------
	struct ShapeLine
	{
		int	m_line_number;		// ���° �����ΰ�?
		bool m_shape_closed;		// ���� Shape�ΰ�?
		int	m_shape_vertexcount;// �� ���� Vertex�� �ִ°�?
		vector<ShapeVertex*> m_shapevertex;	// ���ؽ����� vector

		// �ʱ�ȭ�� ���� ������
		ShapeLine();
	};

	/// <summary>
	/// �� ���� Scene�� �����ϱ� ���� ����ü
	/// ASE���Ͽ��� ���, �ڸ�Ʈ ���� ������ SCENE�κ��� �б� ���� ����ü.
	/// </summary>
	struct Scenedata
	{
	public:
		std::string		m_filename;					// ���� �̸�
		int				m_firstframe;
		int				m_lastframe;
		int				m_framespeed;
		int				m_ticksperframe;
		int				m_meshframestep;
		int				m_keyframestep;
		Vector3			m_scene_background_static;
		Vector3			m_scene_ambient_static;

	public:
		Scenedata();
	};

	///--------------------------------------------------
	/// �� ���� Map�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------
	struct MaterialMap
	{
		std::string		m_map_name;
		std::string		m_map_class;
		int		m_subno;
		float		m_map_amount;
		std::string		m_bitmap;
		int		m_map_type;
		float		m_uvw_u_offset;
		float		m_uvw_v_offset;
		float		m_u_tiling;
		float		m_v_tiling;
		float		m_uvw_angle;
		float		m_uvw_blur;
		float		m_uvw_blur_offset;
		float		m_uvw_noise_amt;
		float		m_uvw_noise_size;
		float		m_uvw_noise_level;
		float		m_uvw_noise_phase;
		int		m_bitmap_filter;

		//
		MaterialMap();
	};

	///--------------------------------------------------
	/// �� ���� Material�� �����ϱ� ���� Ŭ����
	///--------------------------------------------------
	struct ASEMaterial
	{
		//--------------------
		//	��� ������
		//--------------------
	public:
		// ������ *MATERIAL_COUNT 0 �ϳ��ۿ� ������
		int					m_materialnumber;			// Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
		std::string					m_material_name;
		std::string					m_material_class;

		Vector3				m_material_ambient;
		Vector3				m_material_diffuse;
		Vector3				m_material_specular;

		float					m_material_shine;
		float					m_material_shinestrength;
		float					m_material_transparency;
		float					m_material_wiresize;
		int					m_material_shading;
		float					m_material_xp_falloff;
		float					m_material_selfillum;
		float					m_material_falloff;
		int					m_material_xp_type;

		int					m_numsubmtls;
		ASEMaterial* m_submaterial;				// �����Ʈ������ �����͸� ������ ����

		MaterialMap* m_map_diffuse;	/// �ϴ� ������ map_diffuse�ۿ� ����
		MaterialMap* m_map_bump; //�븻��


		//CMaterial_Map*			m_map_diffuse;
		//CMaterial_Map*			m_map_diffuse;

		bool					m_istextureexist;	// �ؽ��İ� �����ϴ°�?

		//--------------------
		//	Method
		//--------------------
	public:
		// ������ �Ҹ���
		ASEMaterial();

	};

	// Light�� Animation ( Setting�� ��ȭ�� Animation )
	struct Light_AnimationData
	{
		// Light Setting
		float				m_timevalue;		// ���� mesh�� �ִµ�.. �̰� ��� ���� �̷����̸�.
		Vector3			m_light_color;
		float				m_light_intens;
		float				m_light_aspect;
		float				m_light_hotspot;		///
		float				m_light_falloff;		/// �� �ΰ����� SpotLight�� ���
		float				m_light_tdist;
		float				m_light_mapbias;
		float				m_light_maprange;
		int				m_light_mapsize;
		float				m_light_raybias;

	public:
		Light_AnimationData();

	};

	// Bone �ϳ��� ����ü
	struct Bone
	{
		int		m_bone_number;		// ������ ���ͷ� ���������� ���߿� ������ �ʿ�, �˻��� �ʿ�?
		std::string		m_bone_name;
		// ��¼�� ���⿡�� ��Ʈ������ �־�� �ϴ����� ����. ������ ��ư ���ܵ���

		// �̳༮�� ó���� NodeTM�̴� - offset�� ���ϱ� ���ؼ� �����Ѵ�.
		Matrix* m_boneTM_NodeTM;
		// �̳༮�� �ִϸ��̼��� �����, ������ WorldTM�̴�
		Matrix* m_boneTM_WorldTM;
	};

	// bone�ϳ��� ����� ����ġ �� ��
	struct Weight
	{
		int		m_bone_number;		// weight�� ����� Bone�� IndexNumber;
		float		m_bone_weight;		// ����ġ
	};

	// VertexPos �ϳ��� �־��� ����ġ���� �����ϴ� ����ü (����ü �ϳ��� ���ؽ� �ϳ��� �ϴ��� �����̴�)
	struct WVertex
	{
		int		m_wvertex_number;	// ���ؽ��� ��ȣ. ���� ���ͷ� �����ϹǷ� �� �� ������...

		vector<Weight>	m_bone_blending_weight;	// bone�ϳ��� ����� ����ġ
	};


	struct Tangent
	{
		Vector3 m_tangent;
		Vector3 m_bitangent;
	};

	///--------------------------------------------------
	/// ��� ������Ʈ�� Base Ŭ���� ��� TM�� ��δ� ����ֳ�..?
	///--------------------------------------------------
	struct BaseObjectInfo
	{
		BaseObjectInfo();
		virtual ~BaseObjectInfo();
		BaseObjectInfo(const BaseObjectInfo&) = default;
		BaseObjectInfo& operator=(BaseObjectInfo&) = default;
		BaseObjectInfo(BaseObjectInfo&&) = default;
		BaseObjectInfo& operator=(BaseObjectInfo&&) = default;


		int m_type;
		int m_isAnimated;

		///----------------------------------
		/// *NODE_NAME
		///----------------------------------
		std::string	m_nodename;		// � �������� ������ �Ʒ��� m_meshname�� ���� �̸��� �ѹ� �� ����. �Ƹ��� �޽��� �̸� ����Ʈ�����... �� �׷��� �ƴұ� ��ư ������ ���� 1�̹Ƿ� �������� ������ ����.
		std::string	m_nodeparent;	// �θ� ����� �̸�. �̰��� �ִٸ� �θ� �ִ� ���̴�.
		bool m_isparentexist;		// �׷��� Ȯ���� bool������ ������ ����.

		///----------------------------------
		/// *NODE_TM (Transform Matrix)
		///----------------------------------
		Vector3	m_inherit_pos;
		Vector3	m_inherit_rot;
		Vector3	m_inherit_scl;

		Matrix m_NodeTm;
		Matrix m_InverseNodeTm;

		Vector3	m_tm_pos;
		Vector3	m_tm_rotaxis;
		float m_tm_rotangle;
		Vector3	m_tm_scale;
		Vector3	m_tm_scaleaxis;
		float m_tm_scaleaxisang;
	};

	///--------------------------------------------------
	/// GeomObject �� Ŭ����
	///--------------------------------------------------
	struct GeomObject : public BaseObjectInfo
	{
		GeomObject();
		~GeomObject() override;
		GeomObject(const GeomObject&) = default;
		GeomObject& operator=(GeomObject&) = default;
		GeomObject(GeomObject&&) = default;
		GeomObject& operator=(GeomObject&&) = default;

		///----------------------------------
		/// *MESH
		///----------------------------------
		float m_timevalue;			// ���� ������ ���������� ���δ�. ��ư �ð���.


		// �޽� ���� vertex �迭�� �����ϱ� ���� ������

		/// *MESH_VERTEX_LIST
		int m_mesh_numvertex;
		vector<VertexPos>	m_mesh_vertex;	// ���ؽ�

		/// *MESH_FACE_LIST
		int m_mesh_numfaces;
		vector<Face> m_mesh_face;		// �ε���

		// �ؽ��Ŀ� �����͵�

		/// *MESH_NUMTVERTEX
		int	m_mesh_numtvertex;		// �ؽ��Ŀ� ���ؽ� ����
		vector<TVertex> m_mesh_tvertex;		// �ؽ��Ŀ� ���ؽ�
		/// *MESH_RVERTLIST
		vector<Face> m_mesh_tvfaces;			// �ؽ��Ŀ� ���̽� ���� - (������ ���� Face�� ���� �̹� ������ ��������Ƿ� ���븸 �־��ָ� �ȴ�)

		int	m_mesh_numcvertex;		// �� �̰� ��� ���°�?

		// �븻 ������
		vector<VetexNormal> m_mesh_facenormal; // �� ���̽� �븻��
		Vector3* m_mesh_vertexnormal;		// ���ؽ� �븻�� 3����

		// ź��Ʈ ������ �־�����
		vector<Tangent> m_mesh_tangent;


		//����ȭ�� ���ؽ����� �־�����.
		vector<OptimizedVertex> m_opt_vertex;	/// ����ȭ ���� ���ؽ���
		vector<OptimizedSkinnedVertex> m_optskinned_vertex;	/// ����ȭ ���� ���ؽ���

		vector<OptimizedIndex> m_opt_index;			/// ����ȭ ���� �ε�����

		int				m_material_ref;				//*MATERIAL_REF			// �� �޽��� ��Ʈ������ ��ȣ
		int				m_prop_motionblur;			//*PROP_MOTIONBLUR 0
		int				m_prop_castshadow;			//*PROP_CASTSHADOW 1
		int				m_prop_recvshadow;			//*PROP_RECVSHADOW 1

		Vector3			m_wireframe_color;			//*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726

		//��Ű�� �޽��ΰ�?
		bool m_IsSkinningMesh;
		//�븻���� �ִ°�
		bool m_IsthereUV;

		int	m_numbone;				/// ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
		vector<Bone> m_BoneList;
		vector<WVertex> m_WVertexs;

		Vector3 m_BoundingBox_Min;
		Vector3 m_BoundingBox_Max;

		//int	m_numskinweight;		/// ��Ų ����ġ. ���Ҹ���?
		//int	m_mesh_numvertex;		// (���ؽ��� ��ǥ������)
		//int	m_mesh_numfaces;		// (���ؽ��� �ε���)


	};

	///--------------------------------------------------
	/// HelperObject �� Ŭ����
	///--------------------------------------------------
	struct HelperObject : public BaseObjectInfo
	{
		HelperObject();
		~HelperObject() override;
		HelperObject(const HelperObject&) = default;
		HelperObject& operator=(HelperObject&) = default;
		HelperObject(HelperObject&&) = default;
		HelperObject& operator=(HelperObject&&) = default;

		Vector3 m_BoundingBox_Min;
		Vector3 m_BoundingBox_Max;

		vector<Vector3> m_Control_Pos_Sample;
	};

	struct ShapeObject : public BaseObjectInfo
	{
		ShapeObject();
		~ShapeObject() override;
		ShapeObject(const ShapeObject&) = default;
		ShapeObject& operator=(ShapeObject&) = default;
		ShapeObject(ShapeObject&&) = default;
		ShapeObject& operator=(ShapeObject&&) = default;

		int m_ShapeLineCount;
		vector<Vector3> m_ShapeVertex;
	};

}

/////----------------------------------
		///// Shape Object
		/////----------------------------------
		//int				m_shape_linecount;			// ������ ��
		//vector<ShapeLine*>	m_vector_shape_line;	// ������ ������ ������


		///// ������ ������

		/////----------------------------------
		///// Camera
		/////----------------------------------
		//// �̷��� ������ �ѹ��� �о������ ���� ���α� �ѵ�..
		//// �ϴ� �̷��� �����������ϱ�. �׸��� �ļ��� ��ͱ����� ����� �Ǹ� �̷��� �ϴ°� ���ϴ�
		//// ��� D3D�� Ŭ������ ������ �� ��ȯ�� �Ǵϱ�
		//std::string			m_camera_type;
		//// CAMERA_SETTINGS
		////float				m_timevalue;		// mesh�� �̹� �ִ�.
		//float				m_camera_near;
		//float				m_camera_far;
		//float				m_camera_fov;
		//float				m_camera_tdist;
		//bool				m_camera_isloadTarget;	// ī�޶�� NodeTM�� �ΰ��� �ִµ�, ��ͱ��������� ������ ����� 

		/////----------------------------------
		///// Light
		/////----------------------------------
		//std::string				m_light_type;
		//// Omni�ΰ� SpotLight�ΰ��� ó���� �� ��� �Ѵ�.

		//// Light Property
		//std::string				m_light_shadows;
		//bool				m_light_uselight;
		//std::string				m_light_spotshape;
		//bool				m_light_useglobal;
		//bool				m_light_absmapbias;
		//bool				m_light_overshoot;

		//// Light Setting & Animation
		//Light_AnimationData m_lightanimation;		// Light Setting
		//vector<Light_AnimationData>	m_list_lightanimation;	// �װ��� Animation

		//bool				m_light_isloadTarget;	// ���� NodeTM�� �� �� �ְ�, ī�޶�� ���� ������ �� ������ �ִ�.
		//											/// ��(�ƹ����� ���� ���� ������. �ٲ���.)
		//bool				m_light_isAnimate;		// Animation�� �Ǵ°�?

		/////----------------------------------
		///// Skinning Object
		/////----------------------------------
		//// SKIN_INITTM�� �����ϰ�
		//// TM_ROW0~3
		//// BONE_LIST
		//bool					m_is_skinningobject;	// ��Ű�� ������Ʈ�ΰ�?

		//Bone m_bone;									// Bone �Ѱ��� ������ (�ļ� ���ο��� �ӽ� �������� ����)
		//VertexWeight m_wvertex;						// Weight_Vertex �Ѱ��� ������ (���������� �ӽð���)
		//vector<Bone>			m_vector_bone_list;		// �̰� �׳� ��ȣ�� �̸� �����̴�. �� �ȿ� �����͸� �־���Ƶ� �����ڱ���(�����ϱ� ���ϰ� - �׷��� ������ �Ź� �̸����� �˻��� �ؾߵ�����. �ƴϸ� �ε����� ������ ��� �ð���� ������ �Ҽ���?)
		//vector<VertexWeight>	m_vector_wvertexs;		// ���ؽ� �ϳ��� �޶�پ��ִ� ����ġ��
