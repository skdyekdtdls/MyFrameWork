#ifdef _USE_IMGUI
#include "ImWindow_ObjectTool.h"

CImWindow_ObjectTool::CImWindow_ObjectTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_ObjectTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_ObjectTool::Tick()
{
	ImGui::Begin("ObjectTool");
	ImGui::PushItemWidth(200);
	ImGui::ListBox("Prototype\n(single select)",
		&Prototype_item_current,
		VectorGetter,
		static_cast<void*>(&Prototype_items),
		Prototype_items.size(),
		8); ImGui::SameLine();
	ImGui::PopItemWidth();
	ImGui::PushItemWidth(200);
	ImGui::ListBox("Hierachy\n(single select)",
		&Hierachy_item_current,
		VectorGetter,
		static_cast<void*>(&Hierachy_itesm),
		Hierachy_itesm.size(),
		8); 
	ImGui::PopItemWidth();
	ImGui::End();
}
CImWindow_ObjectTool* CImWindow_ObjectTool::Create(ImGuiIO* pIO)
{
	CImWindow_ObjectTool* pInstance = new CImWindow_ObjectTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_ObjectTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_ObjectTool::Free()
{
	__super::Free();
}
#endif