#ifdef _USE_IMGUI
#include "ImWindow_Transform.h"
#include "ImWindow_Manager.h"
#include "ImMode.h"
#include "GameInstance.h"
#include "GameObject.h"
CImWindow_Transform::CImWindow_Transform(ImGuiIO* pIO)
	: CImWindow(pIO)
{

}

HRESULT CImWindow_Transform::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;
	
	return S_OK;
}

void CImWindow_Transform::Tick()
{
	if (TRANSFORM_MODE != CImWindow_Manager::GetInstance()->CurretMode() && NAVIGATION_MESH_MODE != CImWindow_Manager::GetInstance()->CurretMode())
		return;

	ImGui::Begin("Transform");
	if (NAVIGATION_MESH_MODE == CImWindow_Manager::GetInstance()->CurretMode())
	{
		strName = "NAVIGATION_MESH";
	}
	ImGui::Text("name : "); ImGui::SameLine();
	ImGui::Text(strName.c_str());
	ImGui::Spacing();
	VecInfo("Position", &vPos, 120);
	VecInfo("Rotation", &vRot, 120);
	VecInfo(" Scale", &vScale, 120);

	if (m_pGameObject)
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
}

void CImWindow_Transform::Set_GameObject(CGameObject* pGameObject)
{
	Safe_Release(m_pGameObject);

	m_pGameObject = pGameObject;

	Safe_AddRef(m_pGameObject);
	CTransform* pTransform = dynamic_cast<CTransform*>(m_pGameObject->Get_Component(L"Com_Transform"));

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

CImWindow_Transform* CImWindow_Transform::Create(ImGuiIO* pIO)
{
	CImWindow_Transform* pInstance = new CImWindow_Transform(pIO);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created CImWindow_Transform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImWindow_Transform::VecInfo(const char* text, _float3* vec3, int iSize)
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
void CImWindow_Transform::Free()
{
	__super::Free();

	Safe_Release(m_pGameObject);
}


#endif
