
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class CrystalGolem;
class CrystalGolemRun final : public StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>
{
private:
	CrystalGolemRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CrystalGolemRun(const CrystalGolemRun& rhs);
	virtual ~CrystalGolemRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };

public:
	static const _tchar* Tag() { return L"CrystalGolemRun"; }
	static CrystalGolemRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<CrystalGolem, CRYSTAL_GOLEM_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END