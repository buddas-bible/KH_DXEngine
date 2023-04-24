 #include "CASEParser.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	meshCount = 0;

	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
/*
	// ���ؽ����� �ϴ� ��� ����
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
	unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face�������� ���ؽ��� �븻�� �־��ش�.
			// (���ÿ�)
			_nowVertex->m_normal.x = 0;
			_nowVertex->m_normal.y = 0;
			_nowVertex->m_normal.z = 0;
		}
	}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

*/
	return FALSE;
}

/// <summary>
/// ���ؽ��� �ؽ��� ���ؽ��� �����ִ� ��.
/// 
/// face�� �ε����� �����ϸ鼭 �ش��ϴ� ���ؽ��� �ؽ��� ��ǥ�� �߰�����.
/// </summary>
Mesh*& CASEParser::CreateVertexList(Mesh*& mesh)
{
	const auto& face = mesh->m_meshface;			// �޽��� face
	const auto& vertex = mesh->m_meshvertex;
	const auto& tvertex = mesh->m_mesh_tvertex;		// �޽��� t
	mesh->istexture = tvertex.empty();

	// �ؽ��� ��ǥ�� ���� �޽��� �Ѿ
	for (size_t i = 0; i < face.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int v1 = face[i]->m_vertexindex[j];			// face���� ���ؽ��� �ε����� ������
			Vector3& n = face[i]->m_normalvertex[j];	// face���� �븻 ���� ������

			Vertex* v = new Vertex;
			const Vertex& vt = *vertex[v1];
			v->m_pos = vt.m_pos;				// ���̽��� ����� �ε����� �ִ� pos�� ������
			v->m_normal = n;					// ���̽��� ����� normal�� ������
			if (mesh->istexture)				// texture �������� ����?
			{
				v->u = 0.f;
				v->v = 0.f;
			}
			else
			{
				int t1 = face[i]->m_TFace[j];				// face���� �ؽ��� ��ǥ�� ������
				v->u = tvertex[t1]->m_u;
				v->v = tvertex[t1]->m_v;
			}

			/// �ش� ���ؽ��� ���� �ȿ� �ִ��� Ȯ��
			/// ������ ���ۿ� ���ؽ��� �ְ� �ε��� ���ۿ� �ε����� �ְ�
			/// �ε����� ������Ŵ.
			/// 
			/// �ش� ���ؽ��� ���� �ȿ� ������
			/// �ش� �ε����� �����ͼ� �ε��� ���ۿ� �ε����� ����. 

			int index = GetVertexIndex(mesh, v);

			mesh->m_rawIndex.push_back(index);
		}
	}
	
	for (auto& e : face)
	{
		delete e;
	}
	for (auto& e : vertex)
	{
		delete e;
	}
	for (auto& e : tvertex)
	{
		delete e;
	}
	vertexMap.clear();

	return mesh;
}


int CASEParser::GetVertexIndex(Mesh*& mesh, Vertex*& v)
{
	int size = mesh->m_rawVertex.size();
	Vertex temp = *v;

	auto itr = vertexMap.find(temp);			// �ʿ��� ���ؽ��� ã��
	if (itr != vertexMap.end())					// ���ؽ��� ã�� �ȴٸ�
	{
		delete v;
		return vertexMap[temp];					// �ش� �ε��� ��ȯ
	}
	else										// ���ؽ��� ã�� ���Ѵٸ�
	{
		vertexMap[*v] = size;					// �ش� ���ؽ��� �ʿ� �ְ�
		mesh->m_rawVertex.push_back(v);			// ���ؽ� �迭�� ������ ����.
		return size;							// �ε��� ��ȯ
	}
}


int CASEParser::GetFVertexIndex()
{
	return 0;
}

ASEParser::Mesh* CASEParser::GetOptMesh(int index)
{
	return CreateVertexList(m_MeshList[index]);
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}


int CASEParser::GetMeshCount()
{
	return meshCount;
}

/*
ASEParser::Mesh* CASEParser::GetMesh()
{
	return CreateVertexList(m_MeshList[meshCount]);
}
*/

//----------------------------------------------------------------
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
			break;

			//--------------------
			// SCENE
			//--------------------
#pragma region SCENE
		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// �׳� ���� ������ �о������ }�� ���ö�����
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;
#pragma endregion

		//--------------------
		// MATERIAL_LIST
		//--------------------

#pragma region Material
		
		case TOKENR_MATERIAL_LIST:
			Create_materialdata_to_list();
			break;
		case TOKENR_MATERIAL_COUNT:
			m_materialcount = Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL:
			m_materialdata->m_materialnumber = Parsing_NumberInt();
			break;
		case TOKENR_MATERIAL_NAME:
			m_materialdata->m_material_name = Parsing_String();
			break;
		case TOKENR_MATERIAL_CLASS:
			m_materialdata->m_material_class = Parsing_String();
			break;
		case TOKENR_MATERIAL_AMBIENT:
			m_materialdata->m_material_ambient = Parsing_NumberVector3();
			break;
		case TOKENR_MATERIAL_DIFFUSE:
			m_materialdata->m_material_diffuse = Parsing_NumberVector3();
			break;
		case TOKENR_MATERIAL_SPECULAR:
			m_materialdata->m_material_specular = Parsing_NumberVector3();
			break;
		case TOKENR_MATERIAL_SHINE:
			m_materialdata->m_material_shine = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SHINESTRENGTH:
			m_materialdata->m_material_shinestrength = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_WIRESIZE:
			m_materialdata->m_material_wiresize = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SHADING:
			m_materialdata->m_material_shading = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_XP_FALLOFF:
			m_materialdata->m_material_xp_falloff = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_SELFILLUM:
			m_materialdata->m_material_selfillum = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_FALLOFF:
			m_materialdata->m_material_falloff = Parsing_NumberFloat();
			break;
		case TOKENR_MATERIAL_XP_TYPE:
			m_materialdata->m_material_xp_type = Parsing_NumberFloat();
			break;
		
#pragma endregion

#pragma region MapDiffuse
		case TOKENR_MAP_DIFFUSE:
			break;
		case TOKENR_MAP_NAME:
			break;
		case TOKENR_MAP_CLASS:
			break;
		case TOKENR_MAP_SUBNO:
			break;
		case TOKENR_MAP_AMOUNT:
			break;
#pragma endregion

#pragma region MapBump
#pragma endregion

		//--------------------
		// GEOMOBJECT
		//--------------------

#pragma region GeomObject
		case TOKENR_GROUP:
			break;

			
		case TOKENR_HELPEROBJECT:
			Create_onemesh_to_list();
			m_OneMesh->m_type = eHelperObject;
			break;
			

		case TOKENR_HELPER_CLASS:
		{}			// ����
		break;
			

		case TOKENR_GEOMOBJECT:
		{
			/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?
			meshCount++;
			Create_onemesh_to_list();
			m_OneMesh->m_type = eGeomobject;
		}
		break;
		
		case TOKENR_NODE_NAME:
			// ��� ������ ������Ʈ���� ���� �� �� �ִ� ������ ���̴�.
			// ��忡 ���� �־�� �� ���� �ٸ���.
			switch (m_OneMesh->m_type)
			{
				case eGeomobject:
					m_OneMesh->m_nodename = Parsing_String();
					break;
				case eHelperObject:
					m_OneMesh->m_nodename = Parsing_String();
					break;
				case eAnimation:
					m_animation->m_nodename = Parsing_String();
					break;
				case eBone_List:
					m_OneMesh->m_bone->m_bone_name = Parsing_String();
					break;

				default:
					break;
			}
			break;

		case TOKENR_NODE_PARENT:
			m_OneMesh->m_nodeparent = Parsing_String();
			m_OneMesh->m_isparentexist = true;
			nodeMap[m_OneMesh->m_nodeparent] = m_OneMesh;
			break;
		

			/// NODE_TM

		case TOKENR_NODE_TM:
			//m_parsingmode	=	eGeomobject;
			// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
			// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

			/// �Դٰ�,
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
			// ��ͱ����� �������̴�....

			break;

			
		case TOKENR_INHERIT_POS:
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			m_OneMesh->m_inherit_pos = Parsing_NumberVector3();
			break;
		case TOKENR_INHERIT_ROT:
			m_OneMesh->m_inherit_rot = Parsing_NumberVector3();
			break;
		case TOKENR_INHERIT_SCL:
			m_OneMesh->m_inherit_scl = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROW0:
			m_OneMesh->m_tm_row0 = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROW1:
			m_OneMesh->m_tm_row2 = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROW2:
			m_OneMesh->m_tm_row1 = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROW3:
			m_OneMesh->m_tm_row3 = Parsing_NumberVector3();
			break;
		case TOKENR_TM_POS:
			m_OneMesh->m_tm_pos = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROTAXIS:
			m_OneMesh->m_tm_rotaxis = Parsing_NumberVector3();
			break;
		case TOKENR_TM_ROTANGLE:
			m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
			break;
		case TOKENR_TM_SCALE:
			m_OneMesh->m_tm_scale = Parsing_NumberVector3();
			break;
		case TOKENR_TM_SCALEAXIS:
			m_OneMesh->m_tm_scaleaxis = Parsing_NumberVector3();
			break;
		case TOKENR_TM_SCALEAXISANG:
			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
			m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
			break;
			
			
#pragma endregion

#pragma region Animation
			
		case TOKENR_TM_ANIMATION:
			m_OneMesh->m_isAnimated = true;
			// m_OneMesh->m_type = eAnimation;
			Create_animationdata_to_list();
			Parsing_String();
			Parsing_String();
			m_animation->m_nodename = Parsing_String();
			break;

		case TOKENR_CONTROL_POS_TRACK:
			break;

		case TOKENR_CONTROL_POS_SAMPLE:
		{
			CAnimation_pos* animation_pos = new CAnimation_pos;
			animation_pos->m_time = Parsing_NumberInt();
			animation_pos->m_pos = Parsing_NumberVector3();
			m_animation->m_position.push_back(animation_pos);
		}
			break;

		case TOKENR_CONTROL_ROT_TRACK:
			break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			CAnimation_rot* animation_rot = new CAnimation_rot;
			animation_rot->m_time = Parsing_NumberInt();
			animation_rot->m_rot = Parsing_NumberVector3();
			animation_rot->m_angle = Parsing_NumberFloat();
			m_animation->m_rotation.push_back(animation_rot);
		}
			break;

#pragma endregion

#pragma region Mesh
		case TOKENR_MESH:
			//
			break;
		case TOKENR_TIMEVALUE:
			m_OneMesh->m_timevalue = Parsing_NumberInt();
			break;
		case TOKENR_MESH_NUMBONE:
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
			// ���� �Է�
			m_OneMesh->m_numbone = Parsing_NumberInt();
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
			m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
			break;
		case TOKENR_MESH_NUMFACES:
			m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
			break;
#pragma endregion

#pragma region VertexList
		case TOKENR_MESH_VERTEX_LIST:
			break;

		case TOKENR_MESH_VERTEX:
		{
			Create_onevertex_to_list();
			int index = Parsing_NumberInt();
			m_OneMesh->m_meshvertex[index]->m_pos = Parsing_NumberVector3();
		}
		break;
#pragma endregion

#pragma region Bone
		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
			// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
			// Bone�� �ѹ��� �о� ����
			ASEParser::Bone* b = new ASEParser::Bone;
			m_OneMesh->m_vector_bone_list.push_back(b);
			m_OneMesh->m_is_skinningobject = true;
		}
		break;
		//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
		case TOKENR_BONE_NAME:
		{

		}
		break;
		case TOKENR_BONE_PROPERTY:
			// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
			break;
			// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// ��ü �� �ܰ踦 ���°ž�...
			// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
			/// �� ��....
		}
		break;
#pragma endregion


		/// MESH_FACE_LIST
#pragma region FaceList
		case TOKENR_MESH_FACE_LIST:
			//
			break;
		case TOKENR_MESH_FACE:
		{
			ASEParser::Face* f = new ASEParser::Face;
			// Face�� ��ȣ�ε�...
			Parsing_String();
			// A:�� �а�
			Parsing_String();
			f->m_vertexindex[0] = Parsing_NumberInt();
			// B:
			Parsing_String();
			f->m_vertexindex[2] = Parsing_NumberInt();
			// C:
			Parsing_String();
			f->m_vertexindex[1] = Parsing_NumberInt();

			/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
			/// ......

			// ���Ϳ� �־��ش�.
			m_OneMesh->m_meshface.push_back(f);
		}
		break;
#pragma endregion

#pragma region MeshNormal
		case TOKENR_MESH_NORMALS:
			break;

		case TOKENR_MESH_FACENORMAL:
		{
			int index = Parsing_NumberInt();
			listcount = index;
			m_OneMesh->m_meshface[index]->m_normal = Parsing_NumberVector3();
		}
		break;

		case TOKENR_MESH_VERTEXNORMAL:
		{
			Parsing_NumberInt();
			m_OneMesh->m_meshface[listcount]->m_normalvertex[0] = Parsing_NumberVector3();
			Parsing_String();
			Parsing_NumberInt();
			m_OneMesh->m_meshface[listcount]->m_normalvertex[2] = Parsing_NumberVector3();
			Parsing_String();
			Parsing_NumberInt();
			m_OneMesh->m_meshface[listcount]->m_normalvertex[1] = Parsing_NumberVector3();
		}
		break;
#pragma endregion


#pragma region TextureVertexList
		case TOKENR_MESH_NUMTVERTEX:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
			break;

			/// MESH_TVERTLIST

		case TOKENR_MESH_TVERTLIST:
			//
			break;
		case TOKENR_MESH_TVERT:
		{
			// ���ؽ��� �ε����� �����µ� ������ ������ �����Ƿ� ������.
			// ���ο� TVertex�� ���� ���Ϳ� �ִ´�
			int index = Parsing_NumberInt();
			COneTVertex* t = new COneTVertex;				// ���ο� �ؽ��� ���ؽ� ����
			t->m_u = Parsing_NumberFloat();					// ��ǥ�� �о��
			t->m_v = Parsing_NumberFloat();
			t->m_w = Parsing_NumberFloat();
			m_OneMesh->m_mesh_tvertex.push_back(t);			// ���Ϳ� ������
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
			break;

		case TOKENR_MESH_TFACELIST:
			break;


		case TOKENR_MESH_TFACE:
		{
			// face����, tface���� normalface ������ ������ ���� ���� �����Ǵ� ������ ������ �� ����
			// �� face�� ������ �ִ� vertex �ε����� Ȯ���ؼ� ���ο� ���ý��� ���� ���Ϳ� ������.
			int index = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[0] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[2] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[1] = Parsing_NumberInt();
		}
		break;
#pragma endregion

		case TOKEND_END:
			// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

			//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
			TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
			return;

			break;

			/// ���� �ƹ��͵� �ش����� ������
		default:
		{

		}
			// �ƹ��͵� ���� �ʴ´�.
			break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)

	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}


// int
int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}


// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG				token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}