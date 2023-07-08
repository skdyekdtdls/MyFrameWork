#include "..\Public\ClintDash.h"
#include "GameInstance.h"
#include "Clint.h"
#include "ClintModel.h"

ClintDash::ClintDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
	: ClintState(pDevice, pContext, pClint)
{
}

void ClintDash::OnStateEnter()
{
	__super::OnStateEnter();

	m_pOwner->GetComponent<ClintModel>()->Set_AnimByIndex(CLINT_ANIM_DASH, UPPER);
	m_pOwner->GetComponent<ClintModel>()->Set_AnimByIndex(CLINT_ANIM_DASH, LOWER);
}

void ClintDash::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished(UPPER))
	{
		m_pOwner->TransitionTo(L"ClintIdle");
	}

	pModel->RootMotion(TimeDelta, pTransform->GetCurDirection());

	Safe_Release(pGameInstance);
}

void ClintDash::OnStateExit()
{
	__super::OnStateExit();
}

ClintDash* ClintDash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	return new ClintDash(pDevice, pContext, pClint);
}

void ClintDash::Free()
{
	__super::Free();
}