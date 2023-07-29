
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CrystalGolem;
class CrystalGolemAttack final : public StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>
{
private:
	CrystalGolemAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CrystalGolemAttack(const CrystalGolemAttack& rhs);
	virtual ~CrystalGolemAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;
	virtual const _tchar* GetTag() override { return Tag(); }

	_int m_iLogic = { 0 };
	_int m_iOrder = { 0 };
public:
	static const _tchar* Tag() { return L"CrystalGolemAttack"; }
	static CrystalGolemAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END
