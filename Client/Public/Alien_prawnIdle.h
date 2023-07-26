
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(End)
class TimeCounter;
END

BEGIN(Client)
class Alien_prawn;
class Alien_prawnIdle final : public StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>
{
private:
	Alien_prawnIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawnIdle(const Alien_prawnIdle& rhs);
	virtual ~Alien_prawnIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

	virtual const _tchar* GetTag() override { return Tag(); }
private:
	_double m_TimeAcc = { 0.0 };

public:
	static const _tchar* Tag() { return L"Alien_prawnIdle"; }
	static Alien_prawnIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END