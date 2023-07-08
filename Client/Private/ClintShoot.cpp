#include "ClintShoot.h"
#include "GameInstance.h"
#include "Clint.h"

ClintShoot::ClintShoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
	: ClintState(pDevice, pContext, pClint)
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

	}
	else
		m_pOwner->TransitionTo(L"ClintIdle");

	Safe_Release(pGameInstance);
}

void ClintShoot::OnStateExit()
{
	__super::OnStateExit();
}

ClintShoot* ClintShoot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint)
{
	return new ClintShoot(pDevice, pContext, pClint);
}

void ClintShoot::Free()
{
	__super::Free();
}