#include "..\Public\ClintDash.h"
#include "GameInstance.h"
#include "Clint.h"

ClintDash::ClintDash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintDash::ClintDash(const ClintDash& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintDash::OnStateEnter()
{
	__super::OnStateEnter();

	m_pOwner->GetComponent<CModel>()->Set_AnimByIndex(CLINT_DASH, UPPER);
	m_pOwner->GetComponent<CModel>()->Set_AnimByIndex(CLINT_DASH, LOWER);
}

void ClintDash::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));



	if (pModel->IsAnimationFinished(LOWER))
	{
		m_pStateContext->TransitionTo(L"ClintIdle");
	}

	pModel->RootMotion(TimeDelta, pTransform->GetCurDirection());

	Safe_Release(pGameInstance);
}

void ClintDash::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintDash::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintDash(*this);
}

ClintDash* ClintDash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintDash(pDevice, pContext);
}

void ClintDash::Free()
{
	__super::Free();
}