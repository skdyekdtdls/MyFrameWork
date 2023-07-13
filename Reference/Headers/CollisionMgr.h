#pragma once

#include "Base.h"

BEGIN(Engine)
enum COLLGROUP { CG_OBJECT, CG_BULLET, CG_ITEM, COLLGROUP_END };
enum TEAM { TEAM_1, TEAM_2, TEAM_END }; // °°Àº ÆÀ³¢¸®´Â Ãæµ¹¾ÈÇÔ.

class CCollider;

class CollisionMgr final : public CBase
{
	typedef list<CCollider*> Colliders;

	DECLARE_SINGLETON(CollisionMgr);
public:
	CollisionMgr() = default;
	~CollisionMgr() = default;

public:
	void Add_Colliders(list<CCollider*> pCollider, COLLGROUP eCollGroup, TEAM eTeam);
	void Late_Tick();
	void CheckByCollGroup(COLLGROUP eSourGroup, COLLGROUP eDestGroup);

private:
	list<Colliders> m_Colliders[COLLGROUP_END][TEAM_END];

private:
	void ClearColliders();

public:
	void Free() override;
};

END