#include "ImWindow.h"
#include "ImguiManager.h"
#include "GameInstance.h"
ImWindow::ImWindow()
	: m_bShowWindow(true)
	, m_pImguiMgr(ImguiManager::GetInstance())
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pImguiMgr);
	Safe_AddRef(m_pGameInstance);
}

void ImWindow::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pImguiMgr);
}
