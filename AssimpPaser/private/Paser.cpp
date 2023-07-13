#include "..\public\Paser.h"

fs::path fsStatic_Path = "../../Resources/Static_Mesh/";
fs::path fsSkeletal_Path = "../../Resources/Skeletal_Mesh/";

HRESULT CPaser::Pasing(fs::path fsModelFilePath)
{
	m_fsModelFilePath = fsModelFilePath;
	cout << "--------Searched .fbx file--------" << endl
		<< "Path : " << m_fsModelFilePath.string() << endl;

	// 필요한 정보들을 미리 분류해놓음.
	fs::path directoryPath = m_fsModelFilePath.parent_path();
	fs::path stem = m_fsModelFilePath.stem();
	string fbxFileName = stem.string() + ".fbx";
	string datFileName = stem.string() + ".dat";

	// 첫 번 째 글자를 대문자로 바꿈
	if (!fbxFileName.empty()) {
		fbxFileName[0] = std::toupper(fbxFileName[0]);
	}
	if (!fbxFileName.empty()) {
		datFileName[0] = std::toupper(datFileName[0]);
	}

	// 필요한 정보들을 미리 분류해놓음.
	fs::path datFilePath = directoryPath / datFileName;
	fs::path fbxFilePath = directoryPath / fbxFileName;
	fs::path newDatFileName;
	fs::path newDatFilePath;
	// ANIM 타입인지 NONANIM타입인지 판단.
	m_eAnimType = IsAnim(m_fsModelFilePath);

	// ANIM타입이면 이름 뒤에 _Skeleton이 붙기 때문에 지워줌.
	if (CModel::TYPE_ANIM == m_eAnimType)
	{
		newDatFileName = RemoveSkeletonFromFilename(datFileName);
		newDatFilePath = datFilePath.parent_path() / newDatFileName;
		MakeDataFile(newDatFilePath); // .dat파일 생성해주는 코드
	}
	else
		MakeDataFile(datFilePath);
	cout << "Completed generating .dat file" << endl << endl;

	// 
	if (CModel::TYPE_ANIM == m_eAnimType)
	{
		MoveEntitySourToDest(fbxFilePath, fsSkeletal_Path / newDatFileName.stem() / fbxFileName);
		MoveEntitySourToDest(newDatFilePath, fsSkeletal_Path / newDatFileName.stem() / newDatFileName);
	}
	else // 아직 스테틱 경로 제대로 안잡았기 때문에 디버그 포인트.
	{
		MoveEntitySourToDest(datFilePath, fsStatic_Path / datFilePath.stem() / datFileName);
		MoveEntitySourToDest(fbxFilePath, fsStatic_Path / datFilePath.stem() / fbxFileName);
	}
	cout << "File Moved Completely" << endl;
	return S_OK;
}

void CPaser::MoveEntitySourToDest(fs::path SourPath, fs::path DestPath)
{
	try {
		if (!fs::exists(DestPath.parent_path())) {
			fs::create_directories(DestPath.parent_path());
		}

		fs::rename(SourPath, DestPath);
	}
	catch (fs::filesystem_error& e) {
		std::cerr << e.what() << '\n';
	}
}

CModel::TYPE CPaser::IsAnim(fs::path m_fsModelFilePath)
{
	CModel::TYPE eAnimType = { CModel::TYPE::TYPE_NONANIM };

	m_pAIScene = m_Importer.ReadFile(m_fsModelFilePath.string(), 0);

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
	return eAnimType;
}

void CPaser::MakeDataFile(fs::path datFilePath)
{
	// 파일이름 가져와서 경로만들기.
	// 파일 개방
	HANDLE		hFile = CreateFile(datFilePath.c_str(),		// 파일 경로와 이름을 명시
		GENERIC_WRITE,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
		NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
		NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
		CREATE_ALWAYS,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
		FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL

	if (INVALID_HANDLE_VALUE == hFile)
	{
		assert(false);
		return;
	}

	HRESULT hr = { 0 };
	DWORD		dwByte = 0;
	_uint		iFlag = 0;

	WriteFile(hFile, &m_eAnimType, sizeof(m_eAnimType), &dwByte, nullptr);

	if (CModel::TYPE::TYPE_NONANIM == m_eAnimType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	m_pAIScene = m_Importer.ReadFile(m_fsModelFilePath.string().c_str(), iFlag);
	cout << "Starting the binary conversion." << endl;

	// 모델 생성을 위한 임시 Device와 임시 DeviceContext
	ID3D11Device* pDevice = { nullptr };
	ID3D11DeviceContext* pContext = { nullptr };
	D3D_FEATURE_LEVEL	FeatureLV;
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, nullptr, 0, D3D11_SDK_VERSION, &pDevice, &FeatureLV, &pContext);

	// 모델 생성 후 세이브 할 정보만 뽑고 바로 삭제
	CModel* pModel = CModel::Create(pDevice, pContext, m_pAIScene, m_eAnimType, datFilePath.wstring().c_str(), XMMatrixIdentity());
	pModel->SaveAssimp(hFile, dwByte);
	Safe_Release(pModel);
	CloseHandle(hFile);
}

string CPaser::RemoveSkeletonFromFilename(const string& filename)
{
	std::string newFilename = filename;
	std::string target = "_Skeleton";

	// "_Skeletal"을 찾습니다.
	size_t pos = newFilename.find(target);

	// 찾았다면 해당 부분을 제거합니다.
	if (pos != std::string::npos) {
		newFilename.erase(pos, target.length());
	}

	return newFilename;
}
















//HRESULT CPaser::Load(const _tchar* pModelFilePath, _Inout_ SCENE& tScene)
//{
//	// 파일 개방
//	HANDLE		hFile = CreateFile(pModelFilePath,		// 파일 경로와 이름을 명시
//		GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
//		NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
//		NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
//		OPEN_EXISTING,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
//		FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
//		NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		assert(false);
//		return E_FAIL;
//	}
//	DWORD dwByte = { 0 };
//	//SCENE Scene;
//
//	CModel::TYPE m_eAnimType;
//	ReadFile(hFile, &m_eAnimType, sizeof(m_eAnimType), &dwByte, nullptr);
//	tScene.Deserialization(hFile, dwByte);
//
//	//Safe_Delete(Scene);
//	CloseHandle(hFile);
//	return S_OK;
//}