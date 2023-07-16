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

	if (pGameInstance->Get_DIKeyState(DIK_W) || pGameInstance->Get_DIKeyState(DIK_A) ||
		pGameInstance->Get_DIKeyState(DIK_S) || pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pStateContext->TransitionTo(L"ClintRun");
	}
	else if (pGameInstance->Get_DIKeyState(DIK_Q))
	{
		m_pStateContext->TransitionTo(L"ClintUltimate01");
	}
	else if (pGameInstance->Get_DIKeyState(DIK_SPACE))
	{
		m_pStateContext->TransitionTo(L"ClintDash");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		m_pStateContext->TransitionTo(L"ClintShoot");
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
