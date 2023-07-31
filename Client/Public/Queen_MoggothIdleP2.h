
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothIdleP2 final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
private:
	Queen_MoggothIdleP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothIdleP2(const Queen_MoggothIdleP2& rhs);
	virtual ~Queen_MoggothIdleP2() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };

public:
	static const _tchar* Tag() { return L"Queen_MoggothIdleP2"; }
	static Queen_MoggothIdleP2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END