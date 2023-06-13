#include "..\public\ModelPaser.h"
#include "MeshPaser.h"
#include "BonePaser.h"
#include "MaterialPaser.h"

HRESULT CModelPaser::Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType)
{

	// ���� ����
	HANDLE		hFile = CreateFile(L"../Bin/Fiona.dat",		// ���� ��ο� �̸��� ���
		GENERIC_WRITE,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		CREATE_ALWAYS,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}

	HRESULT hr = { 0 };
	DWORD		dwByte = 0;

	_uint		iFlag = 0;

	if (ANIM_TYPE::NONANIM == eAnimType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	WriteUINT(static_cast<_uint>(eAnimType));

	// Ready_Bone
	m_pBonePaser->Pasing(hFile, dwByte, m_pAIScene->mRootNode);

	// Ready_Mesh
	m_pMeshPaser->Pasing(hFile, dwByte, m_pAIScene, eAnimType, m_pBonePaser->m_BoneNames);

	// Ready_Material
	m_pMaterialPaser->Pasing(hFile, dwByte, m_pAIScene, pModelFilePath);

	CloseHandle(hFile);

	//HANDLE		hFile = CreateFile(L"../Bin/Line.txt",		// ���� ��ο� �̸��� ���
	//	GENERIC_READ,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
	//	NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
	//	NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
	//	OPEN_EXISTING,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
	//	FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
	//	NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL

	//_uint iFlag = 0;
	//HRESULT hr = { 0 };
	//DWORD		dwByte = 0;
	//m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	//_float3 f;
	//ReadFloat3(f);
	//CloseHandle(hFile);

	return S_OK;
}

HRESULT CModelPaser::Ready_Paser()
{
	m_pBonePaser = new CBonePaser;
	m_pMeshPaser = new CMeshPaser;
	m_pMaterialPaser = new CMaterialPaser;
	return S_OK;
}

void CModelPaser::Relase_Paser()
{
	Safe_Delete(m_pBonePaser);
	Safe_Delete(m_pMeshPaser);
	Safe_Delete(m_pMaterialPaser);
}
