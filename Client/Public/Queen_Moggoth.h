
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
class Observer;
END

BEGIN(Client)

class Bullet;
class BossHP;

template<typename OWNER, typename ANIM_ENUM>
class StateContext;

END

BEGIN(Client)
class Queen_Moggoth final : public CGameObject
{
	typedef StateContext<Queen_Moggoth, QUEEN_MOGGOTH_ANIM> Queen_MoggothState;
public:
	typedef struct tagQueen_MoggothDesc : public tagCGameObjectDesc
	{
		tagQueen_MoggothDesc() : tagCGameObjectDesc() {}
	};
private:
	Queen_Moggoth(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Queen_Moggoth(const Queen_Moggoth& rhs);
	virtual ~Queen_Moggoth() = default;

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
	CCollider* m_pAttack04CollCom = { nullptr };
	Queen_MoggothState* m_pStateContextCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	Raycast* m_pRaycastCom = { nullptr };
	BossHP* m_pHealthCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_Notify();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Queen_Moggoth_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Queen_Moggoth"; }
	static Queen_Moggoth* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END


