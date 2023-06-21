#ifdef _USE_IMGUI
#include "ImWindow_Navigation.h"
#include "ImWindow_Manager.h"
#include "ImWindow_Demo.h"
CImWindow_Navigation::CImWindow_Navigation(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_Navigation::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_Navigation::Tick()
{
	if (NAVIGATION_MESH_MODE != CImWindow_Manager::GetInstance()->CurretMode())
		return;

	CImWindow_Manager* pImManagerInstance = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImManagerInstance);


	CImWindow_Demo* pDemo = static_cast<CImWindow_Demo*>(pImManagerInstance->Get_ImWindow(L"CImWindow_Demo"));

	ImGui::Begin("Navigation Mesh");

	ImGui::RadioButton("CREATE_MODE", (int*)&m_eMode, CREATE_MODE); ImGui::SameLine();
	ImGui::RadioButton("SELECT_POINT_MODE", (int*)&m_eMode, SELECT_POINT_MODE); ImGui::SameLine();
	ImGui::RadioButton("SELECT_CELL_MODE", (int*)&m_eMode, SELECT_CELL_MODE);

	// 선택된 아이템을 기억할 변수

	
	//ImGui::ListBox("Navigation Index", &item_current, VectorGetter, static_cast<void*>(&items),items.size(), 4);
		ImGui::ListBox("listbox\n(single select)",
			&item_current,
			VectorGetter,
			static_cast<void*>(&items),
			items.size(),
			8);

	ImGui::End();

	Safe_Release(pImManagerInstance);
}

bool CImWindow_Navigation::VectorGetter(void* data, int idx, const char** out_str)
{
	auto& vector = *static_cast<std::vector<std::string>*>(data);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_str = vector.at(idx).c_str();
	return true;
}

CImWindow_Navigation* CImWindow_Navigation::Create(ImGuiIO* pIO)
{
	CImWindow_Navigation* pInstance = new CImWindow_Navigation(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_Navigation::Free()
{
	__super::Free();
}
#endif
