#include "ImguiManager.h"
#include "GameInstance.h"
IMPLEMENT_SINGLETON(ImguiManager)

ImguiManager::ImguiManager()
    : m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT ImguiManager::Add_ImWindow(const _tchar* tag, ImWindow* pImWindow)
{
    auto iter = Find_ImWindow(tag);

    if (iter != nullptr)
        return E_FAIL;

    m_ImWindows.emplace(tag, pImWindow);

    return S_OK;
}

HRESULT ImguiManager::Tick(_double TimeDelta)
{
    for (auto& ImWindow : m_ImWindows)
    {
        if (FAILED(ImWindow.second->Tick(TimeDelta)))
        {
            return E_FAIL;
        }
    }

	return S_OK;
}

ImWindow* ImguiManager::Find_ImWindow(const _tchar* tag)
{
    auto iter = find_if(m_ImWindows.begin(), m_ImWindows.end(), CTag_Finder(tag));

    if (iter == m_ImWindows.end())
        return nullptr;

    return iter->second;
}

void ImguiManager::Free()
{
    for (auto& Pair: m_ImWindows)
    {
        Safe_Release(Pair.second);
    }
    m_ImWindows.clear();
    Safe_Release(m_pGameInstance);
}
