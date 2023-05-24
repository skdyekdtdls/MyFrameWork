#include "ImWindow_Manager.h"
#include "ImWindow.h"

IMPLEMENT_SINGLETON(CImWindow_Manager);

CImWindow_Manager::CImWindow_Manager()
{
}

HRESULT CImWindow_Manager::Add_ImWindow(wstring tag, CImWindow* pImWindow)
{
    auto iter = m_ImWindows.find(tag.c_str());


    if (iter == m_ImWindows.end())
        return E_FAIL;

    {if (pImWindow == 0) { __debugbreak(); return E_FAIL; }}

    m_ImWindows.emplace(tag.c_str(), pImWindow);

    return S_OK;
}

void CImWindow_Manager::Update()
{
    for (auto& Pair : m_ImWindows)
        Pair.second->Update();
}

void CImWindow_Manager::Free(void)
{
    for (auto& Pair : m_ImWindows)
        Safe_Release(Pair.second);
}
