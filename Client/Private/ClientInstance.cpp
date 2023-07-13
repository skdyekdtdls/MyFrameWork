#include "ClientInstance.h"
#include "GameInstance.h"
#include "Clint.h"
IMPLEMENT_SINGLETON(ClientInstance)

_vector ClientInstance::GetClintPosition()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CTransform* pTransform = static_cast<CTransform*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Transform"));

	_vector vClintPos = pTransform->Get_State(CTransform::STATE_POSITION);
	Safe_Release(pGameInstance);

	return vClintPos;
}

_bool ClientInstance::IntersectClintBodyColl(CCollider* pOtherCollider)
{
	CColliderAABB* pCollBodyColl = GetClintBodyColl();
	return pCollBodyColl->Intersect(pOtherCollider);
}

Clint* ClientInstance::GetClint()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	Clint* pClint = static_cast<Clint*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1"));
	
	
	Safe_Release(pGameInstance);
	return pClint;
}

CColliderAABB* ClientInstance::GetClintBodyColl()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CColliderAABB* pCollAABB = static_cast<CColliderAABB*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_BodyColl"));
	Safe_Release(pGameInstance);

	return pCollAABB;
}

CNavigation* ClientInstance::GetClintNavigation()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CNavigation* pNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Navigation"));
	Safe_Release(pGameInstance);

	return pNavigation;
}

void ClientInstance::Free(void)
{
}