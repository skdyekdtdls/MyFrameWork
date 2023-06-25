#ifdef _USE_IMGUI
#include "ImWindow_CameraTool.h"

CImWindow_CameraTool::CImWindow_CameraTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_CameraTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_CameraTool::Tick()
{

}
CImWindow_CameraTool* CImWindow_CameraTool::Create(ImGuiIO* pIO)
{
	CImWindow_CameraTool* pInstance = new CImWindow_CameraTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_CameraTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_CameraTool::Free()
{
	__super::Free();
}
#endif
