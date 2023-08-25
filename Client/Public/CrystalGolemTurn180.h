
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CrystalGolem;
class CrystalGolemTurn180 final : public StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>
{
private:
	CrystalGolemTurn180(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CrystalGolemTurn180(const CrystalGolemTurn180& rhs);
	virtual ~CrystalGolemTurn180() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"CrystalGolemTurn180"; }
	static CrystalGolemTurn180* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END