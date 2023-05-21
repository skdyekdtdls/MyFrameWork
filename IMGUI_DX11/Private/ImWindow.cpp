#include "ImWindow.h"
#include "ImguiManager.h"
ImWindow::ImWindow()
	: m_bShowWindow(true)
	, m_pImguiMgr(ImguiManager::GetInstance())
{
	Safe_AddRef(m_pImguiMgr);
}

void ImWindow::Free()
{
	Safe_Release(m_pImguiMgr);
}
