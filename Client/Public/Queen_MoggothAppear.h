
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothAppear final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
private:
	Queen_MoggothAppear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothAppear(const Queen_MoggothAppear& rhs);
	virtual ~Queen_MoggothAppear() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"Queen_MoggothAppear"; }
	static Queen_MoggothAppear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END