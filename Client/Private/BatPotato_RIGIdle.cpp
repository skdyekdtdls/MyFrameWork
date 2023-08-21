#include "..\Public\BatPotato_RIGIdle.h"
#include "GameInstance.h"
#include "BatPotato_RIG.h"
#include "SoundMgr.h"

BatPotato_RIGIdle::BatPotato_RIGIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(pDevice, pContext)
{
}

BatPotato_RIGIdle::BatPotato_RIGIdle(const BatPotato_RIGIdle& rhs)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(rhs)
{
}

void BatPotato_RIGIdle::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(BATPOTATO_RIG_IDLE, LOWER);
	SoundMgr->PlaySound(L"BatIdle.ogg", CHANNELID::BAT, 0.4f);
	m_TimeAcc = { 0.0 };
}

void BatPotato_RIGIdle::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_TimeAcc += TimeDelta;
	if (m_TimeAcc >= 1.f)
		TransitionTo(L"BatPotato_RIGRun");

	Safe_Release(pGameInstance);
}

void BatPotato_RIGIdle::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* BatPotato_RIGIdle::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new BatPotato_RIGIdle(*this);
}

BatPotato_RIGIdle* BatPotato_RIGIdle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new BatPotato_RIGIdle(pDevice, pContext);
}

void BatPotato_RIGIdle::Free()
{
	__super::Free();
}