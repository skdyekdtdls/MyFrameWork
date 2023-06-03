#include "..\Public\ImWindow_Manager.h"
#include "ImWindow.h"
#include "ImWindow_Demo.h"
IMPLEMENT_SINGLETON(CImWindow_Manager)

CImWindow_Manager::CImWindow_Manager()
{
}

CImWindow* CImWindow_Manager::Get_ImWindow(const _tchar* tag)
{
    return Find_Window(tag);
}

HRESULT CImWindow_Manager::Initialize(ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Initialize_Imgui(pIO, pDevice, pContext);
    
    Add_Window(L"CImWindow_Demo", CImWindow_Demo::Create(m_pIO));
    return S_OK;
}

void CImWindow_Manager::Tick()
{
    for (auto ImWindow : m_ImWindows)
        ImWindow.second->Tick();
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

HRESULT CImWindow_Manager::Add_Window(const _tchar* tag, CImWindow* pWindow)
{
    if (nullptr == pWindow)
        return E_FAIL;

    CImWindow* pImWindow = Find_Window(tag);

    if (nullptr != pImWindow)
        return E_FAIL;

    m_ImWindows.emplace(tag, pWindow);

    return S_OK;
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

CImWindow* CImWindow_Manager::Find_Window(const _tchar* tag)
{
    auto iter = find_if(m_ImWindows.begin(), m_ImWindows.end(), CTag_Finder(tag));

    if(iter == m_ImWindows.end())
        return nullptr;

    return iter->second;
}

void CImWindow_Manager::Free(void)
{
    for (auto& ImWindow : m_ImWindows)
        Safe_Release(ImWindow.second);
    m_ImWindows.clear();
}
