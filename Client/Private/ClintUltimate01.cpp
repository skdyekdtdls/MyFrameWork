#include "..\Public\ClintUltimate01.h"
#include "GameInstance.h"
#include "Clint.h"
#include "ClintUltimate01Bullet.h"
#include "SoundMgr.h"
#include "BoomEffect.h"
#include "SkillUI.h"
ClintUltimate01::ClintUltimate01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<Clint, CLINT_ANIM>(pDevice, pContext)
{
}

ClintUltimate01::ClintUltimate01(const ClintUltimate01& rhs)
	: StateMachine<Clint, CLINT_ANIM>(rhs)
	, m_TimeAcc(0.0)
	, m_Duration(6.0)
{
}

void ClintUltimate01::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CLINT_ULTIMATE01, LOWER);
	SetAnimIndex(CLINT_ULTIMATE01, UPPER);
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pBoomEffect = static_cast<BoomEffect*>(m_pOwner->Get_Component(L"Com_BoomEffect"));
	m_pBoomEffect->SetPos(pTransform->Get_State(CTransform::STATE_POSITION));
	m_TimeAcc = { 0.0 };
	m_UltTimeAcc = { 0.0 };

	SkillUI* pSkillUICom = static_cast<SkillUI*>(m_pOwner->Get_Component(L"Com_SkillQ_UI"));
	pSkillUICom->UseSkill();

}

void ClintUltimate01::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	ClintUltimate01Bullet* pUltBullet = static_cast<ClintUltimate01Bullet*>(m_pOwner->Get_Component(L"Com_"));

	_byte W = pGameInstance->Get_DIKeyState(DIK_S);
	_byte A = pGameInstance->Get_DIKeyState(DIK_Z);
	_byte S = pGameInstance->Get_DIKeyState(DIK_X);
	_byte D = pGameInstance->Get_DIKeyState(DIK_C);

	Safe_Release(pGameInstance);
	
	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= m_Duration)
	{
		TransitionTo(L"ClintIdle");
	}

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

	m_UltTimeAcc += TimeDelta;

	m_pBoomEffect->Tick(TimeDelta);
	m_pBoomEffect->Late_Tick(TimeDelta);

	if (m_UltTimeAcc > 0.25)
	{
		m_pBoomEffect->Reset_Effects();
		m_pBoomEffect->SetPos(pTransform->Get_State(CTransform::STATE_POSITION));
		m_UltTimeAcc = 0.0;
		ClintUltimate01Bullet* pUltBullet = static_cast<ClintUltimate01Bullet*>(m_pOwner->Get_Component(L"Com_UltBullet"));
		pUltBullet->Enable();
		SoundMgr->StopSound(CHANNELID::PLAYER_BULLET);
		SoundMgr->PlaySound(L"pistol_shot2.ogg", CHANNELID::PLAYER_BULLET, 1.f);
	}


}

void ClintUltimate01::OnStateExit()
{
	
	__super::OnStateExit();
}

StateMachine<Clint, CLINT_ANIM>* ClintUltimate01::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new ClintUltimate01(*this);
}

ClintUltimate01* ClintUltimate01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new ClintUltimate01(pDevice, pContext);
}

void ClintUltimate01::Free()
{
	__super::Free();
}