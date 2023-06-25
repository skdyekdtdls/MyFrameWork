#ifdef _DEBUG
#include "ImWindow_MapTool.h"
#include "GameInstance.h"
#include "ImWindow_Manager.h"
#include "ImMode.h"
#include "ImWindow_Demo.h"
#include "Terrain.h"
#include "ImWindow.h"
#include "Fiona.h"

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
		// 터레인의 셀들을 가져와서 푸쉬백을 해준다.
		for (size_t i = 0; i < m_pImMgr->GetCellNum(); ++i)
			Cell_Index_items.push_back(to_string(i));
		m_bStart = false;
	}

	CImWindow_Manager* pImManagerInstance = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImManagerInstance);

	ImGui::Begin("Navigation Mesh");

	if (ImGui::RadioButton("CREATE_MODE", (int*)&m_eNaviMode, CREATE_MODE))
	{
		ResetClickCout();
		ZeroMemory(m_vClickPoint, sizeof(_float3) * CCell::POINT_END);
	}
	ImGui::SameLine();
	ImGui::RadioButton("SELECT_POINT_MODE", (int*)&m_eNaviMode, SELECT_POINT_MODE); ImGui::SameLine();
	ImGui::RadioButton("SELECT_CELL_MODE", (int*)&m_eNaviMode, SELECT_CELL_MODE);
	// 선택된 아이템을 기억할 변수
	
	//ImGui::ListBox("Navigation Index", &Cell_Index_item_current, VectorGetter, static_cast<void*>(&Cell_Index_items),Cell_Index_items.size(), 4);
	
	ImGui::ListBox("Cell_Index\n(single select)",
		&Cell_Index_item_current,
		VectorGetter,
		static_cast<void*>(&Cell_Index_items),
		Cell_Index_items.size(),
		20);

	ImGui::End();

	Safe_Release(pImManagerInstance);
}

void CImWindow_MapTool::LateTick()
{
	CreateTriangleStrip();
}

void CImWindow_MapTool::AddItems(const char* strItem)
{
	Cell_Index_items.push_back(strItem);
}

void CImWindow_MapTool::Object_Place()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Fiona::CLONE_DESC tCloneDesc;
	//CCube::CLONE_DESC tCloneDesc;
	//tCloneDesc.vPosition = { *(_float4*)&tPickDesc.vPickPos };
	//tCloneDesc.vPosition.w = 1.f;
	pGameInstance->Add_GameObject(LEVEL_IMGUI, Fiona::ProtoTag(), L"Layer_BackGround", &tCloneDesc);

	Safe_Release(pGameInstance);
}

void CImWindow_MapTool::Edit_Navigation_Mesh()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImMgr);
	Safe_AddRef(pGameInstance);

	switch (m_eNaviMode)
	{
	case CREATE_MODE:
		CreateTriangleStrip();
		break;
	case SELECT_POINT_MODE:
		break;
	case SELECT_CELL_MODE:
		break;
	}

	Safe_Release(pImMgr);
	Safe_Release(pGameInstance);
}

void CImWindow_MapTool::CreateTriangleStrip()
{
	if (m_pImMgr->IsPicking())
	{
		PICK_DESC tPickDesc = m_pImMgr->GetTerrainPickDesc();
		if (nullptr == tPickDesc.pPickedObject)
			return;

		_float3 vPickPos = *(_float3*)&tPickDesc.vPickPos;
		if (m_iClickCount == 2)
		{
			m_vClickPoint[m_iClickCount] = vPickPos;
			m_iClickCount = 0;
			m_pImMgr->AddCell(m_vClickPoint);
		}
		else
		{
			m_vClickPoint[m_iClickCount++] = vPickPos;
		}
	}
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
#endif