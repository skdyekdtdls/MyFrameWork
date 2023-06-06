#include "MainApp.h"
#include "GameInstance.h"
#include "Level.h"
#include "Level_Loading.h"

#ifdef _USE_IMGUI
#include "ImWindow_Manager.h"
#endif // _USE_IMGUI

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC GraphicDesc;
	ZeroStruct(GraphicDesc);

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.hInst = g_hInst;
	GraphicDesc.iViewportSizeX = g_iWinSizeX;
	GraphicDesc.iViewportSizeY = g_iWinSizeY;
	GraphicDesc.eWinMode = GraphicDesc.WM_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

#ifdef _USE_IMGUI
	CImWindow_Manager::GetInstance()->Initialize(&m_pIO, m_pDevice, m_pContext);
#endif

	if (FAILED(Ready_Prototype_Component_For_Static()))
		return E_FAIL;
#ifdef _USE_IMGUI
	if (FAILED(Open_Level(LEVEL_IMGUI)))
		return E_FAIL;
#else
	if(FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
#endif
	return S_OK;
}

void CMainApp::Tick(_double TimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;
#ifdef _USE_IMGUI
	CImWindow_Manager::GetInstance()->Tick();
#endif
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
#ifdef _USE_IMGUI
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

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Vtxtex"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Vtxtex.hlsl"
			, VTXPOSTEX_DECL::Elements, VTXPOSTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl")
			, VTXPOSNORTEX_DECL::Elements, VTXPOSNORTEX_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCol"),
		CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxCol.hlsl"
			, VTXPOSCOL_DECL::Elements, VTXPOSCOL_DECL::iNumElements)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Rect::ProtoTag(),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, CVIBuffer_Cube::ProtoTag(),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext)), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext)), E_FAIL);

	Safe_AddRef(m_pRenderer);

	return S_OK;
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
//
void CMainApp::Free()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
#ifdef _USE_IMGUI
	CImWindow_Manager::DestroyInstance();
#endif // _USE_IMGUI

	CGameInstance::Release_Engine();
}
