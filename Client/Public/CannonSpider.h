#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CCollider;
class CNavigation;
class CModel;
class TimeCounter;
END

BEGIN(Client)

class Bullet;

template<typename OWNER, typename ANIM_ENUM>
class StateContext;

END

BEGIN(Client)
class CannonSpider final : public CGameObject
{
	typedef StateContext<CannonSpider, CANNON_SPIDER_ANIM> CannonSpiderState;
public:
	typedef struct tagCannonSpiderDesc : public tagCGameObjectDesc
	{
		tagCannonSpiderDesc() : tagCGameObjectDesc() {}
	};
private:
	CannonSpider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CannonSpider(const CannonSpider& rhs);
	virtual ~CannonSpider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CannonSpiderState* m_pStateContextCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	Raycast* m_pRaycastCom = { nullptr };
	Health* m_pHealthCom = { nullptr };
	list<Bullet*> m_pBullets = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint CannonSpider_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_CannonSpider"; }
	static CannonSpider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END
