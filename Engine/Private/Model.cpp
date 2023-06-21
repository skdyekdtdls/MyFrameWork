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
	/* �ִϸ��̼��� ���, �� ������ ��ü�鸶�� ����ϴ� �ð��� Ű�����ӵ��� ���� �ε�����
	�����Ͽ� ����ؾ��� �ʿ䰡 �ֱ⶧���� ���� ����. */
	for (auto& pOriginalAnimation : rhs.m_Animations)
		m_Animations.push_back(pOriginalAnimation->Clone());

	/* �� ������ ��ü�鸶�� ���� �ٸ� ������ �� ���¸� ���������߱⶧���� ���� ���縦 �ߴ�. */
	for (auto& pOriginalBone : rhs.m_Bones)
	{
		m_Bones.push_back(pOriginalBone->Clone());
	}

	/* �����Ͱ� �ʹ� ũ��. �������ϸ� ������ ��������ΰ���. */
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}
}

HRESULT CModel::Initialize_Prototype(const _tchar* pModelFilePath, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	SCENE tScene;

	FAILED_CHECK_RETURN(LoadModel(pModelFilePath, tScene), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Bones(tScene.m_RootNode, nullptr), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Meshes(tScene, PivotMatrix), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Materials(&tScene, pModelFilePath), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Animations(tScene), E_FAIL);

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

	/* � �ִϸ��̼��� ����Ϸ����ϴ���?! */
	/* �� �ִϸ��̼��� � ���� ����ϴ���?! */
	/* ������ ���� � ����(TransformationMatrix)�� ���ϰ� �־���ϴ°�?! */

	/* ���� �ִϸ��̼ǿ��� ����ϴ� ������ ã�Ƽ� �ش� ������ TransformationMatrix�� �����Ѵ�. */
	m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(m_Bones, TimeDelta);

	/* �𵨿� ǥ���Ǿ��ִ� ��� ������ CombinedTransformationMatrix */
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

	/* iMeshIndex��° �޽ð� ����ϴ� �������� ����� �����ͼ� BoneMatrices�� �־��ش�. */
	m_Meshes[iMeshIndex]->Get_Matrices(m_Bones, BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

	pShader->Bind_Matrices(pConstantName, BoneMatrices, 256);

	return S_OK;
}


HRESULT CModel::Ready_Meshes(const SCENE& tScene, _fmatrix PivotMatrix)
{
	m_iNumMeshes = tScene.m_NumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eAnimType, m_Bones, &tScene.m_Meshes[i], PivotMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

HRESULT CModel::LoadModel(const _tchar* pModelFilePath, SCENE& tScene)
{
	HANDLE		hFile = CreateFile(pModelFilePath,		// ���� ��ο� �̸��� ���
		GENERIC_READ,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		OPEN_EXISTING,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}
	DWORD dwByte = { 0 };
	//SCENE Scene;

	ReadVoid(&m_eAnimType, sizeof(m_eAnimType));
	
	tScene.Deserialization(hFile, dwByte);
	
	CloseHandle(hFile);
}

HRESULT CModel::Ready_Materials(const SCENE* pScene, const _tchar* pwszModelFilePath)
{
	char pModelFilePath[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, pwszModelFilePath, MAX_PATH, pModelFilePath, MAX_PATH, nullptr, nullptr);
	/* ���� �𵨿��� �ο��� �� �ִ� ����(Diffuse, Normal, Specular etc) �ؽ����� ����. */
	m_iNumMaterials = pScene->m_NumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MESHMATERIAL			MeshMaterial;
		ZeroMemory(&MeshMaterial, sizeof MeshMaterial);

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			AI_STRING	strPath;

			if (false == pScene->m_Materials[i].GetTexture(TextureType(j), 0, &strPath))
				continue;

			char		szDrive[MAX_PATH] = "";
			char		szDirectory[MAX_PATH] = "";
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szFileName[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";
			_splitpath_s(strPath.m_data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

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

HRESULT CModel::Ready_Bones(const NODE* pNode, CBone* pParent)
{
	CBone* pBone = CBone::Create(pNode, pParent, m_Bones.size());
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	for (size_t i = 0; i < pNode->m_NumChildren; ++i)
	{
		Ready_Bones(&pNode->m_Children[i], pBone);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(const SCENE& tScene)
{
	m_iNumAnimations = tScene.m_NumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(&tScene.m_Animations[i], nullptr, m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pModelFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PivotMatrix)))
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
