#ifdef _USE_IMGUI
#include "ImWindow_AnimationTool.h"
#include "GameInstance.h"
#include "ImWindow_Manager.h"
#include "GameObject.h"
#include "Animation.h"
#include "Channel.h"
#include "Bone.h"
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
	ImGui::Columns(2, "mixed");
	// 객체를 클릭하면 리스트 박스에 애니메이션 목록을 넣어준다.
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
			string tmp = pModelProto->GetAnimationByIndex(i)->GetName();
			Animation_items.push_back(tmp);
		}

		string tmp = "Prototype_GameObject_" + Skeletal_Mesh_items[Skeletal_Mesh_item_current];

		ChangeDummyObject(pGameInstance->Clone_GameObject(TO_WSTR(tmp).c_str(), nullptr));
	}

	// 더미 체크박스
	ImGui::Checkbox("Dummy", &m_isDummy);

	// 애니메이션 리스트 박스
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
				
				// 클릭한 애니메이션 항목 대하여 채널 리스트박스 정보를 갱신한다.
				m_pAnimation = pModel->GetAnimationByName(Animation_items[Animation_item_current]);
				
				if (-1 != Animation_item_current)
				{
					Bone_items.clear();
					for (_uint i = 0; i < m_pAnimation->Get_NumChannels(); ++i)
					{
						CBone* pBone= pModel->GetBoneByIndex(i);
						Bone_items.push_back(pBone->GetName());
					}
				}
			}
		}
	}

	// 현재 클릭된 애니메이션 인덱스를 표시
	string CurIndex = "Current Anim Index : " + to_string(Animation_item_current);
	ImGui::Text(CurIndex.c_str());

	// 열 나누기
	ImGui::NextColumn();

	// 채널 리스트 박스
	if (ImGui::ListBox("Channels ##hidden\n(single select)",
		&Bone_item_current,
		VectorGetter,
		static_cast<void*>(&Bone_items),
		Bone_items.size(),
		25))
	{


	}

	// 현재 클릭된 애니메이션 인덱스를 표시
	string CurChannelIndex = "Current Bone Index : " + to_string(Bone_item_current);
	ImGui::Text(CurChannelIndex.c_str());

	// 틱퍼세컨드랑 loop 결정, Duration표시
	if (nullptr != m_pAnimation)
	{
		_float fTickPerSecond = *m_pAnimation->GetTickPerSecondPtr();
		_float fDuration = m_pAnimation->Get_Duration(); 
		ImGui::Text("Duration : "); ImGui::SameLine(); ImGui::Text(to_string(fDuration).c_str());
		ImGui::DragFloat("TickPerSecond", &fTickPerSecond);
		*m_pAnimation->GetTickPerSecondPtr() = fTickPerSecond;

		ImGui::Checkbox("IsLoop", m_pAnimation->GetIsLoopPtr());
	}

	// 세이브 파일 다이얼로그
	if (ImGui::Button("Save Animation"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Save", " .myanim", "../../Resources/Skeletal_Mesh/");
	}

	if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// 만약 위의 인자5번에서 여러가지 선택후 OK가 눌린경우 get selection
			// 반환값은 map<string, string> (file path, file name) 형태로 반환
			ImGuiFileDialog::Instance()->GetSelection();

			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			SaveAnimationData();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	// 로드 파일 다이얼로그
	if (ImGui::Button("Load Animation"))
	{
		ImGuiFileDialog::Instance()->OpenDialog("LoadFileDlgKey", "Load", " .myanim", "../../Resources/Skeletal_Mesh/");
	}

	if (ImGuiFileDialog::Instance()->Display("LoadFileDlgKey"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			// 만약 위의 인자5번에서 여러가지 선택후 OK가 눌린경우 get selection
			// 반환값은 map<string, string> (file path, file name) 형태로 반환
			ImGuiFileDialog::Instance()->GetSelection();

			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			LoadAnimationData();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::Columns(1);

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

void CImWindow_AnimationTool::SaveAnimationData()
{
	if (nullptr == m_pDummyObject)
	{
		CONSOLE_MSG("Failed Save");
		CONSOLE_MSG("Select an Object in list Box");
		return;
	}

	HANDLE hFile = CreateFile(TO_WSTR(filePathName).c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		CONSOLE_MSG("File save failed");
		return;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	DWORD dwByte = { 0 };
	CModel* pModel = m_pDummyObject->GetComponent<CModel>();
	for (size_t i = 0; i < pModel->Get_NumAnimation(); ++i)
	{
		CAnimation* pAnimation = pModel->GetAnimationByIndex(i);
		pAnimation->SaveData(hFile, dwByte);
	}

	Safe_Release(pGameInstance);
	CONSOLE_MSG("File save completed successfully.");
	CloseHandle(hFile);
}

void CImWindow_AnimationTool::LoadAnimationData()
{
	if (nullptr == m_pDummyObject)
	{
		CONSOLE_MSG("Failed Load");
		CONSOLE_MSG("Select an Object in list Box");
		return;
	}

	HANDLE hFile = CreateFile(TO_WSTR(filePathName).c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		CONSOLE_MSG("File load failed");
		return;
	}

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	DWORD dwByte = { 0 };
	CModel* pModel = m_pDummyObject->GetComponent<CModel>();
	for (size_t i = 0; i < pModel->Get_NumAnimation(); ++i)
	{
		CAnimation* pAnimation = pModel->GetAnimationByIndex(i);
		pAnimation->LoadData(hFile, dwByte);
	}

	Safe_Release(pGameInstance);
	CONSOLE_MSG("File load completed successfully.");
	CloseHandle(hFile);
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
