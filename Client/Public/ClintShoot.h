#pragma once
#include "ClintState.h"

BEGIN(Client)
class ClintShoot final : public ClintState
{
private:
	ClintShoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual ~ClintShoot() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

public:
	static ClintShoot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, Clint * pClint);
	virtual void Free() override;
};
END
