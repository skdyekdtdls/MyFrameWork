#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CColliderAABB;
class CCollider;
class CNavigation;
END

BEGIN(Client)
class ClientInstance : public CBase
{
	DECLARE_SINGLETON(ClientInstance);
public:
	explicit ClientInstance() = default;
	virtual ~ClientInstance() = default;

	_vector GetClintPosition();
	_bool IntersectClintBodyColl(CCollider* pOtherCollider);
	class CNavigation* GetClintNavigation();

private:
	class Clint* GetClint();
	class CColliderAABB* GetClintBodyColl();
public:
	virtual void Free(void) override;
};

#define Facade ClientInstance::GetInstance() 
END