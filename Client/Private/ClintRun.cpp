#include "..\Public\ClintRun.h"
#include "GameInstance.h"
#include "Clint.h"

ClintRun::ClintRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintRun::ClintRun(const ClintRun& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
{
}

void ClintRun::OnStateEnter()
{
	__super::OnStateEnter();
	SetAnimIndex(CLINT_RUN, UPPER);
	SetAnimIndex(CLINT_RUN, LOWER);
}

void ClintRun::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	
	_byte W = pGameInstance->Get_DIKeyState(DIK_W);
	_byte A = pGameInstance->Get_DIKeyState(DIK_A);
	_byte S = pGameInstance->Get_DIKeyState(DIK_S);
	_byte D = pGameInstance->Get_DIKeyState(DIK_D);
	_byte Q = pGameInstance->Get_DIKeyState(DIK_Q);
	_byte Space = pGameInstance->Get_DIKeyState(DIK_SPACE);

	SetAnimIndex(CLINT_RUN, UPPER);
	SetAnimIndex(CLINT_RUN, LOWER);

	if (W)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		if (Q)
		{
			m_pStateContext->TransitionTo(L"ClintUltimate01");
		}
	}
	else if (A)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		if (Q)
		{
			m_pStateContext->TransitionTo(L"ClintUltimate01");
		}
	}
	else if (S)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
		if (Q)
		{
			m_pStateContext->TransitionTo(L"ClintUltimate01");
		}
	}
	else if (D)
	{
		pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
		pTransform->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		if (Q)
		{
			m_pStateContext->TransitionTo(L"ClintUltimate01");
		}
	}
	else
	{
		m_pStateContext->TransitionTo(L"ClintIdle");
	}

	if (Space)
	{
		if (W)
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_N);
		}
		else if (A)
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_W);
		}
		else if (S)
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_S);
		}
		else if (D)
		{
			pModel->RootMotion(TimeDelta, CTransform::DIR_E);
		}

		m_pStateContext->TransitionTo(L"ClintDash");
	}
	else if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		m_pStateContext->TransitionTo(L"ClintShoot");
	}

	Safe_Release(pGameInstance);
}

void ClintRun::OnStateExit()
{
	__super::OnStateExit();
}

void ClintRun::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
}

StateMachine<Clint, CLINT_ANIM>* ClintRun::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintRun(*this);
}

ClintRun* ClintRun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintRun(pDevice, pContext);
}

void ClintRun::Free()
{
	__super::Free();
}