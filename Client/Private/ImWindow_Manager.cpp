#ifdef _DEBUG
#include "..\Public\ImWindow_Manager.h"
#include "GameInstance.h"
#include "EditCamera.h"
#include "ImWindow.h"

#include "ImWindow_ObjectTool.h"
#include "ImWindow_MapTool.h"
#include "ImWindow_CameraTool.h"
#include "ImWindow_AnimationTool.h"
#include "ImWindow_EffectTool.h"
#include "ImWindow_UITool.h"
#include "ImWindow_LightTool.h"

#include "ImWindow_Demo.h"
#include "ImWindow_SaveLoads.h"
#include "ImMode.h"

IMPLEMENT_SINGLETON(CImWindow_Manager)

CImWindow_Manager::CImWindow_Manager()
{
    
}

HRESULT CImWindow_Manager::Initialize(ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);
    Initialize_Imgui(pIO, pDevice, pContext);

    m_pObjectTool = CImWindow_ObjectTool::Create(m_pIO);
    m_pMapTool = CImWindow_MapTool::Create(m_pIO);
    m_pCameraTool = CImWindow_CameraTool::Create(m_pIO);
    m_pAnimationTool = CImWindow_AnimationTool::Create(m_pIO);
    m_pEffectTool = CImWindow_EffectTool::Create(m_pIO);
    m_pUITool = CImWindow_UITool::Create(m_pIO);
    m_pLightTool = CImWindow_LightTool::Create(m_pIO);

    m_pDemo =  CImWindow_Demo::Create(m_pIO);
    m_pSaveLoads = CImWindow_SaveLoads::Create(m_pIO);
    m_pImMode = CImMode::Create(m_pIO);

    m_pImMode->Set_Mode(OBJ_TOOL_MODE);

    Safe_Release(pGameInstance);
    return S_OK;
}

void CImWindow_Manager::Tick()
{
    if (m_bStart)
    {
        CGameInstance* pGameInstance = CGameInstance::GetInstance();
        Safe_AddRef(pGameInstance);
        m_pEditCamera = (CEditCamera*)pGameInstance->Get_GameObject(LEVEL_IMGUI, L"Layer_Camera", "EditCamera");
        NULL_CHECK(m_pEditCamera);
        Safe_Release(pGameInstance);
        m_bStart = false;
    }

    if (m_pImMode->Get_Mode() != m_pImMode->Get_PreMode())
    {
        switch (m_pImMode->Get_Mode())
        {
        case OBJ_TOOL_MODE:
            Set_Window(m_pObjectTool);
            break;
        case MAP_TOOL_MODE:
            Set_Window(m_pMapTool);
            break;
        case CAMERA_TOOL_MODE:
            Set_Window(m_pCameraTool);
            break;
        case ANIM_TOOL_MODE:
            Set_Window(m_pAnimationTool);
            break;
        case EFFECT_TOOL_MODE:
            Set_Window(m_pEffectTool);
            break;
        case UI_TOOL_MODE:
            Set_Window(m_pUITool);
            break;
        case LIGHT_TOOL_MODE:
            Set_Window(m_pLightTool);
            break;
        }

        m_pImMode->Set_PreMode(m_pImMode->Get_Mode());
    }

    m_pCurrentImWindow->Tick();
    m_pDemo->Tick();
    m_pSaveLoads->Tick();
    m_pImMode->Tick();
}

void CImWindow_Manager::LateTick()
{
    if(nullptr != m_pCurrentImWindow)
        m_pCurrentImWindow->LateTick();
    m_pDemo->LateTick();
    m_pSaveLoads->LateTick();
    m_pImMode->LateTick();
}

void CImWindow_Manager::Render()
{
    // Rendering
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Tick and Render additional Platform Windows
    if (m_pIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void CImWindow_Manager::Set_Mode(IMWIN_MODE eMode)
{
    m_pImMode->Set_Mode(eMode);
    if (eMode == MAP_TOOL_MODE)
        m_pMapTool->ResetClickCout();
}

void CImWindow_Manager::Set_PreMode(IMWIN_MODE ePreMode)
{
    m_pImMode->Set_Mode(ePreMode);
}

void CImWindow_Manager::Initialize_Imgui(ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    // Show the window
    ::ShowWindow(g_hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(g_hWnd);
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    *pIO = &io;
    m_pIO = *pIO;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //g_io.ConfigViewportsNoAutoMerge = true;
    //g_io.ConfigViewportsNoTaskBarIcon = true;
    //g_io.ConfigViewportsNoDefaultParent = true;
    //g_io.ConfigDockingAlwaysTabBar = true;
    //g_io.ConfigDockingTransparentPayload = true;
    //g_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //g_io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(pDevice, pContext);
}

PICK_DESC CImWindow_Manager::GetMinDistPickDesc()
{
    return m_pEditCamera->GetMinDistPickDesc();
}

PICK_DESC CImWindow_Manager::GetTerrainPickDesc()
{
    return m_pEditCamera->GetTerrainPickDesc();
}

const _bool& CImWindow_Manager::IsPicking()
{
    if (nullptr == m_pEditCamera)
        return false;
    return m_pEditCamera->IsPicking();
}

void CImWindow_Manager::AddItems(const char* strItem)
{
    m_pMapTool->AddItems(strItem);
}

IMWIN_MODE CImWindow_Manager::Get_Mode()
{
    return m_pImMode->Get_Mode();
}

IMWIN_MODE CImWindow_Manager::Get_PreMode()
{
    return m_pImMode->Get_PreMode();
}

void CImWindow_Manager::Free(void)
{
    Safe_Release(m_pEditCamera);

    Safe_Release(m_pObjectTool);
    Safe_Release(m_pMapTool);
    Safe_Release(m_pCameraTool);
    Safe_Release(m_pAnimationTool);
    Safe_Release(m_pEffectTool);
    Safe_Release(m_pUITool);
    Safe_Release(m_pLightTool);

    Safe_Release(m_pDemo);
    Safe_Release(m_pSaveLoads);
    Safe_Release(m_pImMode);
}
#endif