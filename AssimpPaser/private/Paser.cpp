#include "..\public\Paser.h"

HRESULT Paser::Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType)
{
	// 파일 개방
	HANDLE		hFile = CreateFile(L"../Bin/Fiona.dat",		// 파일 경로와 이름을 명시
		GENERIC_WRITE,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		CREATE_ALWAYS,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}

	HRESULT hr = { 0 };
	DWORD		dwByte = 0;

	_uint		iFlag = 0;

	WriteFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);

	if (ANIM_TYPE::NONANIM == eAnimType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	
	Assimp::Importer	m_Importer; /* 내가 경로로 던져준 파일을 읽고 저장한다.*/
	const aiScene* m_pAIScene = { nullptr };
	
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	SCENE::Serialization(m_pAIScene, hFile, dwByte);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT Paser::Load(const char* pModelFilePath)
{
	// 파일 개방
	HANDLE		hFile = CreateFile(L"../Bin/Fiona.dat",		// 파일 경로와 이름을 명시
		GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		OPEN_EXISTING,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return E_FAIL;
	}
	DWORD dwByte = { 0 };
	//SCENE Scene;

	ANIM_TYPE eAnimType;
	ReadFile(hFile, &eAnimType, sizeof(eAnimType), &dwByte, nullptr);
	SCENE tmp;
	tmp.Deserialization(hFile, dwByte);

	//Safe_Delete(Scene);
	CloseHandle(hFile);
	return S_OK;
}