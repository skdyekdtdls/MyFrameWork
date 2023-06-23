#include "..\Public\Loader.h"
#include "GameInstance.h"
#include <process.h>
#include "BackGround.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
#include "Player.h"
#include "Monster.h"
CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: m_pDevice(pDevice)
	, m_pContext(pDeviceContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

_uint WINAPI Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->Loading()))
		return E_FAIL;

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	CoInitializeEx(nullptr, 0);

	HRESULT hr = { 0 };

	EnterCriticalSection(&m_Critical_Section);
	m_pGameInstance->Set_NextLevelIndex(m_eNextLevel);
	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;
	case Client::LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
#ifdef _DEBUG
	case Client::LEVEL_IMGUI:
		hr = Loading_For_IMGUI();
		break;
#endif
	default:
		FAILED_CHECK_RETURN(E_FAIL, E_FAIL);
	}
	

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	Set_LoadingText(L"텍스처 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Default%d.jpg"), 2)), E_FAIL);

	Set_LoadingText(L"모델 로딩 중");


	Set_LoadingText(L"쉐이더 로딩 중");


	Set_LoadingText(L"객체 로딩 중");

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CBackGround::ProtoTag(),
		CBackGround::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"로딩 완료");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	Set_LoadingText(L"텍스처 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Filter.bmp", 1))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Brush.png", 1))), E_FAIL);

	Set_LoadingText(L"모델 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Height.bmp"))), E_FAIL);

	Set_LoadingText(L"쉐이더 로딩 중");


	Set_LoadingText(L"객체 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCamera_Free::ProtoTag(), CCamera_Free::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"로딩 완료");

	m_isFinished = true;

	return S_OK;
}

#ifdef _DEBUG
HRESULT CLoader::Loading_For_IMGUI()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	_matrix		PivotMatrix = XMMatrixIdentity();

	Set_LoadingText(L"텍스처 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Filter.bmp", 1))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Brush.png", 1))), E_FAIL);

	Set_LoadingText(L"모델 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Height.bmp"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);

	//PivotMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));
	//FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Model_clint_Skeleton"),
	//	CModel::Create(m_pDevice, m_pContext, TEXT("../../Resources/Models/clint_Skeleton/clint_Skeleton.dat"), PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, TEXT("../../Resources/Models/Fiona/Fiona.dat"), PivotMatrix)), E_FAIL);

	lstrcpy(m_szLoading, TEXT("네비게이션정보 로딩 중."));
	/* For.Prototype_COmpoentn_Navigation */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CNavigation::ProtoTag(),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation.dat"))), E_FAIL);

	Set_LoadingText(L"셰이더 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Shader_Navigation"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl")
			, VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements)), E_FAIL);

	Set_LoadingText(L"충돌체 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CColliderSphere::ProtoTag(),
		CColliderSphere::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"객체 로딩 중"); // 객체는 마지막에 로딩되어야한다.
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CEditCamera::ProtoTag(), CEditCamera::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCube::ProtoTag(), CCube::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CPlayer::ProtoTag(), CPlayer::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CMonster::ProtoTag(), CMonster::Create(m_pDevice, m_pContext)), E_FAIL);
	Set_LoadingText(L"로딩 완료");

	m_isFinished = true;

	return S_OK;
}
#endif
CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, LEVELID eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Create CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);
	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}