#include "CollisionMgr.h"
#include "Collider.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CollisionMgr)

void CollisionMgr::Add_ColliderGroup(CCollider* pCollider, COLL_GROUP eCollGroup)
{
	if (eCollGroup >= COLL_END)
		return;

	m_Colliders[eCollGroup].push_back(pCollider);
	Safe_AddRef(pCollider);
}

void CollisionMgr::Late_Tick(_double TimeDelta)
{
	CheckByCollGroup(PLAYER_BODY, MONSTER_DETECT, TimeDelta);
	CheckByCollGroup(PLAYER_BODY, MONSTER_BULLET, TimeDelta);
	CheckByCollGroup(PLAYER_BULLET, MONSTER_BODY, TimeDelta);
	CheckByCollGroup(MONSTER_BODY, MONSTER_BODY, TimeDelta);

	ClearAllColliders();
}

void CollisionMgr::CheckByCollGroup(COLL_GROUP eSourGroup, COLL_GROUP eDestGroup, _double TimeDelta)
{
	for (auto& SourCollider : m_Colliders[eSourGroup])
	{
		for (auto& DestCollider : m_Colliders[eDestGroup])
		{
			if (SourCollider == DestCollider)
				continue;

				CCollider::COLLISION_INFO tSourCollisionInfo;
				CCollider::COLLISION_INFO tDestCollisionInfo;
			if (SourCollider->Intersect(DestCollider, tSourCollisionInfo))
			{
				tSourCollisionInfo.pOtherGameObject = DestCollider->GetOwner();
				tSourCollisionInfo.OtherGameObejctName = tSourCollisionInfo.pOtherGameObject->Get_Name();
				tSourCollisionInfo.OtherGameObjectLayerName = tSourCollisionInfo.pOtherGameObject->GetLayerName();
				tSourCollisionInfo.pOtherCollider = DestCollider;
				tSourCollisionInfo.OtherCollName = DestCollider->GetComponentName();
				tSourCollisionInfo.pMyCollider = SourCollider;
				tSourCollisionInfo.MyCollName = SourCollider->GetComponentName();
				SourCollider->OnCollision(tSourCollisionInfo, TimeDelta);
				
				tDestCollisionInfo.pOtherGameObject = SourCollider->GetOwner();
				tDestCollisionInfo.OtherGameObejctName = tDestCollisionInfo.pOtherGameObject->Get_Name();
				tDestCollisionInfo.OtherGameObjectLayerName = tDestCollisionInfo.pOtherGameObject->GetLayerName();
				tDestCollisionInfo.pOtherCollider = SourCollider;
				tDestCollisionInfo.OtherCollName = SourCollider->GetComponentName();
				tDestCollisionInfo.pMyCollider = DestCollider;
				tDestCollisionInfo.MyCollName = DestCollider->GetComponentName();
				XMStoreFloat3(&tDestCollisionInfo.vOverLapVector, -1 * XMLoadFloat3(&tSourCollisionInfo.vOverLapVector));
				DestCollider->OnCollision(tDestCollisionInfo, TimeDelta);

				SourCollider->SetIsColl(true);
				DestCollider->SetIsColl(true);
			}
		}
	}
}

void CollisionMgr::ClearAllColliders()
{
	for (_uint i = 0; i < COLL_END; ++i)
	{
		for (auto& iter : m_Colliders[i])
		{
			Safe_Release(iter);
		}
		m_Colliders[i].clear();
	}
}

void CollisionMgr::Free()
{
	ClearAllColliders();
}
