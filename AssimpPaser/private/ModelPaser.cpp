#include "..\public\ModelPaser.h"
#include "MeshPaser.h"
#include "BonePaser.h"
#include "MaterialPaser.h"

HRESULT CModelPaser::Pasing(const char* pModelFilePath, ANIM_TYPE eAnimType)
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

	//HANDLE		hFile = CreateFile(L"../Bin/Line.txt",		// 파일 경로와 이름을 명시
	//	GENERIC_READ,			// 파일 접근 모드(쓰기 전용), GENERIC_READ(읽기 전용)
	//	NULL,					// 공유 방식, 파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가하는 것에 대한 설정, NULL을 지정하면 공유하지 않겠다는 의미
	//	NULL,					// 보안 속성, NULL인 경우 기본값으로 보안 상태를 설정
	//	OPEN_EXISTING,			// 생성 방식, 해당 파일을 열어서 작업을 할 것인지, 새로 만들 것인지 설정(CREATE_ALWAYS : 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISTING : 파일이 있을 경우에만 연다)
	//	FILE_ATTRIBUTE_NORMAL,  // 파일 속성, FILE_ATTRIBUTE_NORMAL 아무런 속싱 없는 일반적인 파일 생성
	//	NULL);					// 생성될 파일의 속성을 제공할 템플릿 파일, 안쓰니까 NULL

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
