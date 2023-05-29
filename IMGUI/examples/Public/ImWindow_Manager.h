#pragma once

#include "Imgui_Defines.h"
#include "Base.h"

BEGIN(Imgui)
class CImWindow;
class CImWindow_Manager final : public CBase
{
    DECLARE_SINGLETON(CImWindow_Manager);

private:
    explicit CImWindow_Manager();
    ~CImWindow_Manager() = default;

public:
    CImWindow* Get_ImWindow(const _tchar* tag);

public:
    HRESULT Initialize(__inout ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick();
    void Render();
    HRESULT Add_Window(const _tchar* tag, CImWindow* pWindow);

private:
    void Initialize_Imgui(ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

private:
    CImWindow* Find_Window(const _tchar* tag);

private:
    ImGuiIO* m_pIO = { nullptr };
    unordered_map<const _tchar*, CImWindow*>    m_ImWindows;
public:
    virtual void Free(void) override;
};

END
