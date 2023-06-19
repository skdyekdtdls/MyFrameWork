#ifdef _USE_IMGUI
#include "ImMode.h"

CImMode::CImMode(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}


HRESULT CImMode::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImMode::Tick()
{
	ImGui::Begin("Edit_Mode");
	ImGui::RadioButton("Object Place", (int*)&m_eMode, OBJECT_PLACE_MODE); 
	ImGui::RadioButton("Navigation Mesh", (int*)&m_eMode, NAVIGATION_MESH_MODE);
	ImGui::RadioButton("Transform", (int*)&m_eMode, TRANSFORM_MODE);
	ImGui::End();
	
}
CImMode* CImMode::Create(ImGuiIO* pIO)
{
	CImMode* pInstance = new CImMode(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImMode");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImMode::Free()
{
	__super::Free();
}
#endif