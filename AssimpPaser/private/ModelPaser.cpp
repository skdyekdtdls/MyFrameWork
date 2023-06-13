#include "..\public\ModelPaser.h"
#include "MeshPaser.h"
#include "BonePaser.h"

HRESULT CModelPaser::Pasing(ANIM_TYPE eModelType, const char* pModelFilePath)
{

	// ���� ����
	HANDLE		hFile = CreateFile(L"../Bin/Line.dat",		// ���� ��ο� �̸��� ���
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
	

	//m_pBonePaser->Pasing(hFile, dwByte, m_pAIScene->mRootNode,);

	//WriteCHAR(hFile, a->mName.data, dwByte);

	CloseHandle(hFile);

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
