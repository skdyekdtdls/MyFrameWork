
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

template<typename OWNER, typename ANIM_ENUM>
class StateContext;

END

BEGIN(Client)
class CrystalGolem final : public CGameObject
{
	typedef StateContext<CrystalGolem, CRYSTAL_GOLEM_ANIM> CrystalGolemState;
public:
	typedef struct tagCrystalGolemDesc : public tagCGameObjectDesc
	{
		tagCrystalGolemDesc() : tagCGameObjectDesc() {}
	};
private:
	CrystalGolem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CrystalGolem(const CrystalGolem& rhs);
	virtual ~CrystalGolem() = default;

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
	CrystalGolemState* m_pStateContextCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderDectect = { nullptr };
	Health* m_pHealthCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT Add_Notify();
	HRESULT SetUp_ShaderResources();

private:
	static _uint CrystalGolem_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_CrystalGolem"; }
	static CrystalGolem* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END