#include "MainApp.h"
#include "GameInstance.h"
#include "SoundMgr.h"
#include "Level.h"
#include "Level_Loading.h"
#include "ClientInstance.h"
#include "CannonSpiderBullet.h"
#include "BatPotato_RIGBullet.h"
#include "ClintBasicBullet.h"

// 몬스터풀
#include "Alien_prawn.h"
#include "BatPotato_RIG.h"
#include "CannonSpider.h"
#include "Spider.h"

#include "CrystalGolemAttackAreaBullet.h"
#include "CrystalGolemAttackArea02Bullet.h"
#include "CrystalGolemAttackRangeBullet.h"
#include "SpiderBullet.h"
#include "P1Attack01.h"
#include "P1Attack02.h"
#include "P1Attack03.h"
#include "P1Attack04.h"
#include "P2Attack01.h"
#include "P2Attack02.h"
#include "P2Attack03.h"
#include "P2Attack04.h"
#include "Image.h"
#include "CustomMouse.h"

#ifdef _DEBUG
#include "ImWindow_Manager.h"
#endif // DEBUG

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ShowCursor(false);

	GRAPHICDESC GraphicDesc;
	ZeroStruct(GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iViewportSizeX = 1264;
	GraphicDesc.iViewportSizeY = 681;
	GraphicDesc.eWinMode = GraphicDesc.WM_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->Initialize(&m_pIO, m_pDevice, m_pContext);
#endif
	CSoundMgr::Get_Instance()->Initialize();
	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Open_Level(LEVEL_IMGUI)))
		return E_FAIL;
#else
	if(FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
#endif
	//if (FAILED(Ready_Gara()))
	//	return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

	Ready_Pool();
	

	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	//Calc_FPS(TimeDelta);
	m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainApp::Render()
{
	if (nullptr == m_pGameInstance ||
		nullptr == m_pRenderer)
	{
		assert(false);
		return E_FAIL;
	}

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Draw_RenderGroup();

	// 미우스 추적하는 폰트 띄우는 코드
	//::POINT	ptMouse{};
	//GetCursorPos(&ptMouse);
	//ScreenToClient(g_hWnd, &ptMouse);
	//_uint2 WinSize = m_pGameInstance->GetViewPortSize(m_pContext);
	//wstring tmp = to_wstring(ptMouse.x) + L" " + to_wstring(WinSize.y - ptMouse.y);
	//m_pGameInstance->Render_Font(L"Font_135", tmp.c_str(), _float2(ptMouse.x, ptMouse.y), XMVectorSet(1.0, 0.4118, 0.7059, 1.f));

	//Render_FPS();
#ifdef _DEBUG
	CImWindow_Manager::GetInstance()->Render();
#endif
	// TODO
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_For_Static()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		m_pRenderer = CRenderer::Create(m_pDevice, m_pContext)), E_FAIL);

	// 셰이더 준비
	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"
			, VTXANIMMESH_DECL::Elements, VTXANIMMESH_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxBullet01"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxBullet01.hlsl"
			, VTXPOINTINSTANCE_DECL::Elements, VTXPOINTINSTANCE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCol"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCol.hlsl"
			, VTXPOSCOL_DECL::Elements, VTXPOSCOL_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCube.hlsl"
			, VTXCUBE_DECL::Elements, VTXCUBE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh.hlsl"
			, VTXMESH_DECL::Elements, VTXMESH_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl")
			, VTXPOSNORTEX_DECL::Elements, VTXPOSNORTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl")
			, VTXPOINTINSTANCE_DECL::Elements, VTXPOINTINSTANCE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Vtxtex"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Vtxtex.hlsl"
			, VTXPOSTEX_DECL::Elements, VTXPOSTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTexInstance.hlsl")
			, VTXRECTINSTANCE_DECL::Elements, VTXRECTINSTANCE_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Rect::ProtoTag(),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Sphere::ProtoTag(),
		CVIBuffer_Sphere::Create(m_pDevice, m_pContext, 48, 24, 1.f)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_DynamicRect::ProtoTag(),
		CVIBuffer_DynamicRect::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CTransform::ProtoTag(),
		CTransform::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CTransform2D::ProtoTag(),
		CTransform2D::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CColliderAABB::ProtoTag(),
		CColliderAABB::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CColliderSphere::ProtoTag(),
		CColliderSphere::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, Raycast::ProtoTag(),
		Raycast::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TimeCounter::ProtoTag(),
		TimeCounter::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, Observer::ProtoTag(),
		Observer::Create(m_pDevice, m_pContext)), E_FAIL);

	Safe_AddRef(m_pRenderer);

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pContext, TEXT("Font_135"), TEXT("../../Resources/Fonts/135ex.spritefont"))))
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}	

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	_float3		vPoints[3];

	_ulong		dwByte = { 0 };
	HANDLE		hFile = { 0 };

	hFile = CreateFile(TEXT("../Bin/Data/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	vPoints[2] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.f, 0.f, 0.f);
	vPoints[0] = _float3(0.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(0.0f, 0.f, 5.f);
	vPoints[1] = _float3(5.f, 0.f, 5.f);
	vPoints[0] = _float3(5.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(0.0f, 0.f, 10.0f);
	vPoints[1] = _float3(5.f, 0.f, 5.f);
	vPoints[0] = _float3(0.f, 0.f, 5.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[2] = _float3(5.f, 0.f, 5.f);
	vPoints[1] = _float3(10.f, 0.f, 0.f);
	vPoints[0] = _float3(5.f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Ready_Pool()
{
	ObjectPool<class ClintBasicBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class BatPotato_RIGBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CannonSpiderBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackAreaBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackArea02Bullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CrystalGolemAttackRangeBullet>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P1Attack01>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P1Attack02>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P1Attack03>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P1Attack04>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P2Attack01>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P2Attack02>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P2Attack03>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class P2Attack04>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class Alien_prawn>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class BatPotato_RIG>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class CannonSpider>::GetInstance()->Initialize(m_pDevice, m_pContext);
	ObjectPool<class Spider>::GetInstance()->Initialize(m_pDevice, m_pContext);

	return S_OK; // 풀 이니셜 후 해제.
}

void CMainApp::Calc_FPS(_double TimeDelta)
{
	m_TimeAcc += TimeDelta;
	++m_iCount;
	if (m_TimeAcc >= 1.f)
	{
		m_FPS = m_iCount;
		m_iCount = 0;
		m_TimeAcc = 0.0;
	}
}

void CMainApp::Render_FPS()
{
	wstring tmp = L"FPS : " + to_wstring(m_FPS);
	m_pGameInstance->Render_Font(L"Font_135", tmp.c_str(), _float2(0, 0));
}

HRESULT CMainApp::Open_Level(LEVELID eLevelIndex)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	return m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelIndex));
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CMainApp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	ObjectPool<class ClintBasicBullet>::DestroyInstance();
	ObjectPool<class BatPotato_RIGBullet>::DestroyInstance();
	ObjectPool<class CannonSpiderBullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackAreaBullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackArea02Bullet>::DestroyInstance();
	ObjectPool<class CrystalGolemAttackRangeBullet>::DestroyInstance();
	ObjectPool<class P1Attack01>::DestroyInstance();
	ObjectPool<class P1Attack02>::DestroyInstance();
	ObjectPool<class P1Attack03>::DestroyInstance();
	ObjectPool<class P1Attack04>::DestroyInstance();
	ObjectPool<class P2Attack01>::DestroyInstance();
	ObjectPool<class P2Attack02>::DestroyInstance();
	ObjectPool<class P2Attack03>::DestroyInstance();
	ObjectPool<class P2Attack04>::DestroyInstance();
	ObjectPool<class Alien_prawn>::DestroyInstance();
	ObjectPool<class BatPotato_RIG>::DestroyInstance();
	ObjectPool<class CannonSpider>::DestroyInstance();
	ObjectPool<class Spider>::DestroyInstance();
#ifdef _DEBUG
	CImWindow_Manager::DestroyInstance();
#endif // DEBUG
	ClientInstance::DestroyInstance();
	CSoundMgr::Destroy_Instance();
	CGameInstance::Release_Engine();
}
