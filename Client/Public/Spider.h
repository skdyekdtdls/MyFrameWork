
// State 구현하기
// Bullet 구현하기
// 로더에 넣기.
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
class MonsterHP;
template<typename OWNER, typename ANIM_ENUM>
class StateContext;

END

BEGIN(Client)
class Spider : public CGameObject
{
protected:
	typedef StateContext<Spider, SPIDER_ANIM> SpiderState;

public:
	typedef struct tagSpiderDesc : public tagCGameObjectDesc
	{
		tagSpiderDesc() : tagCGameObjectDesc() {}
		_uint iStartIndex;
	};

private:
	Spider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Spider(const Spider& rhs);
	virtual ~Spider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);
	void ResetPool(void* pArg);

private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	SpiderState* m_pStateContextCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	Raycast* m_pRaycastCom = { nullptr };
	MonsterHP* m_pMonsterHP = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT SetUp_Notify();
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Spider_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Spider"; }
	static Spider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END


