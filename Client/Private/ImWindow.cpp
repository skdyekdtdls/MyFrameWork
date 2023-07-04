#ifdef _DEBUG
#include "..\Public\ImWindow.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "GameInstance.h"

CImWindow::CImWindow(ImGuiIO* pIO)
    : m_pIO(pIO)
    , m_IsShow(true)
{

}

HRESULT CImWindow::Initialize(void* pArg)
{
	fs::path Static_Mesh_Folder = "../../Resources/Static_Mesh/";
	fs::directory_iterator iter(Static_Mesh_Folder);
	while (iter != fs::end(iter)) {
		const fs::directory_entry& entry = *iter;
		Static_Mesh_items.push_back(entry.path().filename().string());
		iter++;
	}

	fs::path Skeletal_Mesh_Folder = "../../Resources/Skeletal_Mesh/";
	fs::directory_iterator iter2(Skeletal_Mesh_Folder);
	while (iter2 != fs::end(iter2)) {
		const fs::directory_entry& entry = *iter2;
		Skeletal_Mesh_items.push_back(entry.path().filename().string());
		iter2++;
	}

    return S_OK;
}

void CImWindow::LateTick()
{
}

void CImWindow::VecInfo(const char* text, _float3* vec3, int iSize)
{
	string label;
	label = "##";
	label.append(text);

	ImVec2 textSize = ImGui::CalcTextSize(text);
	ImGui::Text(text); ImGui::SameLine();
	ImGui::Dummy(ImVec2(100 - textSize.x, 0.0f));  ImGui::SameLine();
	ImGui::Text("x"); ImGui::SameLine();
	ImGui::PushItemWidth(iSize);
	label.append("0");
	ImGui::DragFloat(label.c_str(), &vec3->x); ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::Text("y"); ImGui::SameLine();
	ImGui::PushItemWidth(iSize);
	label.append("0");
	ImGui::DragFloat(label.c_str(), &vec3->y); ImGui::SameLine();
	ImGui::PopItemWidth();

	ImGui::Text("z"); ImGui::SameLine();
	ImGui::PushItemWidth(iSize);
	label.append("0");
	ImGui::DragFloat(label.c_str(), &vec3->z); ImGui::Spacing();  // 공백 추가
	ImGui::PopItemWidth();
}

void CImWindow::Set_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;

	if (m_pGameObject == pGameObject)
		return;

	if (!pGameObject->HasTransformCom())
		return;

	Safe_Release(m_pGameObject);

	m_pGameObject = pGameObject;

	Safe_AddRef(m_pGameObject);

	CTransform* pTransform = static_cast<CTransform*>(m_pGameObject->Get_Component(L"Com_Transform"));

	if (nullptr == pTransform)
	{
		Safe_Release(m_pGameObject);
		return;
	}
}

void CImWindow::Free(void)
{
	Safe_Release(m_pGameObject);
}
#endif