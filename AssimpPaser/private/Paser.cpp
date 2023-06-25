#include "..\public\Paser.h"


HRESULT CPaser::Pasing(fs::path ModelFilePath, CModel::TYPE& Out)
{
	cout << "--------Searched .fbx file--------" << endl
		<< "Path : " << ModelFilePath.string() << endl;

	fs::path directoryPath = ModelFilePath.parent_path();
	fs::path stem = ModelFilePath.stem();
	fs::path header = L".dat";
	string fileName = stem.string() + header.string();
	fs::path wstrModelPath = directoryPath / fileName;

	// �����̸� �����ͼ� ��θ����.
	// ���� ����
	HANDLE		hFile = CreateFile(wstrModelPath.c_str(),		// ���� ��ο� �̸��� ���
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
	Assimp::Importer	m_Importer;
	const aiScene* m_pAIScene = { nullptr };
	CModel::TYPE eAnimType = { CModel::TYPE::TYPE_NONANIM };
	m_pAIScene = m_Importer.ReadFile(ModelFilePath.string().c_str(), iFlag);
	
	if (0 == m_pAIScene->mNumAnimations)
	{
		eAnimType = CModel::TYPE::TYPE_NONANIM;
		cout << "Type : TYPE_NONANIM" << endl;
	}
	else
	{
		cout << "Type : TYPE_ANIM" << endl;
		eAnimType = CModel::TYPE::TYPE_ANIM;
	}

	
	m_Importer.FreeScene();
	
	WriteFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);

	if (CModel::TYPE::TYPE_NONANIM == eAnimType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	

	m_pAIScene = m_Importer.ReadFile(ModelFilePath.string().c_str(), iFlag);
	cout << "Starting the binary conversion." << endl;
	SCENE::Serialization(m_pAIScene, hFile, dwByte);

	CloseHandle(hFile);
	cout << "Completed generating .dat file" << endl << endl;

	Out = eAnimType;
	return S_OK;
}
















//HRESULT CPaser::Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene)
//{
//	// ���� ����
//	HANDLE		hFile = CreateFile(pModelFilePath,		// ���� ��ο� �̸��� ���
//		GENERIC_READ,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
//		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
//		NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
//		OPEN_EXISTING,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
//		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
//		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		assert(false);
//		return E_FAIL;
//	}
//	DWORD dwByte = { 0 };
//	//SCENE Scene;
//
//	CModel::TYPE eAnimType;
//	ReadFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);
//	tScene.Deserialization(hFile, dwByte);
//
//	//Safe_Delete(Scene);
//	CloseHandle(hFile);
//	return S_OK;
//}