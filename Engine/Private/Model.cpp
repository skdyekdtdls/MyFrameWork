#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Animation.h"
#include "GameObject.h"
CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
	ZeroMemory(m_iPrevAnimIndex, sizeof(_uint) * BODY_END);
	ZeroMemory(m_iCurrentAnimIndex, sizeof(_uint) * BODY_END);
	ZeroMemory(m_InterTimeAcc, sizeof(_double) * BODY_END);
}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eAnimType(rhs.m_eAnimType)
	, m_RootIndex(rhs.m_RootIndex)
	, m_RootMoveDistance(0.f)
	, m_PrevMoveDistance(0.f)
{
	for (size_t i = 0; i < BODY_END; ++i)
	{
		m_InterTimeAcc[i] = rhs.m_InterTimeAcc[i];
		m_iPrevAnimIndex[i] = rhs.m_iPrevAnimIndex[i];
		m_iCurrentAnimIndex[i] = rhs.m_iCurrentAnimIndex[i];
	}

	/* 애니메이션의 경우, 각 복제된 객체들마다 사용하는 시간과 키프레임들의 현재 인덱스를
	구분하여 사용해야할 필요가 있기때문에 깊은 복사. */
	for (size_t i = 0; i < BODY_END; ++i)
	{
		for (auto& pOriginalAnimation : rhs.m_Animations[i])
			m_Animations[i].push_back(pOriginalAnimation->Clone());
	}
	/* 각 복제된 객체들마다 서로 다른 형태의 뼈 상태를 만들어줘야했기때문에 깊은 복사를 했다. */
	for (auto& pOriginalBone : rhs.m_Bones)
	{
		m_Bones.push_back(pOriginalBone->Clone());
	}

	/* 데이터가 너무 크다. 어지간하면 무조건 얕은복사로가자. */
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}
}

CAnimation* CModel::GetAnimationByName(string strName, BODY eBody)
{
	for (auto& Animation : m_Animations[eBody])
	{
		if (0 == strcmp(Animation->GetName(), strName.c_str()))
		{
			return Animation;
		}
	}
	return nullptr;
}

#ifdef _DEBUG
void CModel::CoutRootNodePos()
{
	_float3 pos = m_Bones[m_RootIndex]->GetTranslation();
	cout << pos.x << "\t" << pos.y << "\t" << pos.z << endl;
}
#endif

_float3 CModel::GetPivotMatrixScale()
{
	_float3 scale;
	scale.x = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_PivotMatrix).r[0]));
	scale.y = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_PivotMatrix).r[1]));
	scale.z = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_PivotMatrix).r[2]));

	return scale;
}

CBone* CModel::GetBoneByName(string strName)
{
	for (auto& Bone : m_Bones)
	{
		if (0 == strcmp(Bone->GetName(), strName.c_str()))
			return Bone;
	}
	return nullptr;
}

void CModel::Set_AnimByIndex(_uint iAnimIndex, BODY eBody)
{
	if (iAnimIndex >= m_iNumAnimations)
		return;

	if (m_iCurrentAnimIndex[eBody] == iAnimIndex)
		return;

	m_Animations[eBody][iAnimIndex]->Reset();
	m_InterTimeAcc[eBody] = 0.0;
	m_iCurrentAnimIndex[eBody] = iAnimIndex;
}

void CModel::Set_AnimByName(const char* pName, BODY eBody)
{
	_int iIndex = 0;
	if (m_Animations[eBody].empty())
		return;

	for (auto& iter : m_Animations[eBody])
	{
		if (0 == strcmp(iter->m_szName, pName))
		{
			Set_AnimByIndex(iIndex, eBody);
			return;
		}
		++iIndex;
	}
#ifdef _DEBUG
	CONSOLE_MSG("Can't find the anim index by name" << __LINE__);
#endif
}

void CModel::Set_RootNode(_uint iBoneIndex)
{
	m_RootIndex = iBoneIndex;
}

void CModel::SetAnimationPlaySpeedByIndex(_double PlaySpeed, _uint iAnimIndex, BODY eBody)
{
	if (iAnimIndex >= m_iNumAnimations)
		return;

	m_Animations[eBody][iAnimIndex]->SetPlaySpeed(PlaySpeed);
}

void CModel::SaveAssimp(HANDLE hFile, DWORD& dwByte)
{
	/* For.Bones*/
	WriteVoid(&m_iNumBones, sizeof(_uint));
	for (size_t i = 0; i < m_iNumBones; ++i)
	{
		m_Bones[i]->SaveAssimp(hFile, dwByte);
	}

	// For. Meshes
	WriteVoid(&m_iNumMeshes, sizeof(_uint));
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		m_Meshes[i]->SaveAssimp(hFile, dwByte);
	}

	/* For.Materials */
	WriteVoid(&m_iNumMaterials, sizeof(_uint));
	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			if (nullptr == m_Materials[i].pMtrlTexture[j])
			{
				WriteEnable(false);
				continue;
			}
			WriteEnable(true);
			m_Materials[i].pMtrlTexture[j]->SaveAssimp(hFile, dwByte);
		}
	}

	/* For. Animations */
	WriteVoid(&m_iCurrentAnimIndex, sizeof(_uint));
	WriteVoid(&m_iNumAnimations, sizeof(_uint));
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		m_Animations[LOWER][i]->SaveAssimp(hFile, dwByte);
	}
}

void CModel::LoadAssimp(const char* pFileName)
{
#ifdef _DEBUG
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<double, std::milli> elapsed;
#endif // _DEBUG

	fs::path FileName = pFileName;
	fs::path Stem = FileName.stem();

	fs::path pModelFilePath = FindModelDirecotyPath("../../Resources/Skeletal_Mesh/", Stem);
	if (pModelFilePath == fs::path())
	{
		pModelFilePath = FindModelDirecotyPath("../../Resources/Static_Mesh/", Stem);
	}

	pModelFilePath = FindDATFile(pModelFilePath);

	HANDLE		hFile = CreateFile(pModelFilePath.wstring().c_str(),
		GENERIC_READ,
		NULL,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return;
	}
#ifdef _DEBUG
	cout << "OpenFile : " << pModelFilePath.string() << endl;
	start = std::chrono::high_resolution_clock::now();
#endif
	DWORD dwByte = 0;

	ReadVoid(&m_eAnimType, sizeof(m_eAnimType));

	/* For.Bones*/
	ReadVoid(&m_iNumBones, sizeof(_uint));
	m_Bones.resize(m_iNumBones);
	for (size_t i = 0; i < m_iNumBones; ++i)
	{
		m_Bones[i] = new CBone();
		m_Bones[i]->LoadAssimp(hFile, dwByte);
	}

	// For. Meshes
	ReadVoid(&m_iNumMeshes, sizeof(_uint));
	m_Meshes.resize(m_iNumMeshes);
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		m_Meshes[i] = new CMesh(m_pDevice, m_pContext);
		m_Meshes[i]->LoadAssimp(hFile, dwByte, m_eAnimType, XMLoadFloat4x4(&m_PivotMatrix));
	}

	/* For.Materials */
	ReadVoid(&m_iNumMaterials, sizeof(_uint));
	m_Materials.resize(m_iNumMaterials);
	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			if (false == ReadEnable())
				continue;
			m_Materials[i].pMtrlTexture[j] = new CTexture(m_pDevice, m_pContext);
			m_Materials[i].pMtrlTexture[j]->LoadAssimp(hFile, dwByte, m_eAnimType);
		}
	}

	/* For. Animations */
	ReadVoid(&m_iCurrentAnimIndex, sizeof(_uint));
	ReadVoid(&m_iNumAnimations, sizeof(_uint));
	m_Animations[LOWER].resize(m_iNumAnimations);
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		m_Animations[LOWER][i] = new CAnimation();
		m_Animations[LOWER][i]->LoadAssimp(hFile, dwByte);
	}

	CloseHandle(hFile);
#ifdef _DEBUG
	end = std::chrono::high_resolution_clock::now();
	elapsed = end - start;
	std::cout << "Load Time Elapse : " << elapsed.count() << " ms\n";
#endif // _DEBUG
}

HRESULT CModel::Initialize_Prototype(const aiScene* pAIScene, TYPE eType, fs::path pModelFilePath, _fmatrix PivotMatrix)
{
	_uint		iFlag = 0;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
	m_eAnimType = eType;
	if (FAILED(Ready_Bones(pAIScene->mRootNode, nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(pAIScene, eType, PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pAIScene, pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations(pAIScene)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

HRESULT CModel::Late_Initialize(const _tchar* pAnimFilePath)
{
	if (nullptr == pAnimFilePath)
		return S_OK;

	HANDLE		hFile = CreateFile(pAnimFilePath, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}

	DWORD dwByte = 0;

	for (auto& pAnimation : m_Animations[LOWER])
		pAnimation->LoadData(hFile, dwByte);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Play_Animation(_double TimeDelta, BODY eBody)
{
	if (m_eAnimType == TYPE_NONANIM)
		return;

	if (m_Animations[eBody].empty())
		return;

	_uint iCurrentAnimIndex = m_iCurrentAnimIndex[eBody];
	_uint iPrevAnimIndex = m_iPrevAnimIndex[eBody];

	// 루프애니메이션이 아닐 경우 NextAnimtion으로 넘어감.
	//if (false == m_Animations[eBody][iCurrentAnimIndex]->m_isLoop)
	//{
	//	if (m_Animations[eBody][iCurrentAnimIndex]->IsFinished())
	//	{
	//		_uint iNextAnimIndex = m_Animations[eBody][iCurrentAnimIndex]->m_iNextIndex;
	//		if (-1 != iNextAnimIndex)
	//		{
	//			Set_AnimByIndex(iNextAnimIndex, eBody);
	//		}
	//	}
	//}

	// 만약 애니메이션의 변경이 일어나면 보간 시작
	if (iPrevAnimIndex != iCurrentAnimIndex)
	{
		m_InterTimeAcc[eBody] += TimeDelta;

		if (m_InterTimeAcc[eBody] > 0.2) // 보간이 끝나면 현재와 이전의 애니메이션 인덱스 세팅
		{	
			m_Animations[eBody][iPrevAnimIndex]->m_isFinished = true;
			m_iPrevAnimIndex[eBody] = iCurrentAnimIndex;
			m_Animations[eBody][iCurrentAnimIndex]->Reset();
		}
		else // 보간 
		{
			m_Animations[eBody][iCurrentAnimIndex]->InterAnimation_TransfomationMatrix(m_Bones, m_InterTimeAcc[eBody], eBody);
		}
	}
	else // 채널 상태 갱신
	{
		m_Animations[eBody][iCurrentAnimIndex]->Invalidate_TransformationMatrix(m_Bones, TimeDelta, eBody);
	}

	if (LOWER == eBody)
	{
		if (-1 != m_RootIndex)
		{
			m_RootMoveDistance = m_Bones[m_RootIndex]->GetTransformationMatrix_43();
			m_Bones[m_RootIndex]->FixBone();
		}
	}
	
	for (auto& Bone : m_Bones)
	{
		Bone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}
}

_bool CModel::IsAnimationFinished(BODY eBody)
{
	if (m_Animations[eBody][m_iCurrentAnimIndex[eBody]]->IsFinished())
	{
		m_PrevMoveDistance = { 0.f };
		m_RootMoveDistance = { 0.f };
		return true;
	}

	return false;
}

void CModel::GroupingBones(vector<_uint>& upper, vector<_uint>& lower)
{
	if (upper.empty())
	{
		for (auto& Bone : m_Bones)
		{
			Bone->Set_Body(LOWER);
		}
		return;
	}

	for (auto& Bone : m_Bones)
	{
		CBone* pBone = Bone;
		_uint iBoneIndex = pBone->GetIndex();
		_bool isContinue = { false };

		// 뼈가 ik_hand_root(7)이거나 spine_01(12)이면 상체
		for (auto& upperIndex : upper)
		{
			if (upperIndex == iBoneIndex)
			{
				Bone->Set_Body(UPPER);
				isContinue = true;
				break;
			}
		}
		if (isContinue)
			continue;

		// 뼈가 ik_foot_root(4)이거나 thigh_l(88) 이거나 thigh_r(94)이면 하체
		for (auto& lowerIndex : lower)
		{
			if (lowerIndex == iBoneIndex)
			{
				Bone->Set_Body(LOWER);
				isContinue = true;
				break;
			}
		}
		if (isContinue)
			continue;

		while (pBone->HasParent()) // 부모 뼈가 있으면 루프
		{
			_uint iParentIndex = pBone->GetParentIndex();
			_bool isBreak = { false };

			// 부모가 상체면 자식도 상체
			for (auto& upperIndex : upper)
			{
				if (upperIndex == iParentIndex)
				{
					Bone->Set_Body(UPPER);
					isBreak = true;
					break;
				}
			}
			if (isBreak)
				break;

			// 부모가 ik_foot_root(4)이거나 thigh_l(88) 이거나 thigh_r(94)이면 하체
			for (auto& lowerIndex : lower)
			{
				if (lowerIndex == iParentIndex)
				{
					Bone->Set_Body(LOWER);
					isBreak = true;
					break;
				}
			}
			if (isBreak)
				break;

			pBone = m_Bones[iParentIndex];
		}
	}

	for (auto& pAnimation : m_Animations[LOWER])
		m_Animations[UPPER].push_back(pAnimation->Clone());
}

void CModel::ResetAnimation(_int iIndex, BODY eBody)
{
	if (iIndex < 0)
	{
		m_Animations[eBody][m_iCurrentAnimIndex[eBody]]->Reset();
		return;
	}

	m_Animations[eBody][iIndex]->Reset();
}

void CModel::RootMotion(_double TimeDelta, CTransform::DIRECTION eDir)
{
	CTransform* pTransform = m_pOwner->GetComponent<CTransform>();
	_float fLength = -m_RootMoveDistance - -m_PrevMoveDistance;

	// 길이를 구해줘야한다.
	pTransform->Go_Direction(TimeDelta, eDir, fLength);

	m_PrevMoveDistance = m_RootMoveDistance;
}

void CModel::RootMotion(_double TimeDelta, _fvector vDir)
{
	CTransform* pTransform = m_pOwner->GetComponent<CTransform>();
	_float fLength = -m_RootMoveDistance - -m_PrevMoveDistance;

	// 길이를 구해줘야한다.
	pTransform->Go_Direction(TimeDelta, vDir, fLength);

	m_PrevMoveDistance = m_RootMoveDistance;
}

// 뼈오프셋 * 뼈 컴바인 * 모델피벗에 대한 메트릭스(내부적으로 0,1,2행 노말라이즈)를 반환
_matrix CModel::GetBoneOCPMatrix(_uint iBoneIndex)
{
	if (iBoneIndex > m_Bones.size())
		return _matrix();

	_matrix AttachingBoneMatrix;

	CBone* pBone = m_Bones[iBoneIndex];
	_matrix PivotMatirx = XMLoadFloat4x4(&m_PivotMatrix);
	_matrix OffsetMatrix = pBone->Get_OffsetMatrix();
	_float4x4 CombinedFloat4x4 = pBone->Get_CombinedTransformationMatrix();
	_matrix CombinedMatrix = XMLoadFloat4x4(&CombinedFloat4x4);
	
	AttachingBoneMatrix = OffsetMatrix * CombinedMatrix * PivotMatirx;
	
	AttachingBoneMatrix.r[0] = XMVector3Normalize(AttachingBoneMatrix.r[0]);
	AttachingBoneMatrix.r[1] = XMVector3Normalize(AttachingBoneMatrix.r[1]);
	AttachingBoneMatrix.r[2] = XMVector3Normalize(AttachingBoneMatrix.r[2]);

	return AttachingBoneMatrix;
}

HRESULT CModel::Bind_Material(CShader* pShader, const char* pConstantName, _uint iMeshIndex, TextureType MaterialType)
{
	if (iMeshIndex >= m_iNumMeshes ||
		MaterialType >= AI_TEXTURE_TYPE_MAX)
		return E_FAIL;

	if (m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMtrlTexture[MaterialType] == nullptr) return S_OK;

	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMtrlTexture[MaterialType]->Bind_ShaderResource(pShader, pConstantName);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const char* pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[256];
	ZeroMemory(BoneMatrices, sizeof(_float4x4) * 256);

	/* iMeshIndex번째 메시가 사용하느 ㄴ뼈들의 행려을 가져와서 BoneMatrices에 넣어준다. */
	m_Meshes[iMeshIndex]->Get_Matrices(m_Bones, BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

	pShader->Bind_Matrices(pConstantName, BoneMatrices, 256);

	return S_OK;
}

HRESULT CModel::Add_TimeLineEvent(string strAnimName, const _tchar* pTag, TIMELINE_EVENT tTimeLineEvent, BODY eBody)
{
	CAnimation* pAnimation = GetAnimationByName(strAnimName, eBody);
	if (nullptr == pAnimation)
		return E_FAIL;

	pAnimation->Add_TimeLineEvent(pTag, tTimeLineEvent);

	return S_OK;
}

void CModel::Delete_TimeLineEvent(string strAnimName, const _tchar* pTag, BODY eBody)
{
	CAnimation* pAnimation = GetAnimationByName(strAnimName);
	if (nullptr == pAnimation)
		return;

	pAnimation->Delete_TimeLineEvent(pTag);
}

const TIMELINE_EVENT* CModel::Get_TimeLineEvent(string strAnimName, const _tchar* pTag, BODY eBody)
{
	CAnimation* pAnimation = GetAnimationByName(strAnimName);

	if (nullptr == pAnimation)
		return nullptr;

	return pAnimation->Get_TimeLineEvent(pTag);
}

_bool CModel::IsCurAnimTimeAccGreaterThan(_double Time, BODY eBody)
{
	if (eBody >= BODY_END)
		return _bool();

	return m_Animations[eBody][m_iCurrentAnimIndex[eBody]]->GetTimeAcc() >= Time;
}

_bool CModel::IsCurAnimTimeAccLessThan(_double Duration, BODY eBody)
{
	return !IsCurAnimTimeAccGreaterThan(Duration, eBody);
}

HRESULT CModel::Ready_Bones(aiNode* pNode, CBone* pParent)
{
	CBone* pBone = CBone::Create(pNode, pParent, m_Bones.size());

	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	for (size_t i = 0; i < pNode->mNumChildren; ++i)
	{
		Ready_Bones(pNode->mChildren[i], pBone);
	}
	m_iNumBones = m_Bones.size();
	return S_OK;
}

HRESULT CModel::Ready_Meshes(const aiScene* pScene, TYPE eType, _fmatrix PivotMatrix)
{
	m_iNumMeshes = pScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, eType, m_Bones, pScene->mMeshes[i], PivotMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

fs::path CModel::find_file_in_dir(const fs::path& dir_path, const fs::path& file_name) {
	fs::recursive_directory_iterator dir(dir_path), end;

	while (dir != end)
	{
		if (dir->path().filename() == file_name)
		{
			return dir->path();
			std::cout << "File found: " << dir->path() << '\n';
			break;
		}
		++dir;
	}
	return fs::path();
}

HRESULT CModel::Ready_Materials(const aiScene* pScene, fs::path pModelFilePath)
{
	/* 현재 모델에게 부여할 수 있는 재질(Diffuse, Normal, Specular etc) 텍스쳐의 갯수. */
	m_iNumMaterials = pScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MESHMATERIAL			MeshMaterial;
		ZeroMemory(&MeshMaterial, sizeof MeshMaterial);

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			aiString	strPath;

			if (FAILED(pScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			//char		szDrive[MAX_PATH] = "";
			//char		szDirectory[MAX_PATH] = "";
			//_splitpath_s(pModelFilePath.string().c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
			////_splitpath_s(strPath.data, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szStem[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szStem, MAX_PATH, szExt, MAX_PATH);

			char		szFileName[MAX_PATH] = "";
			//strcpy_s(szFullPath, szDrive);
			//strcat_s(szFullPath, szDirectory);
			strcat_s(szFileName, szStem);
			strcat_s(szFileName, szExt);

			fs::path fsFileName = szFileName;
			fs::path base_dir = "C:\\KillSquad\\Game\\";
			fs::path fsFullPath = find_file_in_dir(base_dir, fsFileName);

			_tchar		wszFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFileName, strlen(szFileName),
				wszFullPath, MAX_PATH);

			MeshMaterial.pMtrlTexture[j] = CTexture::Create(m_pDevice, m_pContext, fsFullPath.wstring().c_str(), 1);
			if (nullptr == MeshMaterial.pMtrlTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(MeshMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(const aiScene* pScene)
{
	m_iNumAnimations = pScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(pScene->mAnimations[i], m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations[LOWER].push_back(pAnimation);
	}

	return S_OK;
}

fs::path CModel::FindModelDirecotyPath(fs::path ModelPath, fs::path Stem)
{
	fs::directory_iterator CurrentPathIter(ModelPath);

	while (CurrentPathIter != fs::end(CurrentPathIter))
	{
		const fs::directory_entry& CurrentPathEntry = *CurrentPathIter;
		// Static_Mesh
		if (Stem == CurrentPathEntry.path().filename().stem())
		{
			return CurrentPathEntry.path();
		}

		++CurrentPathIter;
	}

	return fs::path();
}

fs::path CModel::FindDATFile(fs::path ModelPath)
{
	fs::directory_iterator CurrentPathIter(ModelPath);

	while (CurrentPathIter != fs::end(CurrentPathIter))
	{
		const fs::directory_entry& CurrentPathEntry = *CurrentPathIter;
		// Static_Mesh
		if (".dat" == CurrentPathEntry.path().extension() || ".DAT" == CurrentPathEntry.path().extension())
		{
			return CurrentPathEntry.path();
		}

		++CurrentPathIter;
	}

	return fs::path();
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix, _int RootIndex)
{
	CModel* pModel = new CModel(pDevice, pContext);
	pModel->Set_PivotMatrix(PivotMatrix);
	pModel->Set_RootNode(RootIndex);
	return pModel;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiScene* pAIScene, TYPE eType, const _tchar* pModelFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIScene, eType, pModelFilePath, PivotMatrix)))
	{
		MSG_BOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CModel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_Release(pTexture);
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (size_t i = 0; i < BODY_END; ++i)
	{
		for (auto& pAnimation : m_Animations[i])
			Safe_Release(pAnimation);

		m_Animations[i].clear();
	}

}
