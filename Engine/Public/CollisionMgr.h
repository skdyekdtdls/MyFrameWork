#pragma once

#include "Base.h"

BEGIN(Engine)
enum COLL_GROUP {PLAYER_BODY
	, PLAYER_BULLET, MONSTER_BODY, MONSTER_BULLET
	, MONSTER_DETECT, COLL_END};

class CCollider;

class CollisionMgr final : public CBase
{
	DECLARE_SINGLETON(CollisionMgr);
public:
	CollisionMgr() = default;
	~CollisionMgr() = default;

public:
	void Add_ColliderGroup(CCollider* pCollider, COLL_GROUP eCollGroup);

	void Late_Tick(_double TimeDelta);
	void CheckByCollGroup(COLL_GROUP eSourGroup, COLL_GROUP eDestGroup, _double TimeDelta);

private:
	list<CCollider*> m_Colliders[COLL_END];

private:
	void ClearAllColliders();

public:
	void Free() override;
};

END