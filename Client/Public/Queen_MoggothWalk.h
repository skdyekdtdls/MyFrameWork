#pragma once
#include "Client_Defines.h"
#include "StateMachine.h"

BEGIN(Client)
class Queen_Moggoth;
class Queen_MoggothWalk final : public StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>
{
	enum PATTERN { WALK_FWD, WALK_BACK, WALK_LEFT, WALK_RIGHT, PATTERN_END	};
private:
	Queen_MoggothWalk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_MoggothWalk(const Queen_MoggothWalk& rhs);
	virtual ~Queen_MoggothWalk() = default;

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
	void WalkFwd(const _double& TimeDelta);
	void WalkBack(const _double& TimeDelta);
	void WalkLeft(const _double& TimeDelta);
	void WalkRight(const _double& TimeDelta);
	void SetAnimIndexByPattern();
public:
	static const _tchar* Tag() { return L"Queen_MoggothWalk"; }
	static Queen_MoggothWalk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual StateMachine<Queen_Moggoth, QUEEN_MOGGOTH_ANIM>* Clone(void* pArg) override;
	virtual void Free() override;
};

END