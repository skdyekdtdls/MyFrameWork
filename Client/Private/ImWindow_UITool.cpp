#ifdef _USE_IMGUI
#include "ImWindow_UITool.h"

CImWindow_UITool::CImWindow_UITool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_UITool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_UITool::Tick()
{

}
CImWindow_UITool* CImWindow_UITool::Create(ImGuiIO* pIO)
{
	CImWindow_UITool* pInstance = new CImWindow_UITool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_UITool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_UITool::Free()
{
	__super::Free();
}
#endif
