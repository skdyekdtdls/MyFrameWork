#include "..\Public\MainTool.h"
#include "Level_Tool.h"
#include "ImWindow_Demo.h"
CMainTool::CMainTool()
    : m_pGameInstance(CGameInstance::GetInstance())
    , m_pImWindow_Manager(CImWindow_Manager::GetInstance())
{
    //Safe_AddRef(m_pImWindow_Manager);
    //Safe_AddRef(m_pGameInstance);
}

HRESULT CMainTool::Initialize()
{
    // Initialize Engine
    GRAPHICDESC GraphicDesc;
    GraphicDesc.eWinMode = GRAPHICDESC::WM_WIN;
    GraphicDesc.hWnd = g_hWnd;
    GraphicDesc.iViewportSizeX = g_iWinSizeX;
    GraphicDesc.iViewportSizeY = g_iWinSizeY;
    
    FAILED_CHECK_RETURN(m_pGameInstance->Initialize_Engine(LEVEL_END, GraphicDesc, &m_pDevice, &m_pContext), E_FAIL);

    // Initialize Imgui
    m_pImWindow_Manager->Initialize(&m_pIO, m_pDevice, m_pContext);

    // Initialize Level
    FAILED_CHECK_RETURN(m_pGameInstance->Open_Level(LEVEL_TOOL, CLevel_Tool::Create(m_pDevice, m_pContext)), E_FAIL);

    // Intialize Compoent
    //Ready_Prototype_Component();

    // Intialize GameObject();
    //Ready_Prototype_GameObject();

    return S_OK;
}

void CMainTool::Update_Demo()
{
   
}

void CMainTool::Tick(_double TimeDelta)
{
    m_pImWindow_Manager->Tick();
    Update_Demo();
    m_pGameInstance->Tick_Engine(TimeDelta);
}

HRESULT CMainTool::Render()
{
    CImWindow_Demo* demo = static_cast<CImWindow_Demo*>(m_pImWindow_Manager->Get_ImWindow(L"CImWindow_Demo"));

    m_pGameInstance->Clear_BackBuffer_View(demo->clear_color);
    m_pGameInstance->Clear_DepthStencil_View();
    if(m_pRenderer != nullptr)
        m_pRenderer->Draw_RenderGroup();

    m_pImWindow_Manager->Render();

    m_pGameInstance->Present();
    return S_OK;
}



CMainTool* CMainTool::Create()
{
    CMainTool* pInstance = New CMainTool;

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created CMainTool");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMainTool::Free()
{
    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pRenderer);
    //Safe_Release(m_pImWindow_Manager);
    //Safe_Release(m_pGameInstance);

    CImWindow_Manager::DestroyInstance();
    CGameInstance::Release_Engine();
}
