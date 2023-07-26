
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Alien_prawn;
class Alien_prawnAttack final : public StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>
{
private:
	Alien_prawnAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawnAttack(const Alien_prawnAttack& rhs);
	virtual ~Alien_prawnAttack() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
private:
	_int iAttackIndex = { 0 };
private:
	void RandomAttack();
public:
	static const _tchar* Tag() { return L"Alien_prawnAttack"; }
	static Alien_prawnAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Alien_prawn, ALIEN_PRAWN_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END