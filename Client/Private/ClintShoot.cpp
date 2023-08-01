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
	SetAnimIndex(CLINT_SHOOT, UPPER);
	SetAnimIndex(CLINT_SHOOT, LOWER);
}

void ClintShoot::OnStateTick(_double TimeDelta)
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

	_vector vMoveDir = XMVectorZero();
	_vector vLookDir = pTransform->Get_State(CTransform::STATE_LOOK);
	
	if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		
		pTransform->Set_Speed(3.5f);
		_bool Calc = false;
		if (W)
		{
			Calc = true;
			if (D)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_NE);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_NE);
			}
			else if(A)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_NW);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_NW);
			}
			else
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_N);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_N);
			}

		}
		else if (A)
		{
			Calc = true;
			if (W)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_NW);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_NW);
			}
			else if (S)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_SW);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_SW);
			}
			else
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_W);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_W);
			}
		}
		else if (S)
		{
			Calc = true;
			if (A)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_SW);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_SW);
			}
			else if (D)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_SE);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_SE);
			}
			else
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_S);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_S);
			}
		}
		else if (D)
		{
			Calc = true;
			if (W)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_NE);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_NE);
			}
			else if (S)
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_SE);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_SE);
			}
			else
			{
				pTransform->Go_Direction(TimeDelta, CTransform::DIR_E);
				vMoveDir = pTransform->DirectionVector(CTransform::DIR_E);
			}
		}
		else
			SetAnimIndex(CLINT_SHOOT, LOWER);
		pTransform->Set_Speed(7.f);
		pTransform->LookAt(Single->PickPosOnTerrain());
		
		if (true == Calc) // 어떤 하체걷기 모션이 재생되어야 할지 계산
		{
			_vector vXDir = XMVectorSet(1.f, 0.f, 0.f, 1.f);

			_float fDegreeInXMove;
			_float fDegreeOffset = 22.5f;
			_float fDegreeInXLook;
			_float fDegree;
			fDegreeInXLook = DegreeBetweenVectors(vXDir, vLookDir);
			fDegreeInXMove = DegreeBetweenVectors(vXDir, vMoveDir);
			fDegree = fabs(fDegreeInXMove - fDegreeInXLook);

			if (337.5f <fDegree &&fDegree <= 360.f 
				|| 0.f <fDegree &&fDegree <= 22.5f)
			{
				SetAnimIndex(CLINT_WALK_F, LOWER);
			}
			else if (22.5f <fDegree && fDegree <= 67.5f)
			{
				SetAnimIndex(CLINT_WALK_FL_45, LOWER);
			}
			else if (67.5f <fDegree &&fDegree <= 112.5f)
			{
				SetAnimIndex(CLINT_WALK_FL, LOWER);
			}
			else if (112.5f <fDegree &&fDegree <= 157.5f)
			{
				SetAnimIndex(CLINT_WALK_BL_45, LOWER);
			}
			else if (157.5f <fDegree &&fDegree <= 202.5f)
			{
				SetAnimIndex(CLINT_WALK_B, LOWER);
			}
			else if (202.5f <fDegree &&fDegree <= 247.5f)
			{
				SetAnimIndex(CLINT_WALK_BR_45, LOWER);
			}
			else if (247.5f <fDegree &&fDegree <= 292.5f)
			{
				SetAnimIndex(CLINT_WALK_FR, LOWER);
			}
			else if (292.5f <fDegree &&fDegree <= 337.5f)
			{
				SetAnimIndex(CLINT_WALK_FR_45, LOWER);
			}
		}
	}
	else
		TransitionTo(L"ClintIdle");

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