#include "ClintShoot.h"
#include "GameInstance.h"
#include "Clint.h"

ClintShoot::ClintShoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintShoot::ClintShoot(const ClintShoot& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintShoot::OnStateEnter()
{
	__super::OnStateEnter();
	SetAnimIndex(CLINT_ANIM_SHOOT, UPPER);
	SetAnimIndex(CLINT_ANIM_SHOOT, LOWER);
}

void ClintShoot::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		if (pGameInstance->Get_DIKeyState(DIK_W))
		{
			pTransform->Go_Straight(TimeDelta, Facade->GetClintNavigation());
			SetAnimIndex(CLINT_ANIM_RUN, LOWER);
		}
		else
			SetAnimIndex(CLINT_ANIM_SHOOT, LOWER);
	}
	else
		m_pStateContext->TransitionTo(L"ClintIdle");

	Safe_Release(pGameInstance);
}

void ClintShoot::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintShoot::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintShoot(*this);
}

ClintShoot* ClintShoot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintShoot(pDevice, pContext);
}

void ClintShoot::Free()
{
	__super::Free();
}