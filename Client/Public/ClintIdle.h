#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintIdle final : public StateMachine<Clint, CLINT_ANIM>
{
private:
	ClintIdle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintIdle(const ClintIdle& rhs);
	virtual ~ClintIdle() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"ClintIdle"; }
	static ClintIdle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END