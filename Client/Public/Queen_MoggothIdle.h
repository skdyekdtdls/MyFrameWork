
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothIdle final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
private:
	Queen_MoggothIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothIdle(const Queen_MoggothIdle& rhs);
	virtual ~Queen_MoggothIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };

public:
	static const _tchar* Tag() { return L"Queen_MoggothIdle"; }
	static Queen_MoggothIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END