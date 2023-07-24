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

	return S_OK;
}

void CImWindow_ObjectTool::Tick()
{
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImMgr);
	Set_GameObject(pImMgr->GetMinDistPickDesc().pPickedObject);
	ImGui::Begin("Transform");

	// 스테틱 폴더 리스트 박스
	if (ImGui::ListBox("Static_Mesh##hidden\n(single select)",
		&Static_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Static_Mesh_items),
		Static_Mesh_items.size(),
		15))
	{
		Skeletal_Mesh_item_current = -1; // 스켈레탈 누르면 스테틱 선택해제
	}

	// 스켈레탈 폴더 리스트 박스.
	if (ImGui::ListBox("Skeletal_Mesh##hidden\n(single select)",
		&Skeletal_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Skeletal_Mesh_items),
		Skeletal_Mesh_items.size(),
		15))
	{
		Static_Mesh_item_current = -1; // 스테틱 누르면 스켈레탈 선택해제
	}

	// 객체 설치 기능

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Key_Down(DIK_GRAVE))
	{
		m_iPlaceOrDelete++;
		if (m_iPlaceOrDelete >= 3)
			m_iPlaceOrDelete = 0;
	}

	Safe_Release(pGameInstance);


	ImGui::RadioButton("Object_Place", &m_iPlaceOrDelete, 0);
	ImGui::RadioButton("Delete_Place", &m_iPlaceOrDelete, 1);
	ImGui::RadioButton("NONE", &m_iPlaceOrDelete, 2);
	if (0 == m_iPlaceOrDelete)
	{
		ObjectPlace(); 
	}
	else if (1 == m_iPlaceOrDelete)
	{
		DeletePlace();
	}

	// 객체 트랜스폼 편집
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

	CGameObject::CGAMEOBJECT_DESC tCloneDesc;
	tCloneDesc.vPosition = _float4(pTerrainDesc.vPickPos);
	tCloneDesc.vPosition.w = 1.f;
	CGameObject* pGameObject = pGameInstance->Add_GameObject(LEVEL_IMGUI, tag.c_str(), szTmp, &tCloneDesc);
	system("cls");
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

void CImWindow_ObjectTool::DeletePlace()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pGameInstance);
	Safe_AddRef(pImMgr);
	system("cls");
	PICK_DESC tPickDesc = pImMgr->GetMinDistPickDesc();
	std::string strName;
	
	if(nullptr != tPickDesc.pPickedObject)
		strName = tPickDesc.pPickedObject->Get_Name().c_str();

	if (nullptr == tPickDesc.pPickedObject)
	{
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	if (0 == strName.compare("Clint1"))
	{
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	if (0 == strName.compare("CTerrain1"))
	{
		Safe_Release(pImMgr);
		Safe_Release(pGameInstance);
		return;
	}

	tPickDesc.pPickedObject->SetDead();

	wstring tag;
	PICK_DESC tTerrainPickDesc = pImMgr->GetTerrainPickDesc();

	CONSOLE_MSG(tPickDesc.pPickedObject->Get_Name() << " Deleted");

	Safe_Release(pImMgr);
	Safe_Release(pGameInstance);
}

void CImWindow_ObjectTool::Free()
{
	__super::Free();
}


#endif
