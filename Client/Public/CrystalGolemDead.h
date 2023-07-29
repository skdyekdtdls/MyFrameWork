
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CrystalGolem;
class CrystalGolemDead final : public StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>
{
private:
	CrystalGolemDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CrystalGolemDead(const CrystalGolemDead& rhs);
	virtual ~CrystalGolemDead() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CrystalGolemDead"; }
	static CrystalGolemDead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END


#include "..\Public\CrystalGolemDead.h"
#include "GameInstance.h"
#include "CrystalGolem.h"

CrystalGolemDead::CrystalGolemDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(pDevice, pContext)
{
}

CrystalGolemDead::CrystalGolemDead(const CrystalGolemDead& rhs)
	: StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>(rhs)
{
}

void CrystalGolemDead::OnStateEnter()
{
	__super::OnStateEnter();

	SetAnimIndex(CRYSTAL_GOLEM_DEATH, LOWER);
}

void CrystalGolemDead::OnStateTick(_double TimeDelta)
{
	__super::OnStateTick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CModel* pModel = static_cast<CModel*>(m_pOwner->Get_Component(L"Com_Model"));
	CTransform* pTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));

	if (pModel->IsAnimationFinished())
		m_pOwner->SetDead();
	Safe_Release(pGameInstance);
}

void CrystalGolemDead::OnStateExit()
{
	__super::OnStateExit();
}

void CrystalGolemDead::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 플레이어랑 레이랑 충돌하면 공격상태로 전환
	if (tCollisionInfo.pMyCollider == m_pOwner->GetComponent<Raycast>() &&
		tCollisionInfo.pOtherGameObject->GetInfo().wstrKey == TEXT("Prototype_GameObject_Clint"))
	{
		TransitionTo(L"CrystalGolemAttack");
	}
}

StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* CrystalGolemDead::Clone(void* pArg)
{
	__super::Initialize(pArg);
	return new CrystalGolemDead(*this);
}

CrystalGolemDead* CrystalGolemDead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CrystalGolemDead(pDevice, pContext);
}

void CrystalGolemDead::Free()
{
	__super::Free();
}


