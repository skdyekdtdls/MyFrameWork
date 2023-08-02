#include "../Public/Loader.h"
#include "GameInstance.h"
#include <process.h>
#include "PlayerLevel.h"
#include "SM_Cliff01.h"
#include "SM_MountainRock_Closed.h"
#include "SM_LargePlainsBoulder002.h"
#include "SM_LargePlainsBoulder003.h"
#include "SM_LargePlainsBoulder002_Red_Desert.h"
#include "SM_MountainRock.h"
#include "Sky.h"
#include "BackGround.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
#include "Model.h"
#include "ForkLift.h"
#include "Fiona.h"
#include "Clint.h"
#include "ClintUltimate01Bullet.h"
// UI
#include "Image.h"
#include "DynamicImage.h"
#include "FontUI.h"

// Static Mesh
#include "Bush01.h"
#include "BushA.h"
#include "BushB.h"
#include "BushC.h"
#include "Cage.h"
#include "Cave.h"
#include "CraterA.h"
#include "CraterB.h"
#include "CraterC.h"
#include "Kemmek_Hole.h"
#include "MineCrystalsA.h"
#include "MineCrystalsA.h"
#include "MineCrystalsB.h"
#include "MineCrystalsC.h"
#include "MineCrystalsD.h"
#include "MineCrystalsE.h"
#include "MineCrystalsF.h"
#include "TreeA.h"
#include "TreeB1.h"
#include "TreeB2.h"

#include "Pistola.h"
#include "Alien_prawn.h"
#include "StateContext.h"
#include "StateMachine.h"
#include "ClintDash.h"
#include "ClintIdle.h"
#include "ClintRun.h"
#include "ClintShoot.h"
#include "ClintUltimate01.h"
#include "ClintSkill01.h"
#include "ClintSkill02.h"
#include "Alien_prawnIdle.h"
#include "Alien_prawnRun.h"
#include "Alien_prawnAttack.h"
#include "Alien_prawnDead.h"
#include "Alien_prawnHit.h"
#include "ClintBasicBullet.h"
#include "ClintUltimate01Bullet.h"
#include "VIBuffer_Cube.h"
#include "Blue_Snow.h"
#include "LargeVolcanicRock_002_Red_Desert.h"

#include "Kemmekh_Formacion_rocosa.h"
#include "Kemmekh_wall.h"
#include "SM_Wall02.h"
#include "SM_Wall01.h"
#include "SM_TrozoPared.h"
#include "SM_Relic.h"
#include "SM_Pueblo02.h"
#include "SM_Poblado01.h"
#include "SM_Poblado.h"
#include "SM_Piedras.h"
#include "SM_MultiLake02.h"
#include "SM_MiniBujer.h"
#include "SM_Mina2.h"
#include "SM_MERGED_RailSupportB2_452.h"
#include "SM_MERGED_MultiRock371.h"
#include "SM_LaMina.h"
#include "SM_LagoSmall.h"
#include "SM_GenericWall01.h"
#include "SM_CrystalYRocas.h"
#include "SM_Crystal.h"
#include "SM_Barriletes.h"
#include "LargeVolcanicRock_002_Red_Desert.h"

#include "BatPotato_RIG.h"
#include "BatPotato_RIGAttack.h"
#include "BatPotato_RIGDeath.h"
#include "BatPotato_RIGHit.h"
#include "BatPotato_RIGIdle.h"
#include "BatPotato_RIGRun.h"
#include "BatPotato_RIGBullet.h"

#include "CannonSpider.h"
#include "CannonSpiderBullet.h"
#include "CannonSpiderAttack.h"
#include "CannonSpiderDead.h"
#include "CannonSpiderIdle.h"
#include "CannonSpiderHit.h"
#include "CannonSpiderRun.h"
#include "CannonSpiderSearch.h"

#include "CrystalGolem.h"
#include "CrystalGolemAppear.h"
#include "CrystalGolemAttack.h"
#include "CrystalGolemDead.h"
#include "CrystalGolemIdle.h"
#include "CrystalGolemRun.h"
#include "CrystalGolemTurn180.h"
#include "CrystalGolemAttackAreaBullet.h"
#include "CrystalGolemAttackArea02Bullet.h"
#include "CrystalGolemAttackRangeBullet.h"

#include "Spider.h"
#include "SpiderBullet.h"
#include "SpiderIdle.h"
#include "SpiderAttack.h"
#include "SpiderRun.h"
#include "SpiderDead.h"
#include "SpiderHit.h"
#include "SpiderAppear.h"

#include "Queen_Moggoth.h"
#include "Queen_MoggothAppear.h"
#include "Queen_MoggothAttack.h"
#include "Queen_MoggothDead.h"
#include "Queen_MoggothIdle.h"
#include "Queen_MoggothWalk.h"
#include "Queen_MoggothWalkP2.h"
#include "Queen_MoggothIdleP2.h"
#include "Queen_MoggothAttackP2.h"
#include "Queen_Moggoth_P1_TO_P2.h"
#include "P1Attack01.h"
#include "P1Attack02.h"
#include "P1Attack03.h"
#include "P1Attack04.h"
#include "P2Attack01.h"
#include "P2Attack02.h"
#include "P2Attack03.h"
#include "P2Attack04.h"

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

	Set_LoadingText(L"�ؽ�ó �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Logo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Art/GameIcon/KillsquadLogoHalloween.png"))), E_FAIL);

	Set_LoadingText(L"�� �ε� ��");

	Set_LoadingText(L"���̴� �ε� ��");

	Set_LoadingText(L"��ü �ε� ��");

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CBackGround::ProtoTag(),
		CBackGround::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"�ε� �Ϸ�");

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	_matrix		PivotMatrix = XMMatrixIdentity();
	CModel* pModel;
	vector<_uint> upper;
	vector<_uint> lower;

	Set_LoadingText(L"�ؽ�ó �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Brush.png"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SkyBox/Sky_%d.dds"), 4)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Snow/Snow.png"), 1)), E_FAIL);

	Set_LoadingText(L"���� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Art/Misc/John/Materials/TexturesCom_SmoothRock_1024_heightmap.bmp"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);

	CVIBuffer_Rect_Instance::INSTANCEDESC tVIBufferRectInstanceDesc;
	ZeroStruct(tVIBufferRectInstanceDesc);
	tVIBufferRectInstanceDesc.vRange = _float3(10.f, 4.f, 10.f); tVIBufferRectInstanceDesc.vSpeed = _uint2(2, 10);
	tVIBufferRectInstanceDesc.fLifeTime = 10.f; tVIBufferRectInstanceDesc.iNumInstance = 30; tVIBufferRectInstanceDesc.fHeight = 10.f;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Rect_Instance::ProtoTag(),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, &tVIBufferRectInstanceDesc)), E_FAIL);

	CVIBuffer_Point_Instance::INSTANCEDESC tVIBufferPointInstanceDesc;
	tVIBufferPointInstanceDesc.vRange = _float3(10.f, 4.f, 10.f); tVIBufferPointInstanceDesc.vSpeed = _uint2(2, 10);
	tVIBufferPointInstanceDesc.fLifeTime = 10.f; tVIBufferPointInstanceDesc.iNumInstance = 30; tVIBufferPointInstanceDesc.fHeight = 10.f;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Point_Instance::ProtoTag(),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &tVIBufferPointInstanceDesc)), E_FAIL);

	// Skeletal_Meshes
	Set_LoadingText(L"�� �ε� ��");
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 3); pModel->LoadAssimp("Clint.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Clint/Animation.myanim"));
	upper.clear(); lower.clear();
	upper.push_back(7); upper.push_back(11); upper.push_back(12);
	lower.push_back(3); lower.push_back(4); lower.push_back(88); lower.push_back(94);
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Clint"), pModel), E_FAIL);

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Alien_prawn1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn1"), pModel), E_FAIL);

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawn2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawn3.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn3"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawnCharger1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawnCharger1"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawnCharger2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawnCharger2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("BatPotato_RIG1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/BatPotato_RIG1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BatPotato_RIG1"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("BatPotato_RIG2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/BatPotato_RIG1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BatPotato_RIG2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("CannonSpider.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/CannonSpider/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CannonSpider"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("CrystalGolem.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/CrystalGolem/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CrystalGolem"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("MicroSpider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MicroSpider1"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("MicroSpider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MicroSpider2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider1"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider2"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider3.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider3"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("SummonSpider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SummonSpider1"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("SummonSpider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SummonSpider2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Queen_Moggoth.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Queen_Moggoth/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Queen_Moggoth"), pModel), E_FAIL);

	/*
			
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmeth_Bird.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Kemmeth_Bird/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmeth_Bird"), pModel), E_FAIL);
	*/


	// Static_Meshes
	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Cliff01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Cliff01"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder002.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder002"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder002_Red_Desert.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder002_Red_Desert"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder003.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder003"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MountainRock.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MountainRock"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MountainRock_Closed.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MountainRock_Closed"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmekh_Formacion_rocosa.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmekh_Formacion_rocosa"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmekh_wall.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmekh_wall"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Wall02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Wall02"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Wall01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Wall01"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_TrozoPared.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_TrozoPared"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Relic.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Relic"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Pueblo02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Pueblo02"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Poblado01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Poblado01"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Poblado.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Poblado"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Piedras.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Piedras"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MultiLake02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MultiLake02"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MiniBujer.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MiniBujer"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Mina2.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Mina2"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MERGED_RailSupportB2_452.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MERGED_RailSupportB2_452"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MERGED_MultiRock371.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MERGED_MultiRock371"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LaMina.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LaMina"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LagoSmall.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LagoSmall"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_GenericWall01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_GenericWall01"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_CrystalYRocas.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_CrystalYRocas"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Crystal.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Crystal"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Barriletes.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Barriletes"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Bush01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Bush01"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushA"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushB"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushC"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Cage.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Cage"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Cave.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Cave"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterA"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterB"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterC"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmek_Hole.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmek_Hole"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsA"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsB"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsC"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsD.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsD"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsE.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsE"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsF.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsF"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeA"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeB1.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeB1"), pModel), E_FAIL);


	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeB2.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeB2"), pModel), E_FAIL);

	
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);// * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Pistola.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Pistola"), pModel), E_FAIL);

	lstrcpy(m_szLoading, TEXT("�׺���̼����� �ε� ��."));
	/* For.Prototype_COmpoentn_Navigation */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CNavigation::ProtoTag(),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/Navigation1.dat"))), E_FAIL);

	Set_LoadingText(L"���̴� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Shader_Navigation"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl")
			, VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements)), E_FAIL);

	Set_LoadingText(L"���� �ε� ��");

	
	StateContext<Clint, CLINT_ANIM>* pClintState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_ClintState"),
		pClintState = StateContext<Clint, CLINT_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pClintState->Add_State(ClintIdle::Tag(), ClintIdle::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintRun::Tag(), ClintRun::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintDash::Tag(), ClintDash::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintShoot::Tag(), ClintShoot::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintUltimate01::Tag(), ClintUltimate01::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintSkill01::Tag(), ClintSkill01::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintSkill02::Tag(), ClintSkill02::Create(m_pDevice, m_pContext));

	
	StateContext<Alien_prawn, ALIEN_PRAWN_ANIM>* pAlienPrawnState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_AlienPrawnState"),
		pAlienPrawnState = StateContext<Alien_prawn, ALIEN_PRAWN_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pAlienPrawnState->Add_State(Alien_prawnIdle::Tag(), Alien_prawnIdle::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnRun::Tag(), Alien_prawnRun::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnAttack::Tag(), Alien_prawnAttack::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnDead::Tag(), Alien_prawnDead::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnHit::Tag(), Alien_prawnHit::Create(m_pDevice, m_pContext));

	
	StateContext<BatPotato_RIG, BATPOTATO_RIG_ANIM>* pBatPotatoState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_BatPotatoRIGState"),
		pBatPotatoState = StateContext<BatPotato_RIG, BATPOTATO_RIG_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pBatPotatoState->Add_State(BatPotato_RIGAttack::Tag(), BatPotato_RIGAttack::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGDeath::Tag(), BatPotato_RIGDeath::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGHit::Tag(), BatPotato_RIGHit::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGIdle::Tag(), BatPotato_RIGIdle::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGRun::Tag(), BatPotato_RIGRun::Create(m_pDevice, m_pContext));

	
	StateContext<CannonSpider, CANNON_SPIDER_ANIM>* pCannonSpiderState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_CannonSpiderState"),
		pCannonSpiderState = StateContext<CannonSpider, CANNON_SPIDER_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pCannonSpiderState->Add_State(CannonSpiderAttack::Tag(), CannonSpiderAttack::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderDead::Tag(), CannonSpiderDead::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderHit::Tag(), CannonSpiderHit::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderIdle::Tag(), CannonSpiderIdle::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderRun::Tag(), CannonSpiderRun::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderSearch::Tag(), CannonSpiderSearch::Create(m_pDevice, m_pContext));

	
	StateContext<CrystalGolem, CRYSTAL_GOLEM_ANIM>* pCrystalGolemState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_CrystalGolemState"),
		pCrystalGolemState = StateContext<CrystalGolem, CRYSTAL_GOLEM_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pCrystalGolemState->Add_State(CrystalGolemIdle::Tag(), CrystalGolemIdle::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemAppear::Tag(), CrystalGolemAppear::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemRun::Tag(), CrystalGolemRun::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemDead::Tag(), CrystalGolemDead::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemTurn180::Tag(), CrystalGolemTurn180::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemAttack::Tag(), CrystalGolemAttack::Create(m_pDevice, m_pContext));

	
	StateContext<Spider, SPIDER_ANIM>* pSpiderState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_SpiderState"),
		pSpiderState = StateContext<Spider, SPIDER_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pSpiderState->Add_State(SpiderAttack::Tag(), SpiderAttack::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderDead::Tag(), SpiderDead::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderHit::Tag(), SpiderHit::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderIdle::Tag(), SpiderIdle::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderRun::Tag(), SpiderRun::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderAppear::Tag(), SpiderAppear::Create(m_pDevice, m_pContext));

	
	StateContext<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* pQueenMoggothState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Queen_MoggothState"),
		pQueenMoggothState = StateContext<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pQueenMoggothState->Add_State(Queen_MoggothAppear::Tag(), Queen_MoggothAppear::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothAttack::Tag(), Queen_MoggothAttack::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothDead::Tag(), Queen_MoggothDead::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothIdle::Tag(), Queen_MoggothIdle::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothWalk::Tag(), Queen_MoggothWalk::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothWalkP2::Tag(), Queen_MoggothWalkP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothIdleP2::Tag(), Queen_MoggothIdleP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothAttackP2::Tag(), Queen_MoggothAttackP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_Moggoth_P1_TO_P2::Tag(), Queen_Moggoth_P1_TO_P2::Create(m_pDevice, m_pContext));

	Set_LoadingText(L"����Ѱ� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, Health::ProtoTag(),
		Health::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"��ü �ε� ��"); // ��ü�� �������� �ε��Ǿ���Ѵ�.
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CEditCamera::ProtoTag(), CEditCamera::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Sky::ProtoTag(), Sky::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmekh_Formacion_rocosa::ProtoTag(), Kemmekh_Formacion_rocosa::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmekh_wall::ProtoTag(), Kemmekh_wall::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Wall02::ProtoTag(), SM_Wall02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Wall01::ProtoTag(), SM_Wall01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_TrozoPared::ProtoTag(), SM_TrozoPared::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Relic::ProtoTag(), SM_Relic::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Pueblo02::ProtoTag(), SM_Pueblo02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Poblado01::ProtoTag(), SM_Poblado01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Poblado::ProtoTag(), SM_Poblado::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Piedras::ProtoTag(), SM_Piedras::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MultiLake02::ProtoTag(), SM_MultiLake02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MiniBujer::ProtoTag(), SM_MiniBujer::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Mina2::ProtoTag(), SM_Mina2::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MERGED_RailSupportB2_452::ProtoTag(), SM_MERGED_RailSupportB2_452::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MERGED_MultiRock371::ProtoTag(), SM_MERGED_MultiRock371::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LaMina::ProtoTag(), SM_LaMina::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LagoSmall::ProtoTag(), SM_LagoSmall::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_GenericWall01::ProtoTag(), SM_GenericWall01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_CrystalYRocas::ProtoTag(), SM_CrystalYRocas::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Crystal::ProtoTag(), SM_Crystal::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Barriletes::ProtoTag(), SM_Barriletes::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LargeVolcanicRock_002_Red_Desert::ProtoTag(), LargeVolcanicRock_002_Red_Desert::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MountainRock::ProtoTag(), SM_MountainRock::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MountainRock_Closed::ProtoTag(), SM_MountainRock_Closed::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder003::ProtoTag(), SM_LargePlainsBoulder003::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder002_Red_Desert::ProtoTag(), SM_LargePlainsBoulder002_Red_Desert::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder002::ProtoTag(), SM_LargePlainsBoulder002::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Cliff01::ProtoTag(), SM_Cliff01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CBlue_Snow::ProtoTag(), CBlue_Snow::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCube::ProtoTag(), CCube::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Bush01::ProtoTag(), Bush01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushA::ProtoTag(), BushA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushB::ProtoTag(), BushB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushC::ProtoTag(), BushC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Cage::ProtoTag(), Cage::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Cave::ProtoTag(), Cave::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterA::ProtoTag(), CraterA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterB::ProtoTag(), CraterB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterC::ProtoTag(), CraterC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmek_Hole::ProtoTag(), Kemmek_Hole::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsA::ProtoTag(), MineCrystalsA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsB::ProtoTag(), MineCrystalsB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsC::ProtoTag(), MineCrystalsC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsD::ProtoTag(), MineCrystalsD::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsE::ProtoTag(), MineCrystalsE::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsF::ProtoTag(), MineCrystalsF::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeA::ProtoTag(), TreeA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeB1::ProtoTag(), TreeB1::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeB2::ProtoTag(), TreeB2::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Clint::ProtoTag(), Clint::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ClintBasicBullet::ProtoTag(), ClintBasicBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ClintUltimate01Bullet::ProtoTag(), ClintUltimate01Bullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Pistola::ProtoTag(), Pistola::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Alien_prawn::ProtoTag(), Alien_prawn::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BatPotato_RIG::ProtoTag(), BatPotato_RIG::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BatPotato_RIGBullet::ProtoTag(), BatPotato_RIGBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CannonSpider::ProtoTag(), CannonSpider::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CannonSpiderBullet::ProtoTag(), CannonSpiderBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolem::ProtoTag(), CrystalGolem::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackAreaBullet::ProtoTag(), CrystalGolemAttackAreaBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackArea02Bullet::ProtoTag(), CrystalGolemAttackArea02Bullet::Create(m_pDevice, m_pContext)), E_FAIL);
	//FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackRangeBullet::ProtoTag(), CrystalGolemAttackRangeBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Spider::ProtoTag(), Spider::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SpiderBullet::ProtoTag(), SpiderBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Queen_Moggoth::ProtoTag(), Queen_Moggoth::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack01::ProtoTag(), P1Attack01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack02::ProtoTag(), P1Attack02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack03::ProtoTag(), P1Attack03::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack04::ProtoTag(), P1Attack04::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack01::ProtoTag(), P2Attack01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack02::ProtoTag(), P2Attack02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack03::ProtoTag(), P2Attack03::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack04::ProtoTag(), P2Attack04::Create(m_pDevice, m_pContext)), E_FAIL);
	Set_LoadingText(L"�ε� �Ϸ�");

	m_isFinished = true;
}

#ifdef _DEBUG
HRESULT CLoader::Loading_For_IMGUI()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	_matrix		PivotMatrix = XMMatrixIdentity();
	CModel* pModel;
	vector<_uint> upper;
	vector<_uint> lower;

	Set_LoadingText(L"�ؽ�ó �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Art/Misc/John/Materials/TexturesCom_RockyDirt_1024_B__3_.png", 1))), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Brush.png"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/SkyBox/Sky_%d.dds"), 4)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Snow/Snow.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_CustomUI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/CustomUI.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_UI_KS_Radar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Killsquad/GUI/Minimap/OldTextures/UI_KS_Radar.dds"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_ui_radar_arrow_icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Killsquad/GUI/Minimap/Textures/ui_radar_arrow_icon.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_ui_radar_player_icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Killsquad/GUI/Minimap/Textures/ui_radar_player_icon.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_LifeBar_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/LifeBar_BG.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_LifeBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/LifeBar.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_UI_dash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Killsquad/GUI/HUD/LifeHUD/Core/UI_dash.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_T_CircularMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Game/NewGUI/Buttons/Textures/ProgressBar/T_CircularMask.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_BasicAttack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/BasicAttack.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_QSkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/QSkill.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_WSkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/WSkill.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_ESkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/ESkill.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_RSkill"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/RSkill.png"), 1)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_ExpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Custom/ExpBar.png"), 1)), E_FAIL);

	Set_LoadingText(L"���� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("C:/KillSquad/Game/Art/Misc/John/Materials/TexturesCom_SmoothRock_1024_heightmap.bmp"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);


	CVIBuffer_Rect_Instance::INSTANCEDESC tVIBufferRectInstanceDesc;
	ZeroStruct(tVIBufferRectInstanceDesc);
	tVIBufferRectInstanceDesc.vRange = _float3(10.f, 4.f, 10.f); tVIBufferRectInstanceDesc.vSpeed = _uint2(2, 10);
	tVIBufferRectInstanceDesc.fLifeTime = 10.f; tVIBufferRectInstanceDesc.iNumInstance = 30; tVIBufferRectInstanceDesc.fHeight = 10.f;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Rect_Instance::ProtoTag(),
		CVIBuffer_Rect_Instance::Create(m_pDevice, m_pContext, &tVIBufferRectInstanceDesc)), E_FAIL);
	
	CVIBuffer_Point_Instance::INSTANCEDESC tVIBufferPointInstanceDesc;
	tVIBufferPointInstanceDesc.vRange = _float3(10.f, 4.f, 10.f); tVIBufferPointInstanceDesc.vSpeed = _uint2(2, 10);
	tVIBufferPointInstanceDesc.fLifeTime = 10.f; tVIBufferPointInstanceDesc.iNumInstance = 30; tVIBufferPointInstanceDesc.fHeight = 10.f;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Point_Instance::ProtoTag(),
		CVIBuffer_Point_Instance::Create(m_pDevice, m_pContext, &tVIBufferPointInstanceDesc)), E_FAIL);
	
	// Skeletal_Meshes
	Set_LoadingText(L"�� �ε� ��");
	cout << "--- Clint ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 3); pModel->LoadAssimp("Clint.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Clint/Animation.myanim"));
	upper.clear(); lower.clear();
	upper.push_back(7); upper.push_back(11); upper.push_back(12);
	lower.push_back(3); lower.push_back(4); lower.push_back(88); lower.push_back(94);
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Clint"), pModel), E_FAIL);

	cout << "--- Alien_prawn1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Alien_prawn1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn1"), pModel), E_FAIL);
	
	cout << "--- Alien_prawn2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawn2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn2"), pModel), E_FAIL);

	cout << "--- Alien_prawn3 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawn3.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawn3"), pModel), E_FAIL);

	cout << "--- Alien_prawnCharger ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawnCharger1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawnCharger1"), pModel), E_FAIL);

	cout << "--- Alien_prawnCharger2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Alien_prawnCharger2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Alien_prawn1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Alien_prawnCharger2"), pModel), E_FAIL);

	cout << "--- BatPotato_RIG1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("BatPotato_RIG1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/BatPotato_RIG1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BatPotato_RIG1"), pModel), E_FAIL);

	cout << "--- BatPotato_RIG2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("BatPotato_RIG2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/BatPotato_RIG1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BatPotato_RIG2"), pModel), E_FAIL);

	cout << "--- CannonSpider ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("CannonSpider.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/CannonSpider/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CannonSpider"), pModel), E_FAIL);


	cout << "--- CrystalGolem ---" << endl;
	PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("CrystalGolem.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/CrystalGolem/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CrystalGolem"), pModel), E_FAIL);

	cout << "--- MicroSpider1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("MicroSpider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MicroSpider1"), pModel), E_FAIL);


	cout << "--- MicroSpider2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("MicroSpider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MicroSpider2"), pModel), E_FAIL);

	cout << "--- Spider1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider1"), pModel), E_FAIL);


	cout << "--- Spider2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider2"), pModel), E_FAIL);


	cout << "--- Spider3 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Spider3.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Spider3"), pModel), E_FAIL);


	cout << "--- SummonSpider1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("SummonSpider1.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SummonSpider1"), pModel), E_FAIL);

	cout << "--- SummonSpider2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("SummonSpider2.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Spider1/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SummonSpider2"), pModel), E_FAIL);
	 
	cout << "--- Queen_Moggoth ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 2); pModel->LoadAssimp("Queen_Moggoth.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Queen_Moggoth/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
	, TEXT("Prototype_Component_Model_Queen_Moggoth"), pModel), E_FAIL);

	/*
			cout << "--- Kemmeth_Bird ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmeth_Bird.dat");
	pModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Kemmeth_Bird/Animation.myanim"));
	upper.clear(); lower.clear();
	pModel->GroupingBones(upper, lower);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmeth_Bird"), pModel), E_FAIL);
	*/


	// Static_Meshes
	cout << "--- SM_Cliff01 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Cliff01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Cliff01"), pModel), E_FAIL);

	cout << "--- SM_LargePlainsBoulder002 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder002.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder002"), pModel), E_FAIL);

	cout << "--- SM_LargePlainsBoulder002_Red_Desert ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder002_Red_Desert.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder002_Red_Desert"), pModel), E_FAIL);

	cout << "--- SM_LargePlainsBoulder003 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LargePlainsBoulder003.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LargePlainsBoulder003"), pModel), E_FAIL);


	cout << "--- SM_MountainRock ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MountainRock.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MountainRock"), pModel), E_FAIL);

	cout << "--- SM_MountainRock_Closed ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MountainRock_Closed.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MountainRock_Closed"), pModel), E_FAIL);


	cout << "--- Kemmekh_Formacion_rocosa ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmekh_Formacion_rocosa.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmekh_Formacion_rocosa"), pModel), E_FAIL);


	cout << "--- Kemmekh_wall ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmekh_wall.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmekh_wall"), pModel), E_FAIL);

	cout << "--- SM_Wall02 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Wall02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Wall02"), pModel), E_FAIL);


	cout << "--- SM_Wall01 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Wall01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Wall01"), pModel), E_FAIL);


	cout << "--- SM_TrozoPared ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_TrozoPared.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_TrozoPared"), pModel), E_FAIL);


	cout << "--- SM_Relic ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Relic.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Relic"), pModel), E_FAIL);


	cout << "--- SM_Pueblo02 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Pueblo02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Pueblo02"), pModel), E_FAIL);


	cout << "--- SM_Poblado01 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Poblado01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Poblado01"), pModel), E_FAIL);


	cout << "--- SM_Poblado ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Poblado.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Poblado"), pModel), E_FAIL);


	cout << "--- SM_Piedras ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Piedras.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Piedras"), pModel), E_FAIL);


	cout << "--- SM_MultiLake02 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MultiLake02.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MultiLake02"), pModel), E_FAIL);


	cout << "--- SM_MiniBujer ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MiniBujer.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MiniBujer"), pModel), E_FAIL);


	cout << "--- SM_Mina2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Mina2.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Mina2"), pModel), E_FAIL);


	cout << "--- SM_MERGED_RailSupportB2_452 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MERGED_RailSupportB2_452.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MERGED_RailSupportB2_452"), pModel), E_FAIL);


	cout << "--- SM_MERGED_MultiRock371 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_MERGED_MultiRock371.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_MERGED_MultiRock371"), pModel), E_FAIL);


	cout << "--- SM_LaMina ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LaMina.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LaMina"), pModel), E_FAIL);


	cout << "--- SM_LagoSmall ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_LagoSmall.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_LagoSmall"), pModel), E_FAIL);


	cout << "--- SM_GenericWall01 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_GenericWall01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_GenericWall01"), pModel), E_FAIL);


	cout << "--- SM_CrystalYRocas ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_CrystalYRocas.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_CrystalYRocas"), pModel), E_FAIL);


	cout << "--- SM_Crystal ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Crystal.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Crystal"), pModel), E_FAIL);


	cout << "--- SM_Barriletes ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SM_Barriletes.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SM_Barriletes"), pModel), E_FAIL);


	cout << "--- Bush01 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Bush01.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Bush01"), pModel), E_FAIL);

	cout << "--- BushA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushA"), pModel), E_FAIL);


	cout << "--- BushB ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushB"), pModel), E_FAIL);


	cout << "--- BushC ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BushC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BushC"), pModel), E_FAIL);


	cout << "--- Cage ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Cage.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Cage"), pModel), E_FAIL);


	cout << "--- Cave ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Cave.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Cave"), pModel), E_FAIL);


	cout << "--- CraterA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterA"), pModel), E_FAIL);

	cout << "--- CraterB ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterB"), pModel), E_FAIL);


	cout << "--- CraterC ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("CraterC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_CraterC"), pModel), E_FAIL);


	cout << "--- Kemmek_Hole ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Kemmek_Hole.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Kemmek_Hole"), pModel), E_FAIL);


	cout << "--- MineCrystalsA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsA"), pModel), E_FAIL);


	cout << "--- MineCrystalsB ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsB"), pModel), E_FAIL);


	cout << "--- MineCrystalsC ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsC"), pModel), E_FAIL);


	cout << "--- MineCrystalsD ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsD.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsD"), pModel), E_FAIL);


	cout << "--- MineCrystalsE ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsE.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsE"), pModel), E_FAIL);


	cout << "--- MineCrystalsF ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("MineCrystalsF.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_MineCrystalsF"), pModel), E_FAIL);


	cout << "--- TreeA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeA"), pModel), E_FAIL);


	cout << "--- TreeB1 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeB1.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeB1"), pModel), E_FAIL);


	cout << "--- TreeB2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TreeB2.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TreeB2"), pModel), E_FAIL);

	cout << "--- Pistola ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);// * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Pistola.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Pistola"), pModel), E_FAIL);

	lstrcpy(m_szLoading, TEXT("�׺���̼����� �ε� ��."));
	/* For.Prototype_COmpoentn_Navigation */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CNavigation::ProtoTag(),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/Navigation1.dat"))), E_FAIL);

	Set_LoadingText(L"���̴� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Shader_Navigation"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl")
			, VTXPOS_DECL::Elements, VTXPOS_DECL::iNumElements)), E_FAIL);

	Set_LoadingText(L"���� �ε� ��");

	cout << "Clint States" << endl;
	StateContext<Clint, CLINT_ANIM>* pClintState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_ClintState"),
		pClintState = StateContext<Clint, CLINT_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pClintState->Add_State(ClintIdle::Tag(), ClintIdle::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintRun::Tag(),  ClintRun::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintDash::Tag(), ClintDash::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintShoot::Tag(), ClintShoot::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintUltimate01::Tag(), ClintUltimate01::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintSkill01::Tag(), ClintSkill01::Create(m_pDevice, m_pContext));
	pClintState->Add_State(ClintSkill02::Tag(), ClintSkill02::Create(m_pDevice, m_pContext));

	cout << "Alien_prawn States" << endl;
	StateContext<Alien_prawn, ALIEN_PRAWN_ANIM>* pAlienPrawnState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_AlienPrawnState"),
		pAlienPrawnState = StateContext<Alien_prawn, ALIEN_PRAWN_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pAlienPrawnState->Add_State(Alien_prawnIdle::Tag(), Alien_prawnIdle::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnRun::Tag(), Alien_prawnRun::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnAttack::Tag(), Alien_prawnAttack::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnDead::Tag(), Alien_prawnDead::Create(m_pDevice, m_pContext));
	pAlienPrawnState->Add_State(Alien_prawnHit::Tag(), Alien_prawnHit::Create(m_pDevice, m_pContext));

	cout << "BatPotato_RIG States" << endl;
	StateContext<BatPotato_RIG, BATPOTATO_RIG_ANIM>* pBatPotatoState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_BatPotatoRIGState"),
		pBatPotatoState = StateContext<BatPotato_RIG, BATPOTATO_RIG_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pBatPotatoState->Add_State(BatPotato_RIGAttack::Tag(), BatPotato_RIGAttack::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGDeath::Tag(), BatPotato_RIGDeath::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGHit::Tag(), BatPotato_RIGHit::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGIdle::Tag(), BatPotato_RIGIdle::Create(m_pDevice, m_pContext));
	pBatPotatoState->Add_State(BatPotato_RIGRun::Tag(), BatPotato_RIGRun::Create(m_pDevice, m_pContext));

	cout << "CannonSpider States" << endl;
	StateContext<CannonSpider, CANNON_SPIDER_ANIM>* pCannonSpiderState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_CannonSpiderState"),
		pCannonSpiderState = StateContext<CannonSpider, CANNON_SPIDER_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pCannonSpiderState->Add_State(CannonSpiderAttack::Tag(), CannonSpiderAttack::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderDead::Tag(), CannonSpiderDead::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderHit::Tag(), CannonSpiderHit::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderIdle::Tag(), CannonSpiderIdle::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderRun::Tag(), CannonSpiderRun::Create(m_pDevice, m_pContext));
	pCannonSpiderState->Add_State(CannonSpiderSearch::Tag(), CannonSpiderSearch::Create(m_pDevice, m_pContext));

	cout << "CrystalGolem States" << endl;
	StateContext<CrystalGolem, CRYSTAL_GOLEM_ANIM>* pCrystalGolemState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_CrystalGolemState"),
		pCrystalGolemState = StateContext<CrystalGolem, CRYSTAL_GOLEM_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pCrystalGolemState->Add_State(CrystalGolemIdle::Tag(), CrystalGolemIdle::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemAppear::Tag(), CrystalGolemAppear::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemRun::Tag(), CrystalGolemRun::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemDead::Tag(), CrystalGolemDead::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemTurn180::Tag(), CrystalGolemTurn180::Create(m_pDevice, m_pContext));
	pCrystalGolemState->Add_State(CrystalGolemAttack::Tag(), CrystalGolemAttack::Create(m_pDevice, m_pContext));

	cout << "Spider States" << endl;
	StateContext<Spider, SPIDER_ANIM>* pSpiderState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_SpiderState"),
		pSpiderState = StateContext<Spider, SPIDER_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pSpiderState->Add_State(SpiderAttack::Tag(), SpiderAttack::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderDead::Tag(), SpiderDead::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderHit::Tag(), SpiderHit::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderIdle::Tag(), SpiderIdle::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderRun::Tag(), SpiderRun::Create(m_pDevice, m_pContext));
	pSpiderState->Add_State(SpiderAppear::Tag(), SpiderAppear::Create(m_pDevice, m_pContext));

	cout << "Queen_Moggoth States" << endl;
	StateContext<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* pQueenMoggothState;
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Queen_MoggothState"),
		pQueenMoggothState = StateContext<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>::Create(m_pDevice, m_pContext)), E_FAIL);
	pQueenMoggothState->Add_State(Queen_MoggothAppear::Tag(), Queen_MoggothAppear::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothAttack::Tag(), Queen_MoggothAttack::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothDead::Tag(), Queen_MoggothDead::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothIdle::Tag(), Queen_MoggothIdle::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothWalk::Tag(), Queen_MoggothWalk::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothWalkP2::Tag(), Queen_MoggothWalkP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothIdleP2::Tag(), Queen_MoggothIdleP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_MoggothAttackP2::Tag(), Queen_MoggothAttackP2::Create(m_pDevice, m_pContext));
	pQueenMoggothState->Add_State(Queen_Moggoth_P1_TO_P2::Tag(), Queen_Moggoth_P1_TO_P2::Create(m_pDevice, m_pContext));

	Set_LoadingText(L"����Ѱ� �ε� ��");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, Health::ProtoTag(),
		Health::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"��ü �ε� ��"); // ��ü�� �������� �ε��Ǿ���Ѵ�.
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CTerrain::ProtoTag(), CTerrain::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CEditCamera::ProtoTag(), CEditCamera::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Sky::ProtoTag(), Sky::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmekh_Formacion_rocosa::ProtoTag(), Kemmekh_Formacion_rocosa::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmekh_wall::ProtoTag(), Kemmekh_wall::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Wall02::ProtoTag(), SM_Wall02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Wall01::ProtoTag(), SM_Wall01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_TrozoPared::ProtoTag(), SM_TrozoPared::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Relic::ProtoTag(), SM_Relic::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Pueblo02::ProtoTag(), SM_Pueblo02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Poblado01::ProtoTag(), SM_Poblado01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Poblado::ProtoTag(), SM_Poblado::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Piedras::ProtoTag(), SM_Piedras::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MultiLake02::ProtoTag(), SM_MultiLake02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MiniBujer::ProtoTag(), SM_MiniBujer::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Mina2::ProtoTag(), SM_Mina2::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MERGED_RailSupportB2_452::ProtoTag(), SM_MERGED_RailSupportB2_452::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MERGED_MultiRock371::ProtoTag(), SM_MERGED_MultiRock371::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LaMina::ProtoTag(), SM_LaMina::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LagoSmall::ProtoTag(), SM_LagoSmall::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_GenericWall01::ProtoTag(), SM_GenericWall01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_CrystalYRocas::ProtoTag(), SM_CrystalYRocas::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Crystal::ProtoTag(), SM_Crystal::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Barriletes::ProtoTag(), SM_Barriletes::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LargeVolcanicRock_002_Red_Desert::ProtoTag(), LargeVolcanicRock_002_Red_Desert::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MountainRock::ProtoTag(), SM_MountainRock::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_MountainRock_Closed::ProtoTag(), SM_MountainRock_Closed::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder003::ProtoTag(), SM_LargePlainsBoulder003::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder002_Red_Desert::ProtoTag(), SM_LargePlainsBoulder002_Red_Desert::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_LargePlainsBoulder002::ProtoTag(), SM_LargePlainsBoulder002::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SM_Cliff01::ProtoTag(), SM_Cliff01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CBlue_Snow::ProtoTag(), CBlue_Snow::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCube::ProtoTag(), CCube::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Bush01::ProtoTag(), Bush01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushA::ProtoTag(), BushA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushB::ProtoTag(), BushB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushC::ProtoTag(), BushC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Cage::ProtoTag(), Cage::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Cave::ProtoTag(), Cave::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterA::ProtoTag(), CraterA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterB::ProtoTag(), CraterB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CraterC::ProtoTag(), CraterC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Kemmek_Hole::ProtoTag(), Kemmek_Hole::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsA::ProtoTag(), MineCrystalsA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsB::ProtoTag(), MineCrystalsB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsC::ProtoTag(), MineCrystalsC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsD::ProtoTag(), MineCrystalsD::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsE::ProtoTag(), MineCrystalsE::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(MineCrystalsF::ProtoTag(), MineCrystalsF::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeA::ProtoTag(), TreeA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeB1::ProtoTag(), TreeB1::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TreeB2::ProtoTag(), TreeB2::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Clint::ProtoTag(), Clint::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ClintBasicBullet::ProtoTag(), ClintBasicBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ClintUltimate01Bullet::ProtoTag(), ClintUltimate01Bullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Pistola::ProtoTag(), Pistola::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Alien_prawn::ProtoTag(), Alien_prawn::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BatPotato_RIG::ProtoTag(), BatPotato_RIG::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BatPotato_RIGBullet::ProtoTag(), BatPotato_RIGBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CannonSpider::ProtoTag(), CannonSpider::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CannonSpiderBullet::ProtoTag(), CannonSpiderBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolem::ProtoTag(), CrystalGolem::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackAreaBullet::ProtoTag(), CrystalGolemAttackAreaBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackArea02Bullet::ProtoTag(), CrystalGolemAttackArea02Bullet::Create(m_pDevice, m_pContext)), E_FAIL);
	//FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CrystalGolemAttackRangeBullet::ProtoTag(), CrystalGolemAttackRangeBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Spider::ProtoTag(), Spider::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SpiderBullet::ProtoTag(), SpiderBullet::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Queen_Moggoth::ProtoTag(), Queen_Moggoth::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack01::ProtoTag(), P1Attack01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack02::ProtoTag(), P1Attack02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack03::ProtoTag(), P1Attack03::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P1Attack04::ProtoTag(), P1Attack04::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack01::ProtoTag(), P2Attack01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack02::ProtoTag(), P2Attack02::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack03::ProtoTag(), P2Attack03::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(P2Attack04::ProtoTag(), P2Attack04::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Image::ProtoTag(), Image::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(DynamicImage::ProtoTag(), DynamicImage::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(FontUI::ProtoTag(), FontUI::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(PlayerLevel::ProtoTag(), PlayerLevel::Create(m_pDevice, m_pContext)), E_FAIL);

	Set_LoadingText(L"�ε� �Ϸ�");

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