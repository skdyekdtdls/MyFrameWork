#ifdef _USE_IMGUI
#include "ImWindow_ObjectTool.h"
#include "Layer.h"
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
	// Static_Meshes
	ImGui::Begin("ObjectTool");
	ImGui::Columns(2, "mycolumns");
	ImGui::PushItemWidth(200);
	
	ImGui::ListBox("Static_Mesh\n(single select)",
		&Static_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Static_Mesh_items),
		Static_Mesh_items.size(),
		10); 

	ImGui::PopItemWidth();
	
	// Skeletal_Meshes
	ImGui::PushItemWidth(200);

	ImGui::ListBox("Skeletal_Mesh\n(single select)",
		&Skeletal_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Skeletal_Mesh_items),
		Skeletal_Mesh_items.size(),
		10); 
	
	ImGui::PopItemWidth(); 

	ImGui::PushItemWidth(70);

	if (ImGui::Button(" Load "))
	{
		
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete"))
	{

	}
	if (ImGui::Button("Search"))
	{

	}
	ImGui::SameLine();
	
	ImGui::InputText("Searchlabel", buf, IM_ARRAYSIZE(buf));
	ImGui::PopItemWidth();

	// Hierachy
	ImGui::NextColumn();
	ImGui::PushItemWidth(200);

	ImGui::ListBox("Hierachy\n(single select)",
		&Hierachy_item_current,
		VectorGetter,
		static_cast<void*>(&Hierachy_items),
		Hierachy_items.size(),
		21);

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