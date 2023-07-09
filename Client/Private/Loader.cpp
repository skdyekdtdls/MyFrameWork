#include "..\Public\Loader.h"
#include "GameInstance.h"
#include <process.h>
#include "BackGround.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
#include "Model.h"
#include "ForkLift.h"
#include "Fiona.h"
#include "Clint.h"
#include "BarrelBoxStool.h"
#include "Bush01.h"
#include "BushA.h"
#include "BushB.h"
#include "BushC.h"
#include "Cage.h"
#include "KemmekhBridgeA.h"
#include "KemmekhBridgeD.h"
#include "KemmekhBridgeF.h"
#include "WheelBarrow.h"
#include "WatchTowerLadder.h"
#include "Wagon2.h"
#include "TunnelEntryWood.h"
#include "Tong.h"
#include "SmallRockC.h"
#include "Rock.h"
#include "RailSupportB.h"
#include "RailSupportA.h"
#include "Pistola.h"

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

	/*PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Model_Fiona"),
		CModel::Create(m_pDevice, m_pContext, TEXT("Fiona.dat"), PivotMatrix)), E_FAIL);

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Model_ForkLift"),
		CModel::Create(m_pDevice, m_pContext, TEXT("ForkLift.dat"), PivotMatrix)), E_FAIL);*/

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
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCamera_Free::ProtoTag(), CCamera_Free::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(CCube::ProtoTag(), CCube::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ForkLift::ProtoTag(), ForkLift::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Fiona::ProtoTag(), Fiona::Create(m_pDevice, m_pContext)), E_FAIL);
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
	CModel* pModel;

	Set_LoadingText(L"텍스처 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Tile%d.dds", 2))), E_FAIL);

	Set_LoadingText(L"버퍼 로딩 중");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Terrain::ProtoTag(),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../Resources/Textures/Terrain/Height.bmp"))), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);
	
	Set_LoadingText(L"모델 로딩 중");
	cout << "--- Clint ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(-90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	CModel* pCModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 3); pCModel->LoadAssimp("Clint.dat");
	pCModel->Late_Initialize(TEXT("../../Resources/Skeletal_Mesh/Clint/Animation.myanim"));
	pCModel->GroupingBones();
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Clint"), pCModel), E_FAIL);

	cout << "--- Fiona ---" << endl;
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix, 1); pModel->LoadAssimp("Fiona.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Fiona"), pModel), E_FAIL);

	cout << "--- ForkLift ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("ForkLift.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_ForkLift"), pModel), E_FAIL);

	cout << "--- BarrelBoxStool ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("BarrelBoxStool.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_BarrelBoxStool"), pModel), E_FAIL);

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

	cout << "--- KemmekhBridgeA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("KemmekhBridgeA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_KemmekhBridgeA"), pModel), E_FAIL);

	cout << "--- KemmekhBridgeD ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("KemmekhBridgeD.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_KemmekhBridgeD"), pModel), E_FAIL);

	cout << "--- KemmekhBridgeF ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("KemmekhBridgeF.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_KemmekhBridgeF"), pModel), E_FAIL);

	cout << "--- RailSupportA ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("RailSupportA.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_RailSupportA"), pModel), E_FAIL);

	cout << "--- RailSupportB ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("RailSupportB.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_RailSupportB"), pModel), E_FAIL);

	cout << "--- Rock ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Rock.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Rock"), pModel), E_FAIL);

	cout << "--- SmallRockC ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("SmallRockC.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_SmallRockC"), pModel), E_FAIL);

	cout << "--- Tong ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Tong.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Tong"), pModel), E_FAIL);

	cout << "--- TunnelEntryWood ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("TunnelEntryWood.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_TunnelEntryWood"), pModel), E_FAIL);

	cout << "--- Wagon2 ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Wagon2.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Wagon2"), pModel), E_FAIL);

	cout << "--- WatchTowerLadder ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("WatchTowerLadder.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_WatchTowerLadder"), pModel), E_FAIL);

	cout << "--- WheelBarrow ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("WheelBarrow.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_WheelBarrow"), pModel), E_FAIL);

	cout << "--- Pistola ---" << endl;
	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);// * XMMatrixRotationY(XMConvertToRadians(180.0f));
	pModel = CModel::Create(m_pDevice, m_pContext, PivotMatrix); pModel->LoadAssimp("Pistola.dat");
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel
		, TEXT("Prototype_Component_Model_Pistola"), pModel), E_FAIL);

	lstrcpy(m_szLoading, TEXT("네비게이션정보 로딩 중."));
	/* For.Prototype_COmpoentn_Navigation */
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(m_eNextLevel, CNavigation::ProtoTag(),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/Data/Navigation/Navigation1.dat"))), E_FAIL);

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
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(ForkLift::ProtoTag(), ForkLift::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BarrelBoxStool::ProtoTag(), BarrelBoxStool::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Bush01::ProtoTag(), Bush01::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushA::ProtoTag(), BushA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushB::ProtoTag(), BushB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(BushC::ProtoTag(), BushC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Cage::ProtoTag(), Cage::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(KemmekhBridgeA::ProtoTag(), KemmekhBridgeA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(KemmekhBridgeD::ProtoTag(), KemmekhBridgeD::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(KemmekhBridgeF::ProtoTag(), KemmekhBridgeF::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(WheelBarrow::ProtoTag(), WheelBarrow::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(WatchTowerLadder::ProtoTag(), WatchTowerLadder::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Wagon2::ProtoTag(), Wagon2::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(TunnelEntryWood::ProtoTag(), TunnelEntryWood::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Tong::ProtoTag(), Tong::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(SmallRockC::ProtoTag(), SmallRockC::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Rock::ProtoTag(), Rock::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(RailSupportB::ProtoTag(), RailSupportB::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(RailSupportA::ProtoTag(), RailSupportA::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Fiona::ProtoTag(), Fiona::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Clint::ProtoTag(), Clint::Create(m_pDevice, m_pContext)), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(Pistola::ProtoTag(), Pistola::Create(m_pDevice, m_pContext)), E_FAIL);

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