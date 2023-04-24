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
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	meshCount = 0;

	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(Mesh* pMesh)
{
/*
	// 버텍스들은 일단 모두 복사
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		Vertex* _pVertex = new Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
	unsigned int _faceCount = pMesh->m_meshface.size();
	TRACE("FaceCount : %d", _faceCount);

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face기준으로 버텍스의 노말을 넣어준다.
			// (예시용)
			_nowVertex->m_normal.x = 0;
			_nowVertex->m_normal.y = 0;
			_nowVertex->m_normal.z = 0;
		}
	}

	// 인덱스는 그냥 복사
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
/// 버텍스와 텍스쳐 버텍스를 합쳐주는 곳.
/// 
/// face의 인덱스를 참고하면서 해당하는 버텍스에 텍스쳐 좌표를 추가해줌.
/// </summary>
Mesh*& CASEParser::CreateVertexList(Mesh*& mesh)
{
	const auto& face = mesh->m_meshface;			// 메쉬의 face
	const auto& vertex = mesh->m_meshvertex;
	const auto& tvertex = mesh->m_mesh_tvertex;		// 메쉬의 t
	mesh->istexture = tvertex.empty();

	// 텍스쳐 좌표가 없는 메쉬면 넘어감
	for (size_t i = 0; i < face.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int v1 = face[i]->m_vertexindex[j];			// face에서 버텍스의 인덱스를 가져옴
			Vector3& n = face[i]->m_normalvertex[j];	// face에서 노말 값을 가져옴

			Vertex* v = new Vertex;
			const Vertex& vt = *vertex[v1];
			v->m_pos = vt.m_pos;				// 페이스에 저장된 인덱스에 있는 pos를 가져옴
			v->m_normal = n;					// 페이스에 저장된 normal을 가져옴
			if (mesh->istexture)				// texture 없을수도 있지?
			{
				v->u = 0.f;
				v->v = 0.f;
			}
			else
			{
				int t1 = face[i]->m_TFace[j];				// face에서 텍스쳐 좌표를 가져옴
				v->u = tvertex[t1]->m_u;
				v->v = tvertex[t1]->m_v;
			}

			/// 해당 버텍스가 버퍼 안에 있는지 확인
			/// 없으면 버퍼에 버텍스를 넣고 인덱스 버퍼에 인덱스를 넣고
			/// 인덱스를 증가시킴.
			/// 
			/// 해당 버텍스가 버퍼 안에 있으면
			/// 해당 인덱스를 가져와서 인덱스 버퍼에 인덱스를 넣음. 

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

	auto itr = vertexMap.find(temp);			// 맵에서 버텍스를 찾음
	if (itr != vertexMap.end())					// 버텍스를 찾게 된다면
	{
		delete v;
		return vertexMap[temp];					// 해당 인덱스 반환
	}
	else										// 버텍스를 찾지 못한다면
	{
		vertexMap[*v] = size;					// 해당 버텍스를 맵에 넣고
		mesh->m_rawVertex.push_back(v);			// 버텍스 배열에 포인터 넣음.
		return size;							// 인덱스 반환
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
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;
	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

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
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
			break;

			//--------------------
			// SCENE
			//--------------------
#pragma region SCENE
		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
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
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
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
		{}			// 없음
		break;
			

		case TOKENR_GEOMOBJECT:
		{
			/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?
			meshCount++;
			Create_onemesh_to_list();
			m_OneMesh->m_type = eGeomobject;
		}
		break;
		
		case TOKENR_NODE_NAME:
			// 어쩄든 지금은 오브젝트들을 구별 할 수 있는 유일한 값이다.
			// 모드에 따라 넣어야 할 곳이 다르다.
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
			// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
			// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

			/// 게다가,
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
			// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
			// 재귀구조의 문제점이다....

			break;

			
		case TOKENR_INHERIT_POS:
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
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
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
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
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
			// 내용 입력
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
			/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
			// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
			// Bone의 넘버를 읽어 주자
			ASEParser::Bone* b = new ASEParser::Bone;
			m_OneMesh->m_vector_bone_list.push_back(b);
			m_OneMesh->m_is_skinningobject = true;
		}
		break;
		//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
		case TOKENR_BONE_NAME:
		{

		}
		break;
		case TOKENR_BONE_PROPERTY:
			// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
			break;
			// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// 버텍스 하나의 정보를 만들어서 리스트에 넣음
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			// 대체 몇 단계를 들어가는거야...
			// 가중치 한개를 만들어서 리스트에 넣는다
			/// 헥 헥....
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
			// Face의 번호인데...
			Parsing_String();
			// A:를 읽고
			Parsing_String();
			f->m_vertexindex[0] = Parsing_NumberInt();
			// B:
			Parsing_String();
			f->m_vertexindex[2] = Parsing_NumberInt();
			// C:
			Parsing_String();
			f->m_vertexindex[1] = Parsing_NumberInt();

			/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
			/// ......

			// 벡터에 넣어준다.
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
			// 버텍스의 인덱스가 나오는데 어차피 순서와 같으므로 버린다.
			// 새로운 TVertex를 만들어서 벡터에 넣는다
			int index = Parsing_NumberInt();
			COneTVertex* t = new COneTVertex;				// 새로운 텍스쳐 버텍스 생성
			t->m_u = Parsing_NumberFloat();					// 좌표를 읽어옴
			t->m_v = Parsing_NumberFloat();
			t->m_w = Parsing_NumberFloat();
			m_OneMesh->m_mesh_tvertex.push_back(t);			// 벡터에 저장함
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
			break;

		case TOKENR_MESH_TFACELIST:
			break;


		case TOKENR_MESH_TFACE:
		{
			// face개수, tface개수 normalface 개수가 같은걸 보니 서로 대응되는 관계라고 유추할 수 있음
			// 각 face가 가지고 있는 vertex 인덱스를 확인해서 새로운 버택스를 만들어서 벡터에 저장함.
			int index = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[0] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[2] = Parsing_NumberInt();
			m_OneMesh->m_meshface[index]->m_TFace[1] = Parsing_NumberInt();
		}
		break;
#pragma endregion

		case TOKEND_END:
			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
			return;

			break;

			/// 위의 아무것도 해당하지 않을때
		default:
		{

		}
			// 아무것도 하지 않는다.
			break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)

	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
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
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
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


// 3개의 Float를 벡터 하나로
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

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	Mesh* temp = new Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEMaterial* temp = new ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	Animation* temp = new Animation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	Vertex* temp = new Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}