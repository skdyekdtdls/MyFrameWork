#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
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
{
	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_AddRef(pTexture);
	}
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const _tchar* pModelFilePath, _fmatrix PivotMatrix)
{
	HANDLE		hFile = CreateFile(pModelFilePath,		// ���� ��ο� �̸��� ���
		GENERIC_READ,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		OPEN_EXISTING,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL

	_uint iFlag = 0;

	DWORD		dwByte = 0;

	_uint iAnimType = 0;
	//ReadUINT(iAnimType);

	FAILED_CHECK_RETURN(Ready_Bones(hFile, dwByte, nullptr), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Meshes(hFile, dwByte, static_cast<TYPE>(iAnimType), PivotMatrix), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Materials(hFile, dwByte), E_FAIL);

	CloseHandle(hFile);

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

HRESULT CModel::Ready_Meshes(HANDLE hFile, DWORD& dwByte, TYPE eType, _fmatrix PivotMatrix)
{
	return S_OK;
}

HRESULT CModel::Ready_Materials(HANDLE hFile, DWORD& dwByte)
{
	return S_OK;
}

HRESULT CModel::Ready_Bones(HANDLE hFile, DWORD& dwByte, CBone* pParent)
{
	CBone* pBone = CBone::Create(hFile, dwByte, pParent);
	NULL_CHECK_RETURN(pBone, E_FAIL);

	m_Bones.push_back(pBone);

	_uint iNumChildren = { 0 };
	//ReadUINT(iNumChildren);
	for (_uint i = 0; i < iNumChildren; ++i)
	{
		Ready_Bones(hFile, dwByte, pParent);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations(HANDLE hFile, DWORD& dwByte)
{
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CComponent* CModel::Clone(void* pArg)
{
	return nullptr;
}

void CModel::Free()
{
	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMtrlTexture)
			Safe_Release(pTexture);
	}
}
