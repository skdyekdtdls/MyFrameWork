#include "CollisionMgr.h"
#include "Collider.h"
IMPLEMENT_SINGLETON(CollisionMgr)


void CollisionMgr::Add_Colliders(list<CCollider*> pCollider, COLLGROUP eCollGroup, TEAM eTeam)
{
	if (eCollGroup >= COLLGROUP_END)
		return;

	if (eTeam >= TEAM_END)
		return;

	if (pCollider.empty())
		return;

	m_Colliders[eCollGroup][eTeam].emplace_back(pCollider);
}

void CollisionMgr::Late_Tick()
{
	CheckByCollGroup(CG_OBJECT, CG_BULLET);
}

void CollisionMgr::CheckByCollGroup(COLLGROUP eSourGroup, COLLGROUP eDestGroup)
{
	for (auto& SourColliders : m_Colliders[eSourGroup][TEAM_1])
	{
		for (auto& SourCollider : SourColliders)
		{
			for (auto& DestColliders : m_Colliders[eDestGroup][TEAM_2])
			{
				for (auto& DestCollider : DestColliders)
				{
					if (DestCollider->Intersect(SourCollider))
					{
						CCollider::COLLISION_INFO tCollisionInfo;

						tCollisionInfo.pOtherCollider = SourCollider;
						DestCollider->OnCollision(&tCollisionInfo);

						tCollisionInfo.pOtherCollider = DestCollider;
						SourCollider->OnCollision(&tCollisionInfo);
					}
				}
			}
		}
	}

	for (auto& SourColliders : m_Colliders[eSourGroup][TEAM_2])
	{
		for (auto& SourCollider : SourColliders)
		{
			for (auto& DestColliders : m_Colliders[eDestGroup][TEAM_1])
			{
				for (auto& DestCollider : DestColliders)
				{
					if (DestCollider->Intersect(SourCollider))
					{
						CCollider::COLLISION_INFO tCollisionInfo;

						tCollisionInfo.pOtherCollider = SourCollider;
						DestCollider->OnCollision(&tCollisionInfo);

						tCollisionInfo.pOtherCollider = DestCollider;
						SourCollider->OnCollision(&tCollisionInfo);
					}
				}
			}
		}
	}
}

void CollisionMgr::ClearColliders()
{
}

void CollisionMgr::Free()
{
}
