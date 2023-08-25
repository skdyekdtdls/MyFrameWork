
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
class Raycast;
class Health;
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class Bullet;
class MonsterHP;
template<typename OWNER, typename ANIM_ENUM>
class StateContext;
class Dissolve;
END

BEGIN(Client)
class BatPotato_RIG final : public CGameObject
{
	typedef StateContext<BatPotato_RIG, BATPOTATO_RIG_ANIM> BatPotatoRIGState;
public:
	typedef struct tagBatPotato_RIGDesc : public tagCGameObjectDesc
	{
		tagBatPotato_RIGDesc() : tagCGameObjectDesc() {}
		_uint iStartIndex;
	}BATPOTATO_RIG_DESC;
private:
	BatPotato_RIG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIG(const BatPotato_RIG& rhs);
	virtual ~BatPotato_RIG() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CCollider::COLLISION_INFO pCollisionInfo, _double TimeDelta) override;	
	void ResetPool(void* pArg);
	void SetPass(_uint iPass) {
		m_iPass = iPass;
	}
private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	Raycast* m_pRaycastCom = { nullptr };
	BatPotatoRIGState* m_pStateContextCom = { nullptr };
	MonsterHP* m_pMonsterHP = { nullptr };
	Bullet* m_pBullet = { nullptr };
	Dissolve* m_pDissolveCom = { nullptr };

private:
	_uint m_iPass = { 0 };
	_double m_TimeDelta;
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint BatPotato_RIG_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_BatPotato_RIG1"; }
	static BatPotato_RIG* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

