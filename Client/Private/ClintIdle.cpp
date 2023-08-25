#include "..\Public\ClintIdle.h"
#include "GameInstance.h"
#include "Clint.h"

ClintIdle::ClintIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintIdle::ClintIdle(const ClintIdle& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintIdle::OnStateEnter()
{
	__super::OnStateEnter();
	SetAnimIndex(CLINT_IDLE, UPPER);
	SetAnimIndex(CLINT_IDLE, LOWER);
}

void ClintIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	_byte W = pGameInstance->Get_DIKeyState(DIK_S);
	_byte A = pGameInstance->Get_DIKeyState(DIK_Z);
	_byte S = pGameInstance->Get_DIKeyState(DIK_X);
	_byte D = pGameInstance->Get_DIKeyState(DIK_C);
	_byte Space = pGameInstance->Get_DIKeyState(DIK_SPACE);

	if (pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_Z) ||
		pGameInstance->Get_DIKeyState(DIK_X) || pGameInstance->Get_DIKeyState(DIK_C))
	{
		TransitionTo(L"ClintRun");
	}
	else if (pGameInstance->Get_DIKeyState(DIK_Q))
	{
		TransitionTo(L"ClintUltimate01");
	}
	else if (Space)
	{
		TransitionTo(L"ClintDash");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		TransitionTo(L"ClintShoot");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_RBUTTON))
	{
		TransitionTo(L"ClintSkill01");
	}
	Safe_Release(pGameInstance);
}

void ClintIdle::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintIdle(*this);
}

ClintIdle* ClintIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintIdle(pDevice, pContext);
}

void ClintIdle::Free()
{
	__super::Free();
}
