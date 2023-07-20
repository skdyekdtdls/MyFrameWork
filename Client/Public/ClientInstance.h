#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CColliderAABB;
class CCollider;
class CNavigation;
class CRenderer;
class CTransform;
END

BEGIN(Client)
class ClientInstance : public CBase
{
	DECLARE_SINGLETON(ClientInstance);
public:
	explicit ClientInstance() = default;
	virtual ~ClientInstance() = default;

	_vector GetClintPosition();
	class CNavigation* GetClintNavigation();
	class CTerrain* GetTerrain();

	RAY GetMouseRay(ID3D11DeviceContext* pContext);

	_bool isRender(CRenderer* pRenderer, CTransform* pTransform, _float fRange = 2.f);
private:
	class Clint* GetClint();
	class CColliderAABB* GetClintBodyColl();
public:
	virtual void Free(void) override;
};

#define Facade ClientInstance::GetInstance() 
END
