
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Alien_prawn;
class Alien_prawnRun final : public StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>
{
private:
	Alien_prawnRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawnRun(const Alien_prawnRun& rhs);
	virtual ~Alien_prawnRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

public:
	static const _tchar* Tag() { return L"Alien_prawnRun"; }
	static Alien_prawnRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END