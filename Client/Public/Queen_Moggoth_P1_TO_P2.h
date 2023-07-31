
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_Moggoth_P1_TO_P2 final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
private:
	Queen_Moggoth_P1_TO_P2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_Moggoth_P1_TO_P2(const Queen_Moggoth_P1_TO_P2& rhs);
	virtual ~Queen_Moggoth_P1_TO_P2() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

	virtual const _tchar* GetTag() override { return Tag(); }
public:
	static const _tchar* Tag() { return L"Queen_Moggoth_P1_TO_P2"; }
	static Queen_Moggoth_P1_TO_P2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END