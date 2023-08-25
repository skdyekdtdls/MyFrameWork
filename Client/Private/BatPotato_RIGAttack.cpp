#include "..\Public\BatPotato_RIGAttack.h"
#include "GameInstance.h"
#include "BatPotato_RIG.h"
#include "SoundMgr.h"
BatPotato_RIGAttack::BatPotato_RIGAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(pDevice, pContext)
{
}

BatPotato_RIGAttack::BatPotato_RIGAttack(const BatPotato_RIGAttack& rhs)
	: StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>(rhs)
{
}

void BatPotato_RIGAttack::OnStateEnter()
{
	__super::OnStateEnter();

	SoundMgr->StopSound(BAT);
	SoundMgr->PlaySound(L"BatAttack.ogg", CHANNELID::BAT, 0.4f);

	SetAnimIndex(BATPOTATO_RIG_ATTACK01, LOWER);
}

void BatPotato_RIGAttack::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		TransitionTo(L"BatPotato_RIGIdle");
	else
		pModel->RootMotion(TimeDelta, pTransform->Get_State(CTransform::STATE_LOOK));
	
	Safe_Release(pGameInstance);
}

void BatPotato_RIGAttack::OnStateExit()
{
	__super::OnStateExit();
}

StateMachine<BatPotato_RIG, BATPOTATO_RIG_ANIM>* BatPotato_RIGAttack::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new BatPotato_RIGAttack(*this);
}

BatPotato_RIGAttack* BatPotato_RIGAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new BatPotato_RIGAttack(pDevice, pContext);
}

void BatPotato_RIGAttack::Free()
{
	__super::Free();
}