#ifdef _DEBUG
#include "ImWindow_ObjectTool.h"
#include "ImWindow_Manager.h"
#include "ImMode.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Terrain.h"
#include <filesystem>

namespace fs = std::filesystem;

CImWindow_ObjectTool::CImWindow_ObjectTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_ObjectTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;
	
	//fs::path Staitc_Mesh_Folder = "../../Resources/Models/Staitc_Mesh/";
	//fs::directory_iterator itr(Staitc_Mesh_Folder);
	//while (itr != fs::end(itr)) {
	//	const fs::directory_entry& entry = *itr;
	//	Static_Mesh_items.push_back(entry.path().filename().string());
	//	itr++;
	//}



	return S_OK;
}

void CImWindow_ObjectTool::Tick()
{
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImMgr);
	Set_GameObject(pImMgr->GetMinDistPickDesc().pPickedObject);
	ImGui::Begin("Transform");

	//ImGui::ListBox("Navigation Index", &item_current, VectorGetter, static_cast<void*>(&items),items.size(), 4);
	ImGui::ListBox("Staitc_Mesh\n(single select)",
		&Staitc_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Static_Mesh_items),
		Static_Mesh_items.size(),
		15);

	//ImGui::ListBox("Navigation Index", &item_current, VectorGetter, static_cast<void*>(&items),items.size(), 4);
	ImGui::ListBox("Skeletal_Mesh\n(single select)",
		&Skeletal_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Skeletal_Mesh_items),
		Skeletal_Mesh_items.size(),
		15);

	//if (ImGui::Button("Open File Dialog"))
	//{
	//	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");
	//}
	//ShowFileDialog();

	ImGui::Text("name : "); ImGui::SameLine();
	ImGui::Text(strName.c_str());
	ImGui::Spacing();

	VecInfo("Position", &vPos, 120);
	VecInfo("Rotation", &vRot, 120);
	VecInfo(" Scale", &vScale, 120);

	if (nullptr != m_pGameObject)
	{
		CTransform* pTransform = static_cast<CTransform*>(m_pGameObject->Get_Component(L"Com_Transform"));
		if (pTransform)
		{
			_matrix matWorld = XMMatrixIdentity();
			_matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
			_matrix matRot = XMMatrixRotationX(XMConvertToRadians(vRot.x)) * XMMatrixRotationY(XMConvertToRadians(vRot.y)) * XMMatrixRotationZ(XMConvertToRadians(vRot.z));
			_matrix matPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
			matWorld = matScale * matRot * matPos;
			pTransform->Set_WorldMatrix(matWorld);
		}
	}

	ImGui::End();
	Safe_Release(pImMgr);
}

void CImWindow_ObjectTool::Set_GameObject(CGameObject* pGameObject)
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

	strName = m_pGameObject->Get_Name();

	XMStoreFloat3(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));
	vRot = pTransform->Get_AulerDegree();
	vScale = pTransform->Get_Scaled();
}

void CImWindow_ObjectTool::ShowFileDialog()
{
	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// 만약 위의 인자5번에서 여러가지 선택후 OK가 눌린경우 get selection
			// 반환값은 map<string, string> (file path, file name) 형태로 반환
			ImGuiFileDialog::Instance()->GetSelection();

			 filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			 filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
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

void CImWindow_ObjectTool::VecInfo(const char* text, _float3* vec3, int iSize)
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
void CImWindow_ObjectTool::Free()
{
	__super::Free();

	Safe_Release(m_pGameObject);
}


#endif
