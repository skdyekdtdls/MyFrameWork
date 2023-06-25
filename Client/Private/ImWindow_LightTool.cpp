#ifdef _USE_IMGUI
#include "ImWindow_LightTool.h"

CImWindow_LightTool::CImWindow_LightTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_LightTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_LightTool::Tick()
{

}
CImWindow_LightTool* CImWindow_LightTool::Create(ImGuiIO* pIO)
{
	CImWindow_LightTool* pInstance = new CImWindow_LightTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_LightTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_LightTool::Free()
{
	__super::Free();
}
#endif
