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

void CImWindow_ObjectTool::Tick()
{
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImMgr);
	Set_GameObject(pImMgr->GetMinDistPickDesc().pPickedObject);
	ImGui::Begin("Transform");

	//ImGui::ListBox("Navigation Index", &item_current, VectorGetter, static_cast<void*>(&items),items.size(), 4);
	if (ImGui::ListBox("Static_Mesh##hidden\n(single select)",
		&Static_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Static_Mesh_items),
		Static_Mesh_items.size(),
		15))
	{
		Skeletal_Mesh_item_current = -1;
	}

	//ImGui::ListBox("Navigation Index", &item_current, VectorGetter, static_cast<void*>(&items),items.size(), 4);
	if (ImGui::ListBox("Skeletal_Mesh##hidden\n(single select)",
		&Skeletal_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Skeletal_Mesh_items),
		Skeletal_Mesh_items.size(),
		15))
	{
		Static_Mesh_item_current = -1;
	}

	ImGui::Checkbox("Object_Place", &m_bCheck);
	if (m_bCheck)
	{
		ObjectPlace();
	}

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
void CImWindow_ObjectTool::ObjectPlace()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pGameInstance);
	Safe_AddRef(pImMgr);

	wstring tag;
	PICK_DESC tTerrainPickDesc = pImMgr->GetTerrainPickDesc();
	if (Static_Mesh_item_current != -1)
		tag = L"Prototype_GameObject_" + TO_WSTR(Static_Mesh_items[Static_Mesh_item_current]);
	else if(Skeletal_Mesh_item_current != -1)
		tag = L"Prototype_GameObject_" + TO_WSTR(Skeletal_Mesh_items[Skeletal_Mesh_item_current]);
	
	ImGui::Begin("Select Layer");
	int iCount = 0;
	
	for (_uint i = 0; i < pGameInstance->GetNumLayers(LEVEL_IMGUI); ++i)
	{
		iCount = 0;
		for (auto& LayerPair : pGameInstance->GetLayers()[i])
		{
			char szTmp[MAX_PATH];
			TO_CHAR(LayerPair.first, szTmp);
			if (ImGui::RadioButton(szTmp, &m_bRadioButton, iCount)) {
				strcpy_s(m_szCurItemLabel, szTmp);
			}

			iCount++;
		}
	}

	ImGui::End();

	if (!pImMgr->IsPicking())
	{
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	if (0 == strcmp("", m_szCurItemLabel))
	{
		CONSOLE_MSG("Select any Layer");
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	PICK_DESC pTerrainDesc = pImMgr->GetTerrainPickDesc();
	if (tTerrainPickDesc == PICK_DESC())
	{
		CONSOLE_MSG("Terrain is not picked");
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	if (-1 == Skeletal_Mesh_item_current && -1 == Static_Mesh_item_current)
	{
		CONSOLE_MSG("Select the static mesh or the skeletal mesh");
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	_tchar szTmp[MAX_PATH];
	TO_WCHAR(m_szCurItemLabel, szTmp);

	CGameObject::CLONE_DESC tCloneDesc;
	tCloneDesc.vPosition = _float4(pTerrainDesc.vPickPos);
	tCloneDesc.vPosition.w = 1.f;
	CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_IMGUI, tag.c_str(), szTmp, &tCloneDesc);
	CONSOLE_MSG("The Object Tool call \'Add_GameObject\'");
	if (nullptr == pGameObject)
	{
		WCONSOLE_MSG("Tag         : " << tag.c_str());
		CONSOLE_MSG("Can't Find GameObject From Prototypes");
	}
	else
	{
		WCONSOLE_MSG("Tag         : " << tag.c_str());
		WCONSOLE_MSG("Layer       : " << szTmp);
		CONSOLE_MSG("Position    : " << pTerrainDesc.vPickPos.x << " " << pTerrainDesc.vPickPos.y << " " << pTerrainDesc.vPickPos.z);
	}

	Safe_Release(pImMgr);
	Safe_Release(pGameInstance);
}
void CImWindow_ObjectTool::Free()
{
	__super::Free();

	Safe_Release(m_pGameObject);
}


#endif
