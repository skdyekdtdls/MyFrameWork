#include "..\public\Paser.h"

fs::path fsStatic_Path = "../../Resources/Static_Mesh/";
fs::path fsSkeletal_Path = "../../Resources/Skeletal_Mesh/";

HRESULT CPaser::Pasing(fs::path fsModelFilePath)
{
	m_fsModelFilePath = fsModelFilePath;
	cout << "--------Searched .fbx file--------" << endl
		<< "Path : " << m_fsModelFilePath.string() << endl;

	// �ʿ��� �������� �̸� �з��س���.
	fs::path directoryPath = m_fsModelFilePath.parent_path();
	fs::path stem = m_fsModelFilePath.stem();
	string fbxFileName = stem.string() + ".fbx";
	string datFileName = stem.string() + ".dat";

	// ù �� ° ���ڸ� �빮�ڷ� �ٲ�
	if (!fbxFileName.empty()) {
		fbxFileName[0] = std::toupper(fbxFileName[0]);
	}
	if (!fbxFileName.empty()) {
		datFileName[0] = std::toupper(datFileName[0]);
	}

	// �ʿ��� �������� �̸� �з��س���.
	fs::path datFilePath = directoryPath / datFileName;
	fs::path fbxFilePath = directoryPath / fbxFileName;
	fs::path newDatFileName;
	fs::path newDatFilePath;
	// ANIM Ÿ������ NONANIMŸ������ �Ǵ�.
	m_eAnimType = IsAnim(m_fsModelFilePath);

	// ANIMŸ���̸� �̸� �ڿ� _Skeleton�� �ٱ� ������ ������.
	if (CModel::TYPE_ANIM == m_eAnimType)
	{
		newDatFileName = RemoveSkeletonFromFilename(datFileName);
		newDatFilePath = datFilePath.parent_path() / newDatFileName;
		MakeDataFile(newDatFilePath); // .dat���� �������ִ� �ڵ�
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
	else // ���� ����ƽ ��� ����� ����ұ� ������ ����� ����Ʈ.
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
	// �����̸� �����ͼ� ��θ����.
	// ���� ����
	HANDLE		hFile = CreateFile(datFilePath.c_str(),		// ���� ��ο� �̸��� ���
		GENERIC_WRITE,			// ���� ���� ���(���� ����), GENERIC_READ(�б� ����)
		NULL,					// ���� ���, ������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡�ϴ� �Ϳ� ���� ����, NULL�� �����ϸ� �������� �ʰڴٴ� �ǹ�
		NULL,					// ���� �Ӽ�, NULL�� ��� �⺻������ ���� ���¸� ����
		CREATE_ALWAYS,			// ���� ���, �ش� ������ ��� �۾��� �� ������, ���� ���� ������ ����(CREATE_ALWAYS : ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISTING : ������ ���� ��쿡�� ����)
		FILE_ATTRIBUTE_NORMAL,  // ���� �Ӽ�, FILE_ATTRIBUTE_NORMAL �ƹ��� �ӽ� ���� �Ϲ����� ���� ����
		NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �Ⱦ��ϱ� NULL

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

	// �� ������ ���� �ӽ� Device�� �ӽ� DeviceContext
	ID3D11Device* pDevice = { nullptr };
	ID3D11DeviceContext* pContext = { nullptr };
	D3D_FEATURE_LEVEL	FeatureLV;
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, nullptr, 0, D3D11_SDK_VERSION, &pDevice, &FeatureLV, &pContext);

	// �� ���� �� ���̺� �� ������ �̰� �ٷ� ����
	CModel* pModel = CModel::Create(pDevice, pContext, m_pAIScene, m_eAnimType, datFilePath.wstring().c_str(), XMMatrixIdentity());
	pModel->SaveAssimp(hFile, dwByte);
	Safe_Release(pModel);
	CloseHandle(hFile);
}

string CPaser::RemoveSkeletonFromFilename(const string& filename)
{
	std::string newFilename = filename;
	std::string target = "_Skeleton";

	// "_Skeletal"�� ã���ϴ�.
	size_t pos = newFilename.find(target);

	// ã�Ҵٸ� �ش� �κ��� �����մϴ�.
	if (pos != std::string::npos) {
		newFilename.erase(pos, target.length());
	}

	return newFilename;
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
//	CModel::TYPE m_eAnimType;
//	ReadFile(hFile, &m_eAnimType, sizeof(m_eAnimType), &dwByte, nullptr);
//	tScene.Deserialization(hFile, dwByte);
//
//	//Safe_Delete(Scene);
//	CloseHandle(hFile);
//	return S_OK;
//}