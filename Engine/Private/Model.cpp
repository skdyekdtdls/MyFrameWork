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
	HANDLE		hFile = CreateFile(pModelFilePath,		// 파일 경로와 이름을 명시
		GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		OPEN_EXISTING,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL

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
