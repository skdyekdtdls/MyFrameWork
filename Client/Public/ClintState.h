#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "IStateMachine.h"
#include "Clint.h"

BEGIN(Client)

class ClintState abstract : public CBase, public IStateMachine
{
protected:
	ClintState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Clint* pClint);
	virtual ~ClintState() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(double TimeDelta) override;
	virtual void OnStateExit() override;

	void SetOwner(Clint* pOwner) {
		m_pOwner = pOwner;
	}

	void SetAnimIndex(CLINT_ANIM eClintAnim, BODY eBody);

protected:
	Clint* m_pOwner = { nullptr };
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual void Free() override;

};

END
