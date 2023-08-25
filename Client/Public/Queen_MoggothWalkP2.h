
#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothWalkP2 final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
	enum PATTERN { WALK_BACK, WALK_BL, WALK_BR, WALK_FL, WALK_FR, WALK_FWD, WALK_LEFT, WALK_RIGHT, PATTERN_END };
private:
	Queen_MoggothWalkP2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothWalkP2(const Queen_MoggothWalkP2& rhs);
	virtual ~Queen_MoggothWalkP2() = default;

public:
	virtual void OnStateEnter() override;
	virtual void OnStateTick(_double TimeDelta) override;
	virtual void OnStateExit() override;

	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;
	virtual const _tchar* GetTag() override { return Tag(); }

private:
	_double m_TimeAcc = { 0.0 };
	PATTERN m_ePattern = { PATTERN_END };
	CModel* m_pModel = { nullptr };
	CTransform* m_pTransform = { nullptr };
	CNavigation* m_pNavigation = { nullptr };

private:
	void SetAnimIndexByPattern();
public:
	static const _tchar* Tag() { return L"Queen_MoggothWalkP2"; }
	static Queen_MoggothWalkP2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END