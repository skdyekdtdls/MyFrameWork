#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Clint;
class ClintRun final : public StateMachine<Clint, CLINT_ANIM>
{
	typedef struct tagClintRunDesc : public tagStateMachineDesc
	{
		tagClintRunDesc() : tagStateMachineDesc() {}
	}CLINT_RUN_DESC;
private:
	ClintRun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintRun(const ClintRun& rhs);
	virtual ~ClintRun() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"ClintRun"; }
	static ClintRun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Clint, CLINT_ANIM>* Clone(void* pArg);
	virtual void Free() override;
};

END