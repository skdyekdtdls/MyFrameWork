#include "..\public\Paser.h"
#include "GameInstance.h"

HRESULT CPaser::Pasing(const char* pModelFilePath)
{
	// �����̸� �����ͼ� ��θ����.
	// ���� ����
	HANDLE		hFile = CreateFile(L"../../Resources/Models/Fiona/Fiona.dat",		// ���� ��ο� �̸��� ���
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
	CModel::TYPE eAnimType = CModel::TYPE::TYPE_ANIM; //assert(false); // �ڵ����� Ÿ�԰�������

	WriteFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);

	if (CModel::TYPE::TYPE_NONANIM == eAnimType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	
	Assimp::Importer	m_Importer; /* ���� ��η� ������ ������ �а� �����Ѵ�.*/
	const aiScene* m_pAIScene = { nullptr };
	
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	SCENE::Serialization(m_pAIScene, hFile, dwByte);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CPaser::Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene)
{
	// ���� ����
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

	CModel::TYPE eAnimType;
	ReadFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);
	tScene.Deserialization(hFile, dwByte);

	//Safe_Delete(Scene);
	CloseHandle(hFile);
	return S_OK;
}