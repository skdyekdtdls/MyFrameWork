#include "..\public\Paser.h"

HRESULT Paser::Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType)
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
	
	Assimp::Importer	m_Importer; /* ���� ��η� ������ ������ �а� �����Ѵ�.*/
	const aiScene* m_pAIScene = { nullptr };

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	NODE::Serialization(m_pAIScene->mRootNode, hFile, dwByte);
	//WriteVoid(&eAnimType, sizeof(eAnimType));

	//Scene.Serialization(m_pAIScene, hFile, dwByte);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT Paser::Load(const char* pModelFilePath)
{
	// ���� ����
	HANDLE		hFile = CreateFile(L"../Bin/Fiona.dat",		// ���� ��ο� �̸��� ���
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

	ANIM_TYPE eAnimType;

	//ReadVoid(&eAnimType, sizeof(eAnimType));
	 
	NODE a;
	a.Deserialization(&a, hFile, dwByte);
	
	//SCENE* Scene = new SCENE;
	//SCENE::Deserialization(Scene, hFile, dwByte);

	//Safe_Delete(Scene);
	CloseHandle(hFile);
	return S_OK;
}