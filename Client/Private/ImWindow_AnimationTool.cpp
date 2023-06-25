#ifdef _USE_IMGUI
#include "ImWindow_AnimationTool.h"

CImWindow_AnimationTool::CImWindow_AnimationTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_AnimationTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_AnimationTool::Tick()
{

}
CImWindow_AnimationTool* CImWindow_AnimationTool::Create(ImGuiIO* pIO)
{
	CImWindow_AnimationTool* pInstance = new CImWindow_AnimationTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_AnimationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_AnimationTool::Free()
{
	__super::Free();
}
#endif
