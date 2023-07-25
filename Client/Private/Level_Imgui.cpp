#ifdef _DEBUG
#include "Level_Imgui.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
#include "ImWindow_Manager.h"
#include "Clint.h"
#include "Sky.h"
#include "Blue_Snow.h"
#include "LargeVolcanicRock_002_Red_Desert.h"
#include "Alien_prawn.h"
CLevel_Imgui::CLevel_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Imgui::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Ready_Lights();
	Ready_Layer_Player(TEXT("Layer_Player"));
	Ready_Layer_BackGround(TEXT("Layer_BackGround"));
	Ready_Layer_Camera(TEXT("Layer_Camera"));
	Ready_Layer_Monster(TEXT("Layer_Monster"));
	Ready_Layer_Effect(TEXT("Layer_Effect"));
}

void CLevel_Imgui::Tick(_double TimeDelta)
{
#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->Tick();
#endif

	__super::Tick(TimeDelta);
}

void CLevel_Imgui::Late_Tick(_double TimeDelta)
{
#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->LateTick();
#endif

	__super::Late_Tick(TimeDelta);

	SetWindowText(g_hWnd, TEXT("게임 레벨입니다."));
}

HRESULT CLevel_Imgui::Render()
{
	return S_OK;
}

void CLevel_Imgui::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLight::LIGHTDESC LightDesc;
	ZeroStruct(LightDesc);

	LightDesc.eType = { CLight::TYPE_DIRECTIONAL };
	LightDesc.vDir = { 1.f, -1.f, 1.f, 0.f };
	LightDesc.vAmbient = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.vDiffuse = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.vSpecular = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(pGameInstance->Add_Lights(LightDesc)))
		return;

	Safe_Release(pGameInstance);
}

void CLevel_Imgui::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, CTerrain::ProtoTag(), pLayerTag));
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Sky::ProtoTag(), pLayerTag));

	Safe_Release(pGameInstance);
}

void CLevel_Imgui::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CEditCamera::EDIT_CAMERA_DESC Edit_Camera_Desc;

	Edit_Camera_Desc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	Edit_Camera_Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Edit_Camera_Desc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
	Edit_Camera_Desc.fFovy = XMConvertToRadians(60.0f);
	Edit_Camera_Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	Edit_Camera_Desc.fNear = 0.2f;
	Edit_Camera_Desc.fFar = 300.f;
	Edit_Camera_Desc.TransformDesc.SpeedPerSec = 10.f;
	Edit_Camera_Desc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, CEditCamera::ProtoTag(), pLayerTag, &Edit_Camera_Desc));

	Safe_Release(pGameInstance);
}

void CLevel_Imgui::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 몬스터 레이어 생성을 위해서 생성했다가 바로지움.
	CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_IMGUI, CCube::ProtoTag(), pLayerTag);
	pGameInstance->Delete_GameObject(LEVEL_IMGUI, L"Layer_Monster", pGameObject->Get_Name());
	
	Alien_prawn::ALIEN_PRAWN_DESC tAlienPrawnDesc;
	tAlienPrawnDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	pGameObject = pGameInstance->Add_GameObject(LEVEL_IMGUI, Alien_prawn::ProtoTag(), pLayerTag);

	Safe_Release(pGameInstance);
}

void CLevel_Imgui::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Clint::ProtoTag(), pLayerTag));

	Safe_Release(pGameInstance);
}

void CLevel_Imgui::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, CBlue_Snow::ProtoTag(), pLayerTag));

	Safe_Release(pGameInstance);
}

CLevel_Imgui* CLevel_Imgui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Imgui* pInstance = new CLevel_Imgui(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CLevel_Imgui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Imgui::Free()
{
	__super::Free();
}
#endif