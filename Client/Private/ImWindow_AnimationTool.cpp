#ifdef _USE_IMGUI
#include "ImWindow_AnimationTool.h"
#include "GameInstance.h"
#include "ImWindow_Manager.h"
#include "GameObject.h"
#include "Animation.h"

CImWindow_AnimationTool::CImWindow_AnimationTool(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_AnimationTool::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;

	return S_OK;
}

void CImWindow_AnimationTool::Tick()
{
	CImWindow_Manager* pImMgr = CImWindow_Manager::GetInstance();
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pImMgr);
	Safe_AddRef(pGameInstance);

	ImGui::Begin("Animation Tool");
	if (ImGui::ListBox("Skeletal_Mesh##hidden\n(single select)",
		&Skeletal_Mesh_item_current,
		VectorGetter,
		static_cast<void*>(&Skeletal_Mesh_items),
		Skeletal_Mesh_items.size(),
		8))
	{
		string Proto = "Prototype_Component_Model_" + Skeletal_Mesh_items[Skeletal_Mesh_item_current];
		CComponent* pProtoCom = pGameInstance->Get_ProtoComponent(LEVEL_IMGUI, TO_WSTR(Proto).c_str());
		CModel* pModelProto = static_cast<CModel*>(pProtoCom);
		Animation_items.clear();
		for (size_t i = 0; i < pModelProto->Get_NumAnimation(); ++i)
		{
			string tmp = pModelProto->Get_Animation(i)->GetName();
			Animation_items.push_back(tmp);
		}

		string tmp = "Prototype_GameObject_" + Skeletal_Mesh_items[Skeletal_Mesh_item_current];

		ChangeDummyObject(pGameInstance->Clone_GameObject(TO_WSTR(tmp).c_str(), nullptr));
	}

	ImGui::Checkbox("Dummy", &m_isDummy);

	if (ImGui::ListBox("Animations ##hidden\n(single select)",
		&Animation_item_current,
		VectorGetter,
		static_cast<void*>(&Animation_items),
		Animation_items.size(),
		30))
	{
		// 더미오브젝트를 세우고 이 더미오브젝트의 애니메이션 인덱스를 바꾼다.
		if (m_isDummy)
		{
			// 더미오브젝트의 인덱스를 바꾼다.
			if (nullptr != m_pDummyObject)
			{
				CModel* pModel = static_cast<CModel*>(m_pDummyObject->Get_Component(L"Com_Model"));
				pModel->Set_AnimByName(Animation_items[Animation_item_current].c_str());

			}
		}
	}
	string CurIndex = "Current Anim Index : " + to_string(Animation_item_current);
	ImGui::Text(CurIndex.c_str());

	if (m_isDummy)
	{
		if (nullptr != m_pDummyObject)
		{
			m_pDummyObject->Tick(0.016);
		}
	}

	ImGui::End();

	Safe_Release(pGameInstance);
	Safe_Release(pImMgr);
		
}
void CImWindow_AnimationTool::LateTick()
{
	if (m_isDummy)
	{
		if (nullptr != m_pDummyObject)
		{
			if (nullptr != m_pDummyObject)
				m_pDummyObject->Late_Tick(0.016);
		}
	}
}

void CImWindow_AnimationTool::ChangeDummyObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return;
	Safe_Release(m_pDummyObject);
	m_pDummyObject = pGameObject;
	CTransform* pTransform = static_cast<CTransform*>(m_pDummyObject->Get_Component(L"Com_Transform"));
	pTransform->Scaled(_float3(20.f, 20.f, 20.f));
}

CImWindow_AnimationTool* CImWindow_AnimationTool::Create(ImGuiIO* pIO)
{
	CImWindow_AnimationTool* pInstance = new CImWindow_AnimationTool(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_AnimationTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_AnimationTool::Free()
{
	__super::Free();
	Safe_Release(m_pDummyObject);
}
#endif
