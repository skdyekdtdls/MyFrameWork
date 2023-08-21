#include "Level_GamePlay.h"
#include "Spawner.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
#include "ImWindow_Manager.h"
#include "Clint.h"
#include "Sky.h"
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
CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	Ready_Lights();
	Ready_Layer_Player(TEXT("Layer_Player"));
	Ready_Layer_BackGround(TEXT("Layer_BackGround"));
	Ready_Layer_Camera(TEXT("Layer_Camera"));
	Ready_Layer_Monster(TEXT("Layer_Monster"));
	Ready_Layer_Effect(TEXT("Layer_Effect"));

	// 로드, 이 순서를 유지하는게 신상에 좋음.
	LoadLevel();

	Ready_Layer_UI(TEXT("Layer_UI"));
	Ready_Layer_ETC();

	return S_OK;
}

void CLevel_GamePlay::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	Image* pImage;
	DynamicImage* pDynamicImage;
	DynamicImage::tagDynamicImageDesc tDynamicImageDesc;
	Image::tagImageDesc tImageDesc;

	tImageDesc.Pos = _float2(g_iWinSizeX >> 1, g_iWinSizeY >> 1);
	tImageDesc.Size = _float2(g_iWinSizeX, g_iWinSizeY);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_CustomUI");
	pImage = static_cast<Image*>(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));
	pImage->ImageDepth(0.001f);

	tImageDesc.Size = _float2(14, 4);
	tImageDesc.pTextureProtoTag = TEXT("Prototype_Component_Texture_UI_dash");
	for (_uint i = 0; i < 3; ++i)
	{
		tImageDesc.Pos = _float2((g_iWinSizeX >> 1) - 40 + i * (tImageDesc.Size.x + 5), (g_iWinSizeY >> 1) + 87);
		pImage = static_cast<Image*>(pGameInstance->Add_GameObject(LEVEL_IMGUI, Image::ProtoTag(), pLayerTag, &tImageDesc));
		pImage->SetPass(2);
	}

	pGameInstance->Add_GameObject(LEVEL_IMGUI, MiniMap::ProtoTag(), pLayerTag);
	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Layer_ETC()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eCurLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	NULL_CHECK(pGameInstance->Add_GameObject(eCurLevelID, Spawner::ProtoTag(), L"Layer_Spawner", nullptr));

	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_GamePlay::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	SetWindowText(g_hWnd, TEXT("게임 레벨입니다."));
}

HRESULT CLevel_GamePlay::Render()
{
	return S_OK;
}

void CLevel_GamePlay::LoadLevel()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	HANDLE hFile = CreateFile(TEXT("../Bin/Data/Level/Level_GamePlay.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	CRenderer* pRenderer = static_cast<CRenderer*>(pGameInstance->Get_ProtoComponent(LEVEL_STATIC, CRenderer::ProtoTag()));
	pRenderer->Draw_RenderGroup();
	if (0 == hFile)
		assert(false);
	DWORD dwByte = { 0 };
	pGameInstance->Deserialization(hFile, dwByte, LEVEL_GAMEPLAY);
	CloseHandle(hFile);
	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Lights()
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
void CLevel_GamePlay::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, CTerrain::ProtoTag(), pLayerTag));
	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Sky::ProtoTag(), pLayerTag));

	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
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

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, CEditCamera::ProtoTag(), pLayerTag, &Edit_Camera_Desc));

	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//Alien_prawn::ALIEN_PRAWN_DESC tAlienPrawnDesc;
	//tAlienPrawnDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Alien_prawn::ProtoTag(), pLayerTag, &tAlienPrawnDesc);

	/*BatPotato_RIG::BATPOTATO_RIG_DESC tBatPotatoRigDesc;
	tBatPotatoRigDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, BatPotato_RIG::ProtoTag(), pLayerTag, &tBatPotatoRigDesc);*/

	//CannonSpider::tagCannonSpiderDesc tCannonSpiderDesc;
	//tCannonSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, CannonSpider::ProtoTag(), pLayerTag, &tCannonSpiderDesc);

	//CrystalGolem::tagCrystalGolemDesc tCrystalGolemDesc;
	//tCrystalGolemDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, CrystalGolem::ProtoTag(), pLayerTag, &tCrystalGolemDesc);

	//Spider::tagSpiderDesc tSpiderDesc;
	//tSpiderDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	//pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Spider::ProtoTag(), pLayerTag, &tSpiderDesc);

	Queen_Moggoth::tagQueen_MoggothDesc tQueenMoggothDesc;
	tQueenMoggothDesc.vPosition = _float4(10.f, 0.f, 10.f, 1.f);
	pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Queen_Moggoth::ProtoTag(), pLayerTag, &tQueenMoggothDesc);

	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	NULL_CHECK(pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Clint::ProtoTag(), pLayerTag));

	Safe_Release(pGameInstance);
}

void CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CLevel_GamePlay");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}