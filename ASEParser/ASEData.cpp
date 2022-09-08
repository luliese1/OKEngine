#include "Parserpch.h"
#include "ASEData.h"

namespace ASEData {

	VertexPos::VertexPos()
		: m_pos()
		//, m_normal()
		//, u(0.0f), v(0.0f), u2(0.0f), v2(0.0f)
		//, m_isnormalset(false)
		//, m_istextureset(false)
		//, m_bw1(0.0f), m_bw2(0.0f), m_bw3(0.0f), m_indices(0), m_indicesPointer(nullptr)
	{

	}

	Face::Face() : m_vertexindex{ 0, 0, 0 }
		//, m_normal(0, 0, 0)
		//, m_TFace{ 0, 0, 0 }
	{

	}

	TVertex::TVertex() : m_uvPos{ 0, 0, 0 }
	{
	}

	ShapeLine::ShapeLine() : m_line_number(0), m_shape_closed(false), m_shape_vertexcount(0)
	{

		//m_line_number = 0;		// ���° �����ΰ�?
		//m_shape_closed = false;	// ���� Shape�ΰ�?
		//m_shape_vertexcount = 0;		// �� ���� Vertex�� �ִ°�?
		////m_shapevertex;					// ���ؽ����� vector
	}

	Scenedata::Scenedata()
	{
		//m_filename;		CString �ʱ�ȭ �ʿ����?
		m_firstframe = 0;
		m_lastframe = 0;
		m_framespeed = 0;
		m_ticksperframe = 0;
		m_meshframestep = 0;
		m_keyframestep = 0;
		m_scene_background_static = Vector3(0, 0, 0);
		m_scene_ambient_static = Vector3(0, 0, 0);
	}

	MaterialMap::MaterialMap()
	{
		// 	CString		m_map_name;
		// 	CString		m_map_class;
		m_subno = 0;
		m_map_amount = 0;
		//	CString		m_bitmap;
		m_map_type = 0;
		m_uvw_u_offset = 0;
		m_uvw_v_offset = 0;
		m_u_tiling = 0;
		m_v_tiling = 0;
		m_uvw_angle = 0;
		m_uvw_blur = 0;
		m_uvw_blur_offset = 0;
		m_uvw_noise_amt = 0;
		m_uvw_noise_size = 0;
		m_uvw_noise_level = 0;
		m_uvw_noise_phase = 0;
		m_bitmap_filter = 0;
	}

	ASEMaterial::ASEMaterial()
	{
		m_materialnumber = 0;	// Material�� ��ȣ. �̰��� �������� ������Ʈ���� �����Ѵ�.
		//CString					m_material_name;
		//CString					m_material_class;

		m_submaterial = nullptr;
		m_map_diffuse = nullptr;
		m_map_bump = nullptr;

		m_material_ambient = Vector3(0, 0, 0);
		m_material_diffuse = Vector3(0, 0, 0);
		m_material_specular = Vector3(0, 0, 0);
		m_material_shine = 0.0f;
		m_material_shinestrength = 0.0f;
		m_material_transparency = 0.0f;
		m_material_wiresize = 0;
		m_material_shading = 0;
		m_material_xp_falloff = 0.0f;
		m_material_selfillum = 0.0f;
		m_material_falloff = 0.0f;
		m_material_xp_type = 0;

		m_numsubmtls = 0;

		m_istextureexist = false;
	}


	Light_AnimationData::Light_AnimationData()
	{
		// Light Setting
		m_timevalue = -1;// �̰� �ִϸ��̼��� ���°���� �����̴�. �׷��� �ð��� -1�� ��찡 ������ ��¼��?
		m_light_color = Vector3(1.0f, 1.0f, 1.0f);
		m_light_intens = 0.0f;
		m_light_aspect = 0.0f;
		m_light_hotspot = 0.0f;		///
		m_light_falloff = 0.0f;		/// �� �ΰ����� SpotLight�� ���
		m_light_tdist = 0.0f;
		m_light_mapbias = 0.0f;
		m_light_maprange = 0.0f;
		m_light_mapsize = 0;
		m_light_raybias = 0.0f;
	}

	OptimizedVertex::OptimizedVertex()
		: m_pos(), m_normal()
		, u(0.0f), v(0.0f), u2(0.0f), v2(0.0f)
	{

	}

	VetexNormal::VetexNormal()
		: m_faceNormal(), m_vertexNormal()
	{

	}

	BaseObjectInfo::BaseObjectInfo()
	{
		m_type = 0;
		m_isAnimated = false;
		//CString				m_nodename;				// CString�� �ڵ����� �ʱ�ȭ �����ִ°�?
		//CString				m_nodeparent;
		m_isparentexist = false;					// �⺻������ �θ� ���� ����ġ �ʴ´�.

		//CString				m_meshname;
		m_inherit_pos = Vector3(0, 0, 0);
		m_inherit_rot = Vector3(0, 0, 0);
		m_inherit_scl = Vector3(0, 0, 0);

		m_tm_pos = Vector3(0, 0, 0);
		m_tm_rotaxis = Vector3(0, 0, 0);
		m_tm_rotangle = 0;
		m_tm_scale = Vector3(0, 0, 0);
		m_tm_scaleaxis = Vector3(0, 0, 0);
		m_tm_scaleaxisang = 0;
	}
	BaseObjectInfo::~BaseObjectInfo()
	{

	}

	GeomObject::GeomObject() : BaseObjectInfo()
	{
		m_timevalue = 0;
		m_numbone = 0;					/// ���� ����. �̰��� �ִٸ� ��Ű�� ������Ʈ�� �����Ѵ�.
		m_mesh_numvertex = 0;
		m_mesh_numfaces = 0;

		m_material_ref = -1;

		m_prop_motionblur = 0;
		m_prop_castshadow = 0;
		m_prop_recvshadow = 0;
		m_wireframe_color = Vector3(0, 0, 0);

		m_mesh_numtvertex = 0;
		m_mesh_numcvertex = 0;

		m_mesh_vertexnormal = 0;
		m_IsSkinningMesh = false;
		m_IsthereUV = false;
	}
	GeomObject::~GeomObject()
	{

	}

	HelperObject::HelperObject() : BaseObjectInfo()
	{

	}
	HelperObject::~HelperObject()
	{

	}

	ShapeObject::ShapeObject() : BaseObjectInfo(), m_ShapeLineCount()
	{

	}

	ShapeObject::~ShapeObject()
	{
	}

	OptimizedSkinnedVertex::OptimizedSkinnedVertex() :
		m_pos(), m_normal(),
		u(0.0f), v(0.0f), u2(0.0f), v2(0.0f), m_weight(), m_boneindices()
	{

	}

}

//	----------------------------------
//	 Camera
//	----------------------------------
//	CString				m_camera_type;
//	 CAMERA_SETTINGS
//	m_camera_near = 0.0f;
//	m_camera_fov = 0.0f;
//	m_camera_tdist = 0.0f;

//	m_camera_isloadTarget = false;

//	----------------------------------
//	 Light
//	----------------------------------
//	CString				m_light_type;
//	 �̳༮�� Target�� �ֱ�..

//	 Light Property
//	CString				m_light_shadows;
//	m_light_uselight = false;
//	CString				m_spotshape;
//	m_light_useglobal = false;
//	m_light_absmapbias = false;
//	m_light_overshoot = false;

//	 Light Setting
//	m_light_isloadTarget = false;
//	m_light_isAnimate = false;

//	----------------------------------
//	 Skinning Object
//	----------------------------------
//	 SKIN_INITTM�� �����ϰ�
//	 TM_ROW0~3
//	 BONE_LIST
//	m_is_skinningobject = false;		// ��Ű�� ������Ʈ�ΰ�?
//	vector<CBone*>			m_vector_bone_list;		// �̰� �׳� ��ȣ�� �̸� �����̴�. �� �ȿ� �����͸� �־���Ƶ� �����ڱ���(�����ϱ� ���ϰ� - �׷��� ������ �Ź� �̸����� �˻��� �ؾߵ�����. �ƴϸ� �ε����� ������ ��� �ð���� ������ �Ҽ���?)
//	vector<CWeight_Vertex*>	m_vector_wvertexs;		// ���ؽ� �ϳ��� �޶�پ��ִ� ����ġ��