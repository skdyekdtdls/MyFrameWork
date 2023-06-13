#include "..\public\ModelPaser.h"
#include "MeshPaser.h"
#include "BonePaser.h"

HRESULT CModelPaser::Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType)
{

	// ���� ����
	HANDLE		hFile = CreateFile(L"../Bin/Line.txt",		// ���� ��ο� �̸��� ���
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
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	// Ready_Bone
	m_pBonePaser->Pasing(hFile, dwByte, m_pAIScene->mRootNode);

	// Ready_Mesh
	WriteUINT(m_pAIScene->mNumMeshes);
	for (_uint i = 0; i < m_pAIScene->mNumMeshes; ++i)
	{
		m_pMeshPaser->Pasing(hFile, m_pAIScene->mMeshes[i], dwByte, eAnimType, m_pBonePaser->m_BoneNames);
	}

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
	
	return S_OK;
}

void CModelPaser::Relase_Paser()
{
	Safe_Delete(m_pBonePaser);
	Safe_Delete(m_pMeshPaser);
}
