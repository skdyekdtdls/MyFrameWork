#include "..\Public\ClintRun.h"
#include "GameInstance.h"
#include "Clint.h"
#include "SoundMgr.h"
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
	
	SoundMgr->PlaySound(L"step_bare2.ogg", CHANNELID::PLAYER, 0.5f);

	_byte W = pGameInstance->Get_DIKeyState(DIK_S);
	_byte A = pGameInstance->Get_DIKeyState(DIK_Z);
	_byte S = pGameInstance->Get_DIKeyState(DIK_X);
	_byte D = pGameInstance->Get_DIKeyState(DIK_C);
	_byte Q = pGameInstance->Get_DIKeyState(DIK_Q);
	_byte Space = pGameInstance->Get_DIKeyState(DIK_SPACE);

	SetAnimIndex(CLINT_RUN, UPPER);
	SetAnimIndex(CLINT_RUN, LOWER);

	if (W)
	{
		if (D)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_NE);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_NE), TimeDelta);
		}
		else if (A)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_NW);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_NW), TimeDelta);
		}
		else
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_N), TimeDelta);
		}
	}
	else if (A)
	{
		if (W)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_NW);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_NW), TimeDelta);
		}
		else if (S)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_SW);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_SW), TimeDelta);
		}
		else
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_W), TimeDelta);
		}

	}
	else if (S)
	{
		if (D)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_SE);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_SE), TimeDelta);
		}
		else if (A)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_SW);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_SW), TimeDelta);
		}
		else
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_S), TimeDelta);
		}
	}
	else if (D)
	{
		if (W)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_NE);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_NE), TimeDelta);
		}
		else if (S)
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_SE);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_SE), TimeDelta);
		}
		else
		{
			pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
			pTransform->Turn(WorldAxisY(), pTransform->DirectionVector(CTransform::DIR_E), TimeDelta);
		}
	}
	else
	{
		m_pStateContext->TransitionTo(L"ClintIdle");
	}


	if (Q)
	{
		TransitionTo(L"ClintUltimate01");
	}

	if (Space)
	{
		TransitionTo(L"ClintDash");
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