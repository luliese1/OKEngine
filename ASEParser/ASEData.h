/// <summary>
/// CParsingDataClass.h
/// 
/// Ver 1.0
/// ASE파일을 파싱했을때 나오는 데이터들을 구조체로 만들었다.
/// 
/// 일단 한 번 읽어들이는 것이고, 렌더러에 독립적으로 만들어야 한다.
/// D3DX접두어나, XM접두어가 없었으면 좋겠다.
/// 
/// (예전 주석)
/// 죄다 struct에 public으로 했는데 이유는 아무래도
/// 여기 저기에서 접근 가능해야하기 때문.
/// 
/// 대신 이 자료구조를 넘겨받는 D3D용 데이터형들은
/// 캡슐화가 되어있다. 대신 변환 함수들은 그쪽으로 갈 수도 있다.
/// (그렇지 않다면 무수한 GetSet메소드들을 만들어야 한다)
/// (예전 주석)
/// 
/// 2021.04.11 LeHideOffice
/// </summary>

#pragma once

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

namespace ASEData {

	///--------------------------------------------------
	/// Animation Data를 저장하기 위한 클래스
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
		//쓸곳을 찾았다.
		XMVECTOR	m_rotQT_accumulation;		// 상대변량이기 때문에 이전 값들과 누적해서 가지고 있는다 (즉 지금까지의 절대량으로 변환한다)
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
	/// 위 클래스를 조합한 Animation 클래스
	///--------------------------------------------------
	struct Animation
	{
		// 애니메이션 데이터의 이름. 유일한 참조수단이다
		int					m_nodeIndex;
		vector<Animation_Control_Pos>		m_position;
		vector<Animation_Control_Rot>		m_rotation;
		vector<Animation_Control_Scl>		m_scale;

		int		m_ticksperFrame;	// 한 프레임의 틱 크기. 이것은 AnimationTable을 가지는 각각의 오브젝트마다 다를 수 있다.
		int		m_minKeyframe;		// 키 프레임간의 최소크기. 이것의 위의것과 꼭 일치하지는 않는다.
	};

	///--------------------------------------------------
	/// 한 개의 VertexPos
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///--------------------------------------------------
	struct VertexPos
	{
		VertexPos();
		//VertexPos(VertexPos&& other);


		// 좌표상의 위치값
		Vector3 m_pos;

	};
	///--------------------------------------------------
	/// 한 개의 Texture - VertexPos
	///
	/// 이것은 Raw데이터이며 최적화가 이뤄져서 결국은
	/// COneVertex에 들어가게 된다.
	///--------------------------------------------------
	struct TVertex
	{
		TVertex();

		float m_uvPos[3]; // u, v, w
	};

	///--------------------------------------------------
	/// 한 개의 Face
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///
	/// 파싱 초기에 Face의 갯수는 정해져 있으며
	/// 중복되는 VertexNormal값, 텍스쳐 코디네이트 값이
	/// 있을 수 있다. 그것을 나눠줘야 한다.
	///--------------------------------------------------
	struct Face
	{
		// 이 Face를 이루는 버텍스의 인덱스
		int				m_vertexindex[3];
		//// Face의 Normal값
		//Vector3			m_normal;
		//// Vertex의 Normal값
		//Vector3			m_normalvertex[3];
		//// Texture Coordinate
		//int				m_TFace[3];

		// 초기화를 위한 생성자.
		Face();
	};
	///--------------------------------------------------
	/// 한 개의 FaceNormal
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///
	/// 파싱 초기에 Face의 갯수는 정해져 있으며
	/// 중복되는 VertexNormal값, 텍스쳐 코디네이트 값이
	/// 있을 수 있다. 그것을 나눠줘야 한다.
	///--------------------------------------------------
	struct VetexNormal
	{
		//facenormal.. 어따쓰지?
		Vector3				m_faceNormal;

		int m_vertexIndex[3];
		//vertexNormal 써야할 자료들!
		Vector3				m_vertexNormal[3];

		// 초기화를 위한 생성자.
		VetexNormal();
	};

	/// <summary>
	/// optimize data를 포함할 녀석들이다.
	/// </summary>
	struct OptimizedVertex
	{
		OptimizedVertex();

		// 좌표상의 위치값
		Vector3 m_pos;

		// 노말값
		Vector3 m_normal;
		// 탄젠트를 넣자
		Vector3 m_tangent;

		// 텍스쳐 좌표
		float u, v;
		float u2, v2;

	};

	/// <summary>
/// optimize data를 포함할 녀석들이다.
/// </summary>
	struct OptimizedSkinnedVertex
	{
		OptimizedSkinnedVertex();

		// 좌표상의 위치값
		Vector3 m_pos;

		// 노말값
		Vector3 m_normal;
		// 탄제트
		Vector3 m_tangent;

		// 텍스쳐 좌표
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
	/// 한 개의 Shape Vertex의 정보를 저장하기 위한 구조체
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///--------------------------------------------------
	struct ShapeVertex
	{
		int				m_indexnum;			// 인덱스 넘버. 그냥 일련번호다
		bool				m_isknot;			// KNOT인가 INTERP인가?
		Vector3			m_pos;				// 포지션 데이터
	};

	///--------------------------------------------------
	/// 한 개의 Shape Object의 정보를 저장하기 위한 구조체
	///
	/// 3DSMAX의 ASE출력을 그대로 가져오기 위한 것이다.
	///--------------------------------------------------
	struct ShapeLine
	{
		int	m_line_number;		// 몇번째 라인인가?
		bool m_shape_closed;		// 닫힌 Shape인가?
		int	m_shape_vertexcount;// 몇 개의 Vertex가 있는가?
		vector<ShapeVertex*> m_shapevertex;	// 버텍스들의 vector

		// 초기화를 위한 생성자
		ShapeLine();
	};

	/// <summary>
	/// 한 개의 Scene을 저장하기 위한 구조체
	/// ASE파일에서 헤더, 코멘트 다음 나오는 SCENE부분을 읽기 위한 구조체.
	/// </summary>
	struct Scenedata
	{
	public:
		std::string		m_filename;					// 파일 이름
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
	/// 한 개의 Map을 저장하기 위한 클래스
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
	/// 한 개의 Material을 저장하기 위한 클래스
	///--------------------------------------------------
	struct ASEMaterial
	{
		//--------------------
		//	멤버 변수들
		//--------------------
	public:
		// 지금은 *MATERIAL_COUNT 0 하나밖에 없더라
		int					m_materialnumber;			// Material의 번호. 이것을 기준으로 오브젝트에서 참조한다.
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
		ASEMaterial* m_submaterial;				// 서브매트리얼은 포인터만 가지고 있자

		MaterialMap* m_map_diffuse;	/// 일단 지금은 map_diffuse밖에 없다
		MaterialMap* m_map_bump; //노말맵


		//CMaterial_Map*			m_map_diffuse;
		//CMaterial_Map*			m_map_diffuse;

		bool					m_istextureexist;	// 텍스쳐가 존재하는가?

		//--------------------
		//	Method
		//--------------------
	public:
		// 생성자 소멸자
		ASEMaterial();

	};

	// Light의 Animation ( Setting의 변화의 Animation )
	struct Light_AnimationData
	{
		// Light Setting
		float				m_timevalue;		// 역시 mesh에 있는데.. 이거 어떻게 하지 이런식이면.
		Vector3			m_light_color;
		float				m_light_intens;
		float				m_light_aspect;
		float				m_light_hotspot;		///
		float				m_light_falloff;		/// 이 두가지는 SpotLight인 경우
		float				m_light_tdist;
		float				m_light_mapbias;
		float				m_light_maprange;
		int				m_light_mapsize;
		float				m_light_raybias;

	public:
		Light_AnimationData();

	};

	// Bone 하나의 구조체
	struct Bone
	{
		int		m_bone_number;		// 어차피 벡터로 잡을거지만 나중에 참조시 필요, 검색시 필요?
		std::string		m_bone_name;
		// 어쩌면 여기에서 매트릭스가 있어야 하는지도 몰라. 하지만 여튼 남겨두자

		// 이녀석은 처음의 NodeTM이다 - offset을 구하기 위해서 존재한다.
		Matrix* m_boneTM_NodeTM;
		// 이녀석은 애니메이션이 적용된, 마지막 WorldTM이다
		Matrix* m_boneTM_WorldTM;
	};

	// bone하나와 연결된 가중치 한 개
	struct Weight
	{
		int		m_bone_number;		// weight가 적용된 Bone의 IndexNumber;
		float		m_bone_weight;		// 가중치
	};

	// VertexPos 하나에 주어진 가중치들을 포함하는 구조체 (구조체 하나가 버텍스 하나와 일대일 대응이다)
	struct WVertex
	{
		int		m_wvertex_number;	// 버텍스의 번호. 역시 벡터로 관리하므로 알 수 있지만...

		vector<Weight>	m_bone_blending_weight;	// bone하나와 연결된 가중치
	};


	struct Tangent
	{
		Vector3 m_tangent;
		Vector3 m_bitangent;
	};

	///--------------------------------------------------
	/// 모든 오브젝트의 Base 클래스 노드 TM은 모두다 들고있네..?
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
		std::string	m_nodename;		// 어떤 식인지는 모르지만 아래의 m_meshname과 같은 이름이 한번 더 들어간다. 아마도 메쉬의 이름 리스트라던지... 뭐 그런게 아닐까 여튼 지금은 버전 1이므로 이정도로 만족을 하자.
		std::string	m_nodeparent;	// 부모 노드의 이름. 이것이 있다면 부모가 있는 것이다.
		bool m_isparentexist;		// 그래도 확실히 bool형으로 가지고 있자.

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
	/// GeomObject 용 클래스
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
		float m_timevalue;			// 여러 곳에서 여러가지로 쓰인다. 여튼 시간값.


		// 메시 안의 vertex 배열을 선언하기 위한 포인터

		/// *MESH_VERTEX_LIST
		int m_mesh_numvertex;
		vector<VertexPos>	m_mesh_vertex;	// 버텍스

		/// *MESH_FACE_LIST
		int m_mesh_numfaces;
		vector<Face> m_mesh_face;		// 인덱스

		// 텍스쳐용 데이터들

		/// *MESH_NUMTVERTEX
		int	m_mesh_numtvertex;		// 텍스쳐용 버텍스 갯수
		vector<TVertex> m_mesh_tvertex;		// 텍스쳐용 버텍스
		/// *MESH_RVERTLIST
		vector<Face> m_mesh_tvfaces;			// 텍스쳐용 페이스 갯수 - (갯수는 위의 Face와 같고 이미 위에서 만들었으므로 내용만 넣어주면 된다)

		int	m_mesh_numcvertex;		// ※ 이건 어디에 쓰는것?

		// 노말 정보들
		vector<VetexNormal> m_mesh_facenormal; // 각 페이스 노말에
		Vector3* m_mesh_vertexnormal;		// 버텍스 노말이 3개씩

		// 탄젠트 정보를 넣어주자
		vector<Tangent> m_mesh_tangent;


		//최적화한 버텍스들을 넣어주자.
		vector<OptimizedVertex> m_opt_vertex;	/// 최적화 후의 버텍스들
		vector<OptimizedSkinnedVertex> m_optskinned_vertex;	/// 최적화 후의 버텍스들

		vector<OptimizedIndex> m_opt_index;			/// 최적화 후의 인덱스들

		int				m_material_ref;				//*MATERIAL_REF			// 이 메쉬의 매트리얼의 번호
		int				m_prop_motionblur;			//*PROP_MOTIONBLUR 0
		int				m_prop_castshadow;			//*PROP_CASTSHADOW 1
		int				m_prop_recvshadow;			//*PROP_RECVSHADOW 1

		Vector3			m_wireframe_color;			//*WIREFRAME_COLOR 0.89803922	0.65098041	0.84313726

		//스키닝 메쉬인가?
		bool m_IsSkinningMesh;
		//노말맵이 있는게
		bool m_IsthereUV;

		int	m_numbone;				/// 본의 갯수. 이것이 있다면 스키닝 오브젝트로 간주한다.
		vector<Bone> m_BoneList;
		vector<WVertex> m_WVertexs;

		Vector3 m_BoundingBox_Min;
		Vector3 m_BoundingBox_Max;

		//int	m_numskinweight;		/// 스킨 가중치. 뭔소린지?
		//int	m_mesh_numvertex;		// (버텍스의 좌표데이터)
		//int	m_mesh_numfaces;		// (버텍스의 인덱스)


	};

	///--------------------------------------------------
	/// HelperObject 용 클래스
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
		//int				m_shape_linecount;			// 라인의 수
		//vector<ShapeLine*>	m_vector_shape_line;	// 쉐이프 라인의 데이터


		///// 나머지 정보들

		/////----------------------------------
		///// Camera
		/////----------------------------------
		//// 이렇게 모든것을 한번에 읽어버리는 것은 별로긴 한데..
		//// 일단 이렇게 만들어버렸으니까. 그리고 파서를 재귀구조로 만들게 되면 이렇게 하는게 편하다
		//// 어쩄든 D3D용 클래스로 전달할 때 변환이 되니까
		//std::string			m_camera_type;
		//// CAMERA_SETTINGS
		////float				m_timevalue;		// mesh에 이미 있다.
		//float				m_camera_near;
		//float				m_camera_far;
		//float				m_camera_fov;
		//float				m_camera_tdist;
		//bool				m_camera_isloadTarget;	// 카메라는 NodeTM이 두개가 있는데, 재귀구조내에서 문제가 생긴다 

		/////----------------------------------
		///// Light
		/////----------------------------------
		//std::string				m_light_type;
		//// Omni인가 SpotLight인가의 처리를 해 줘야 한다.

		//// Light Property
		//std::string				m_light_shadows;
		//bool				m_light_uselight;
		//std::string				m_light_spotshape;
		//bool				m_light_useglobal;
		//bool				m_light_absmapbias;
		//bool				m_light_overshoot;

		//// Light Setting & Animation
		//Light_AnimationData m_lightanimation;		// Light Setting
		//vector<Light_AnimationData>	m_list_lightanimation;	// 그것의 Animation

		//bool				m_light_isloadTarget;	// 빛도 NodeTM이 두 개 있고, 카메라와 같은 이유로 이 변수가 있다.
		//											/// ※(아무래도 좋지 않은 구조다. 바꾸자.)
		//bool				m_light_isAnimate;		// Animation이 되는가?

		/////----------------------------------
		///// Skinning Object
		/////----------------------------------
		//// SKIN_INITTM은 무시하고
		//// TM_ROW0~3
		//// BONE_LIST
		//bool					m_is_skinningobject;	// 스키닝 오브젝트인가?

		//Bone m_bone;									// Bone 한개의 포인터 (파서 내부에서 임시 공간으로 쓰임)
		//VertexWeight m_wvertex;						// Weight_Vertex 한개의 포인터 (마찬가지로 임시공간)
		//vector<Bone>			m_vector_bone_list;		// 이건 그냥 번호와 이름 관계이다. 이 안에 포인터를 넣어놓아도 괜찮겠구나(참조하기 편하게 - 그렇지 않으면 매번 이름으로 검색을 해야될지도. 아니면 인덱스가 있으니 상수 시간대로 접근을 할수도?)
		//vector<VertexWeight>	m_vector_wvertexs;		// 버텍스 하나에 달라붙어있는 가중치들
