#include "ToolInstance.h"
#include "ImWindow_Manager.h"
#include "GameInstance.h"
#include "Level_Tool.h"
IMPLEMENT_SINGLETON(CToolInstance);

CToolInstance::CToolInstance()
{
    m_pImWindow_Manager = CImWindow_Manager::GetInstance();
    m_pGameInstace = CGameInstance::GetInstance();
    Safe_AddRef(m_pImWindow_Manager);
}

HRESULT CToolInstance::Initialize()
{
    // Initialize Engine
    GRAPHICDESC GraphicDesc;
    GraphicDesc.eWinMode = GRAPHICDESC::WM_WIN;
    GraphicDesc.hWnd = g_hWnd;
    GraphicDesc.hInst = g_hInst;
    GraphicDesc.iViewportSizeX = g_iWinSizeX;
    GraphicDesc.iViewportSizeY = g_iWinSizeY;

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    FAILED_CHECK_RETURN(pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext), E_FAIL);

    pGameInstance->Open_Level(LEVEL_TOOL, CLevel_Tool::Create(m_pDevice, m_pContext));

    FAILED_CHECK_RETURN(Ready_Prototype_Component(), E_FAIL);

    m_pImWindow_Manager->Initialize(&m_pIO, m_pDevice, m_pContext);
    
    Safe_Release(pGameInstance);
    return S_OK;
}

void CToolInstance::Tick(_double TimeDelta)
{
    m_pImWindow_Manager->Tick();

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);
    
    pGameInstance->Tick_Engine(TimeDelta);

    Safe_Release(pGameInstance);
}


HRESULT CToolInstance::Render()
{
    if (nullptr == m_pRenderer)
        return E_FAIL;

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
    pGameInstance->Clear_DepthStencil_View();
    
    m_pImWindow_Manager->Render();

    FAILED_CHECK_RETURN(m_pRenderer->Draw_RenderGroup(), E_FAIL);

    pGameInstance->Present();

    Safe_Release(pGameInstance);
    return S_OK;
}

CImWindow* CToolInstance::Get_ImWindow(const _tchar* tag)
{
    return m_pImWindow_Manager->Get_ImWindow(tag);
}

HRESULT CToolInstance::Add_Window(const _tchar* tag, CImWindow* pWindow)
{
    return m_pImWindow_Manager->Add_Window(tag, pWindow);
}

HRESULT CToolInstance::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
    return m_pGameInstace->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

HRESULT CToolInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
    return m_pGameInstace->Add_Prototype(pPrototypeTag, pPrototype);
}

HRESULT CToolInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
    return m_pGameInstace->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
}

HRESULT CToolInstance::Ready_Prototype_Component()
{
    if (FAILED(m_pGameInstace->Add_Prototype(LEVEL_STATIC, CRenderer::ProtoTag(), m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstace->Add_Prototype(LEVEL_TOOL, CTransform::ProtoTag(), CTransform::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    Safe_AddRef(m_pRenderer);
    return S_OK;
}

void CToolInstance::Release_Tool()
{
    CImWindow_Manager::DestroyInstance();
    CToolInstance::DestroyInstance();

    CGameInstance::Release_Engine();
}

void CToolInstance::Free(void)
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pRenderer);
    Safe_Release(m_pImWindow_Manager);
}
