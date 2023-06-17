#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
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
	, m_Animations(rhs.m_Animations)
	, m_PivotMatrix(rhs.m_PivotMatrix)
{
	for (auto& pOriginalAnimation : rhs.m_Animations)
		m_Animations.push_back(pOriginalAnimation->Clone());

	for (auto& pOriginalBone : rhs.m_Bones)
		m_Bones.push_back(pOriginalBone->Clone());

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
	SCENE tScene;
	FAILED_CHECK_RETURN(LoadModel(pModelFilePath, tScene), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Bones(tScene.m_RootNode, nullptr), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Meshes(tScene, PivotMatrix), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Materials(tScene), E_FAIL);	
	FAILED_CHECK_RETURN(Ready_Animations(tScene), E_FAIL);	
	
	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	return S_OK;
}

void CModel::Play_Animation(_double TimeDelta)
{
}

HRESULT CModel::Bind_Material(CShader* pShader, const char* pConstantName, _uint iMeshIndex, TextureType MaterialType)
{
	return S_OK;
}

HRESULT CModel::Bind_BoneMatrices(CShaer* pShader, const char* pConstantName, _uint iMeshIndex)
{
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
		return E_FAIL;
	}

	_uint iFlag = 0;

	int a = 0;
	DWORD		dwByte = 0;

	ReadVoid(&m_eAnimType, sizeof(m_eAnimType));
	tScene.Deserialization(hFile, dwByte);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CModel::Ready_Meshes(const SCENE& tScene, _fmatrix PivotMatrix)
{
	return S_OK;
}

HRESULT CModel::Ready_Materials(const SCENE& tScene)
{
	return S_OK;
}

HRESULT CModel::Ready_Bones(const NODE* pNode, CBone* pParent)
{
	CBone* pBone = CBone::Create(pNode, pParent, m_Bones.size());
	NULL_CHECK_RETURN(pBone, E_FAIL);

	m_Bones.push_back(pBone);
	
	for (_uint i = 0; i < pNode->m_NumChildren; ++i)
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
		CAnimation* pAnimation = CAnimation::Create(&tScene.m_Animations[i], m_Bones);
		NULL_CHECK_RETURN(pAnimation, E_FAIL);

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
