#ifdef _DEBUG
#include "ImWindow_MapTool.h"
#include "GameInstance.h"
#include "ImWindow_Manager.h"
#include "ImMode.h"
#include "ImWindow_Demo.h"
#include "Terrain.h"
#include "ImWindow.h"
#include "Fiona.h"
#include "EditCamera.h"

CImWindow_MapTool::CImWindow_MapTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_MapTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	m_pImMgr = CImWindow_Manager::GetInstance();

	return S_OK;
}

void CImWindow_MapTool::Tick()
{
	if (m_bStart)
	{
		// �ͷ����� ������ �����ͼ� Ǫ������ ���ش�.
		for (size_t i = 0; i < m_pImMgr->GetCellNum(); ++i)
			Cell_Index_items.push_back(to_string(i));
		m_pCurTerrain = Facade->GetTerrain();
		m_bStart = false;
	}

	CImWindow_Manager* pImManagerInstance = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImManagerInstance);

	CNavigation* pNavigation = m_pCurTerrain->GetComponent<CNavigation>();
	 
	ImGui::Begin("Navigation Mesh");

	if (ImGui::RadioButton("CREATE_MODE", (int*)&m_eNaviMode, CREATE_MODE))
	{
		ResetClickCout();
		ZeroMemory(m_vClickPoint, sizeof(_float3) * CCell::POINT_END);
	}
	ImGui::SameLine();
	ImGui::RadioButton("VERTEX_EDIT_MODE", (int*)&m_eNaviMode, VERTEX_EDIT_MODE); ImGui::SameLine();

	// ���� ����� ���
	if (ImGui::Button("DeleteCell"))
	{
		if(Cell_Index_item_current >= 0)
			pNavigation->DeleteCellByIndex(Cell_Index_item_current);
		Cell_Index_items.clear();
		for (size_t i = 0; i < m_pImMgr->GetCellNum(); ++i)
			Cell_Index_items.push_back(to_string(i));
	}
	
	ImGui::ListBox("Cell_Index\n(single select)",
		&Cell_Index_item_current,
		VectorGetter,
		static_cast<void*>(&Cell_Index_items),
		Cell_Index_items.size(),
		20);
	
	VecInfo("Position", &vPos, 120);
	
	// display
	if (ImGui::Button("Open File Dialog"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", " .dat", "../../Client/Bin/Data/Navigation/");
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// ���� ���� ����5������ �������� ������ OK�� ������� get selection
			// ��ȯ���� map<string, string> (file path, file name) ���·� ��ȯ
			ImGuiFileDialog::Instance()->GetSelection();

			
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			SaveNaviMesh(filePathName);
			// action
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	// �׺�޽� �������� ���� üũ�ڽ�
	ImGui::Checkbox("NaviRender", pNavigation->isRenderPtr());

	// �� �׸���
	ImGui::Checkbox("DrawMap", &m_pCurTerrain->m_bShowBrush);
	if (m_pCurTerrain->m_bShowBrush)
	{
		ImGui::RadioButton("Default", &DrawOption, 0);
		ImGui::RadioButton("Sand", &DrawOption, 1);
		ImGui::RadioButton("Rock", &DrawOption, 2);
	}
	
	// ���̺� ���� ���̾�α�
	if (ImGui::Button("Save MapTexture"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("SaveMapTextureFileDlgKey", "Save", " .mtex", "../../Resources/Skeletal_Mesh/");
	}

	if (ImGuiFileDialog::Instance()->Display("SaveMapTextureFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// ���� ���� ����5������ �������� ������ OK�� ������� get selection
			// ��ȯ���� map<string, string> (file path, file name) ���·� ��ȯ
			ImGuiFileDialog::Instance()->GetSelection();

			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			SaveMapTextureData();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	// �ε� ���� ���̾�α�
	if (ImGui::Button("Load MapTexture"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("LoadMapTextureFileDlgKey", "Load", " .mtex", "../../Resources/Skeletal_Mesh/");
	}

	if (ImGuiFileDialog::Instance()->Display("LoadMapTextureFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// ���� ���� ����5������ �������� ������ OK�� ������� get selection
			// ��ȯ���� map<string, string> (file path, file name) ���·� ��ȯ
			ImGuiFileDialog::Instance()->GetSelection();

			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			LoadMapTextureData();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	
	ImGui::End();

	switch (m_eNaviMode)
	{
	case CREATE_MODE:
		CreateTriangleStrip();
		break;
	case VERTEX_EDIT_MODE:
		Vertex_Edit();
		break;
	}

	Safe_Release(pImManagerInstance);
}

void CImWindow_MapTool::LateTick()
{
}

void CImWindow_MapTool::AddItems(const char* strItem)
{
	Cell_Index_items.push_back(strItem);
}

void CImWindow_MapTool::SaveNaviMesh(string filePathName)
{
	HANDLE hFile = CreateFile(TO_WSTR(filePathName).c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		CONSOLE_MSG("���� ���� ����");
		CloseHandle(hFile);
		return;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CEditCamera* pEditCamera = static_cast<CEditCamera*>(pGameInstance->Get_GameObject(LEVEL_IMGUI, L"Layer_Camera", "EditCamera"));
	CNavigation* pNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(LEVEL_IMGUI, L"Layer_BackGround", "CTerrain1", L"Com_Navigation"));
	
	DWORD dwByte = { 0 };
	pNavigation->Save(hFile, dwByte);

	Safe_Release(pGameInstance);
	CONSOLE_MSG("���� ���� ����");
	CloseHandle(hFile);
}

void CImWindow_MapTool::Vertex_Edit()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pGameInstance);
	Safe_AddRef(pImMgr);

	for (auto& CellPickDesc : tCellPickDesces)
	{
		CellPickDesc.pPickedCell->Set_Point(static_cast<CCell::POINT>(CellPickDesc.iVertexIndex), &vPos);
	}

	if (!pImMgr->IsPicking())
	{
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}
	tCellPickDesces.clear();
	CEditCamera* pEditCamera = static_cast<CEditCamera*>(pGameInstance->Get_GameObject(LEVEL_IMGUI, L"Layer_Camera", "EditCamera"));
	CNavigation* pNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(LEVEL_IMGUI, L"Layer_BackGround", "CTerrain1", L"Com_Navigation"));
	RAY tMouseRay = pEditCamera->GetMouseRay();
	
	pNavigation->IsCellVertexPicked(tCellPickDesces, tMouseRay);
	for (auto& CellPickDesc : tCellPickDesces)
	{
		CONSOLE_MSG("CellPtr : " << CellPickDesc.pPickedCell << "\t" << "VertexIndex : " << CellPickDesc.iVertexIndex);
	}
	
	if (!tCellPickDesces.empty())
	{
		vPos = *(_float3*)&tCellPickDesces[0].vPickPos;
	}

	Safe_Release(pImMgr);
	Safe_Release(pGameInstance);
}

void CImWindow_MapTool::CreateTriangleStrip()
{
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImMgr);

	if (m_pImMgr->IsPicking())
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		PICK_DESC tPickDesc = m_pImMgr->GetTerrainPickDesc();
		CEditCamera* pEditCamera = static_cast<CEditCamera*>(pGameInstance->Get_GameObject(LEVEL_IMGUI, L"Layer_Camera", "EditCamera"));
		CNavigation* pNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(LEVEL_IMGUI, L"Layer_BackGround", "CTerrain1", L"Com_Navigation"));
		RAY tMouseRay = pEditCamera->GetMouseRay();
		_float3 vPickPos;
		tCellPickDesces.clear();
		pNavigation->IsCellVertexPicked(tCellPickDesces, tMouseRay);

		if (nullptr != tPickDesc.pPickedObject)
		{
			vPickPos = *(_float3*)&tPickDesc.vPickPos;
		}

		if (!tCellPickDesces.empty())
		{
			vPickPos = *(_float3*)&tCellPickDesces[0].vPickPos;
		}

		if (nullptr == tPickDesc.pPickedObject && tCellPickDesces.empty())
		{
			Safe_Release(pGameInstance);
			Safe_Release(pImMgr);
			return;
		}

		if (m_iClickCount == 2)
		{
			m_vClickPoint[m_iClickCount] = vPickPos;
			m_iClickCount = 0;
			pNavigation->AddCell(m_vClickPoint);
			Cell_Index_items.clear();
			for(size_t i = 0 ; i < pNavigation->GetCellNum(); ++i)
				Cell_Index_items.push_back(to_string(i));
		}
		else
		{
			m_vClickPoint[m_iClickCount++] = vPickPos;
		}

		Safe_Release(pGameInstance);
	}

	Safe_Release(pImMgr);
}

void CImWindow_MapTool::SaveMapTextureData()
{
	HANDLE hFile = CreateFile(TO_WSTR(filePathName).c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		CONSOLE_MSG("File save failed");
		return;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	DWORD dwByte = { 0 };

	Safe_Release(pGameInstance);
	CONSOLE_MSG("File save completed successfully.");
	CloseHandle(hFile);
}

void CImWindow_MapTool::LoadMapTextureData()
{
	HANDLE hFile = CreateFile(TO_WSTR(filePathName).c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		CONSOLE_MSG("File load failed");
		return;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	DWORD dwByte = { 0 };
	
	Safe_Release(pGameInstance);
	CONSOLE_MSG("File load completed successfully.");
	CloseHandle(hFile);
}

CImWindow_MapTool* CImWindow_MapTool::Create(ImGuiIO* pIO)
{
	CImWindow_MapTool* pInstance = new CImWindow_MapTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_MapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_MapTool::Free()
{
	__super::Free();
}

void CImWindow_MapTool::VecInfo(const char* text, _float3* vec3, int iSize)
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
	ImGui::DragFloat(label.c_str(), &vec3->z); ImGui::Spacing();  // ���� �߰�
	ImGui::PopItemWidth();
}

#endif