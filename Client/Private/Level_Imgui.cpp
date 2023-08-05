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
#include "BatPotato_RIG.h"
#include "CannonSpider.h"
#include "CrystalGolem.h"
#include "Spider.h"
#include "Queen_Moggoth.h"
#include "PlayerHP.h"
#include "Image.h"
#include "DynamicImage.h"
#include "MiniMap.h"

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
	Ready_Layer_UI(TEXT("Layer_UI"));
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
	
	Alien_prawn::ALIEN_PRAWN_DESC tAlienPrawnDesc;
	tAlienPrawnDesc.vPosition = _float4(11.f, 0.f, 10.f, 1.f);
	pGameInstance->Add_GameObject(LEVEL_IMGUI, Alien_prawn::ProtoTag(), pLayerTag, &tAlienPrawnDesc);

	//tAlienPrawnDesc.vPosition = _float4(12.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Alien_prawn::ProtoTag(), pLayerTag, &tAlienPrawnDesc);

	//tAlienPrawnDesc.vPosition = _float4(11.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Alien_prawn::ProtoTag(), pLayerTag, &tAlienPrawnDesc);

	//BatPotato_RIG::BATPOTATO_RIG_DESC tBatPotatoRigDesc;
	//tBatPotatoRigDesc.vPosition = _float4(12.f, 0.f, 10.f, 1.f);
  	//pGameInstance->Add_GameObject(LEVEL_IMGUI, BatPotato_RIG::ProtoTag(), pLayerTag, &tBatPotatoRigDesc);
	
	//CannonSpider::tagCannonSpiderDesc tCannonSpiderDesc;
	//tCannonSpiderDesc.vPosition = _float4(13.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, CannonSpider::ProtoTag(), pLayerTag, &tCannonSpiderDesc);
	
	CrystalGolem::tagCrystalGolemDesc tCrystalGolemDesc;
	tCrystalGolemDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	pGameInstance->Add_GameObject(LEVEL_IMGUI, CrystalGolem::ProtoTag(), pLayerTag, &tCrystalGolemDesc);
	
	/*Spider::tagSpiderDesc tSpiderDesc;
	tSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	pGameInstance->Add_GameObject(LEVEL_IMGUI, Spider::ProtoTag(), pLayerTag, &tSpiderDesc);*/

	//tSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Spider::ProtoTag(), pLayerTag, &tSpiderDesc);
	//
	//tSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Spider::ProtoTag(), pLayerTag, &tSpiderDesc);
	//
	//tSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Spider::ProtoTag(), pLayerTag, &tSpiderDesc);*/

	//Queen_Moggoth::tagQueen_MoggothDesc tQueenMoggothDesc;
	//tQueenMoggothDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_IMGUI, Queen_Moggoth::ProtoTag(), pLayerTag, &tQueenMoggothDesc);

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

void CLevel_Imgui::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	Image* pImage;
	DynamicImage* pDynamicImage;
	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	Image::tagImageDesc tImageDesc;
	
	/*
	tImageDesc.Pos = _float2(842, g_iWinSizeY - 615);
	tImageDesc.Size = _float2(80, 80);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_T_WeaponSelected_Arrow");
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));

	tImageDesc.Pos = _float2(1181, g_iWinSizeY - 33);
	tImageDesc.Size = _float2(20, 20);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_DNA");
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));

	tImageDesc.Pos = _float2(1181, g_iWinSizeY - 33);
	tImageDesc.Size = _float2(-20, 20);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_DNA");
	pGameObject = pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc);
	*/

	//tImageDesc.Pos = _float2(g_iWinSizeX - 150, g_iWinSizeY - 607);
	//tImageDesc.Size = _float2(210, 210);
	//tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_UI_KS_Radar");
	//NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));

	tImageDesc.Pos = _float2(g_iWinSizeX >> 1, g_iWinSizeY >> 1);
	tImageDesc.Size = _float2(g_iWinSizeX, g_iWinSizeY);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_CustomUI");
	pImage = static_cast<Image*>(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));
	pImage->ImageDepth(0.001f);

	//tImageDesc.Pos = _float2(g_iWinSizeX - 150, (g_iWinSizeY - 607));
	//tImageDesc.Size = _float2(20, 20);
	//tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_ui_radar_player_icon");
	//NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));

	tImageDesc.Size = _float2(14, 4);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_UI_dash");
	for (_uint i = 0; i < 3; ++i)
	{
		tImageDesc.Pos = _float2((g_iWinSizeX >> 1) - 40 + i * (tImageDesc.Size.x + 5), (g_iWinSizeY >> 1) + 87);
		pImage = static_cast<Image*>(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));
		pImage->SetPass(2);
	}
	
	pGameInstance->Add_GameObject(LEVEL_IMGUI, MiniMap::ProtoTag(), pLayerTag);

	//tDynamicImageDesc.Pos = _float2(532, 96);
	//tDynamicImageDesc.Size = _float2(70, 70);
	//tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_QSkill");
	//NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, DynamicImage::ProtoTag(), pLayerTag, &tDynamicImageDesc));

	/*tDynamicImageDesc.Pos = _float2(532 + 80, 96);
	tDynamicImageDesc.Size = _float2(60, 60);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_WSkill");
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, DynamicImage::ProtoTag(), pLayerTag, &tDynamicImageDesc));

	tDynamicImageDesc.Pos = _float2(532 + 160, 96);
	tDynamicImageDesc.Size = _float2(70, 70);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_ESkill");
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, DynamicImage::ProtoTag(), pLayerTag, &tDynamicImageDesc));

	tDynamicImageDesc.Pos = _float2(532 + 240, 96);
	tDynamicImageDesc.Size = _float2(70, 70);
	tDynamicImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_RSkill");
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_IMGUI, DynamicImage::ProtoTag(), pLayerTag, &tDynamicImageDesc));*/

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