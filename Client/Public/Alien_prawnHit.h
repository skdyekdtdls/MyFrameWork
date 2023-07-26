
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Alien_prawn;
class Alien_prawnHit final : public StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>
{
private:
	Alien_prawnHit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawnHit(const Alien_prawnHit& rhs);
	virtual ~Alien_prawnHit() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"Alien_prawnHit"; }
	static Alien_prawnHit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END