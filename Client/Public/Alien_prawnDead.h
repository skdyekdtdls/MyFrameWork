
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Alien_prawn;
class Alien_prawnDead final : public StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>
{
private:
	Alien_prawnDead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawnDead(const Alien_prawnDead& rhs);
	virtual ~Alien_prawnDead() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"Alien_prawnDead"; }
	static Alien_prawnDead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END




