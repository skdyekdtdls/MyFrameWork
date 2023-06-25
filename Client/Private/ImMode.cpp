#ifdef _DEBUG
#include "ImMode.h"
#include "ImWindow_Manager.h"
#include "ImWindow_MapTool.h"
CImMode::CImMode(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}


void CImMode::Set_Mode(IMWIN_MODE eMode)
{
	m_eMode = eMode;
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
	ImGui::RadioButton("Object Tool", (int*)&m_eMode, OBJ_TOOL_MODE);
	ImGui::RadioButton("Map Tool", (int*)&m_eMode, MAP_TOOL_MODE);
	ImGui::RadioButton("Camera Tool", (int*)&m_eMode, CAMERA_TOOL_MODE);
	ImGui::RadioButton("Animation Tool", (int*)&m_eMode, ANIM_TOOL_MODE);
	ImGui::RadioButton("Effect Tool", (int*)&m_eMode, EFFECT_TOOL_MODE);
	ImGui::RadioButton("UI Tool", (int*)&m_eMode, UI_TOOL_MODE);
	ImGui::RadioButton("Lighth Tool", (int*)&m_eMode, LIGHT_TOOL_MODE);
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