#ifdef _USE_IMGUI
#include "ImWindow_EffectTool.h"

CImWindow_EffectTool::CImWindow_EffectTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_EffectTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_EffectTool::Tick()
{

}
CImWindow_EffectTool* CImWindow_EffectTool::Create(ImGuiIO* pIO)
{
	CImWindow_EffectTool* pInstance = new CImWindow_EffectTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_EffectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_EffectTool::Free()
{
	__super::Free();
}
#endif
