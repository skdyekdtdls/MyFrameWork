#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
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
{
	/* 애니메이션의 경우, 각 복제된 객체들마다 사용하는 시간과 키프레임들의 현재 인덱스를
	구분하여 사용해야할 필요가 있기때문에 깊은 복사. */
	for (auto& pOriginalAnimation : rhs.m_Animations)
		m_Animations.push_back(pOriginalAnimation->Clone());

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
		m_Animations[i]->SaveAssimp(hFile, dwByte);
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
	m_Animations.resize(m_iNumAnimations);
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		m_Animations[i] = new CAnimation();
		m_Animations[i]->LoadAssimp(hFile, dwByte);
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
	
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Play_Animation(_double TimeDelta)
{
	if (TYPE_NONANIM == m_eAnimType)
		return;

	/* 어떤 애니메이션을 재생하려고하는지?! */
	/* 이 애니메이션은 어떤 뼈를 사용하는지?! */
	/* 뼈들은 각각 어떤 상태(TransformationMatrix)를 취하고 있어야하는가?! */

	/* 현재 애니메이션에서 사용하는 뼈들을 찾아서 해당 뼈들의 TransformationMatrix를 갱신한다. */
	m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(m_Bones, TimeDelta);

	/* 모델에 표현되어있는 모든 뼈들의 CombinedTransformationMatrix */
	for (auto& pBone : m_Bones)
	{
		pBone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}
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

			char		szDrive[MAX_PATH] = "";
			char		szDirectory[MAX_PATH] = "";
			_splitpath_s(pModelFilePath.string().c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szFileName[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			char		szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			
			_tchar		wszFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath),
				wszFullPath, MAX_PATH);

			MeshMaterial.pMtrlTexture[j] = CTexture::Create(m_pDevice, m_pContext, wszFullPath, 1);
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

		m_Animations.push_back(pAnimation);
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

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix)
{
	CModel* pModel = new CModel(pDevice, pContext);
	pModel->Set_PivotMatrix(PivotMatrix);
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

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();
}
