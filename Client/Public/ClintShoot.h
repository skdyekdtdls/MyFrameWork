#pragma once

#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintShoot final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintShoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintShoot(const ClintShoot& rhs);
	virtual ~ClintShoot() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static const _tchar* Tag() { return L"ClintShoot"; }
	static ClintShoot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END
