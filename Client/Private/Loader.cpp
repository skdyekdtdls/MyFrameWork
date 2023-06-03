#include "..\Public\Loader.h"
#include "GameInstance.h"
#include <process.h>
#include "BackGround.h"
#include "Camera_Free.h"
#include "Terrain.h"

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

	switch (m_eNextLevel)
	{
	case Client::LEVEL_LOGO:
		hr = Loading_For_Logo();
		break;
	case Client::LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay();
		break;
	case Client::LEVEL_IMGUI:
		hr = Loading_For_GamePlay();
		break;
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
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2)), E_FAIL);

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
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	Set_LoadingText(L"모델 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp"))), E_FAIL);

	Set_LoadingText(L"쉐이더 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXPOSNORTEX_DECL::Elements, VTXPOSNORTEX_DECL::iNumElements)), E_FAIL);

	Set_LoadingText(L"객체 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCamera_Free::ProtoTag(), CCamera_Free::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"로딩 완료");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_IMGUI()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	Set_LoadingText(L"텍스처 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_IMGUI, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	Set_LoadingText(L"모델 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_IMGUI, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp"))), E_FAIL);

	Set_LoadingText(L"쉐이더 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_IMGUI, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXPOSNORTEX_DECL::Elements, VTXPOSNORTEX_DECL::iNumElements)), E_FAIL);

	Set_LoadingText(L"객체 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCamera_Free::ProtoTag(), CCamera_Free::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"로딩 완료");

	m_isFinished = true;

	return S_OK;
}

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