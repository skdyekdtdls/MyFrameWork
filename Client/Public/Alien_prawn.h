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
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class MonsterHP;
template<typename OWNER, typename ANIM_ENUM>
class StateContext;
class Dissolve;
END

BEGIN(Client)
class Alien_prawn final : public CGameObject
{
	typedef StateContext<Alien_prawn, ALIEN_PRAWN_ANIM> AlienPrawnState;
public:
	typedef struct tagAlien_prawnDesc : public tagCGameObjectDesc
	{
		tagAlien_prawnDesc() : tagCGameObjectDesc() {}
		_uint iStartIndex;
	}ALIEN_PRAWN_DESC;

private:
	Alien_prawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Alien_prawn(const Alien_prawn& rhs);
	virtual ~Alien_prawn() = default;

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
	AlienPrawnState* m_pStateContextCom = { nullptr };
	MonsterHP* m_pMonsterHP = { nullptr };
	Dissolve* m_pDissolveCom = { nullptr };

private:
	_uint m_iPass = { 0 };
	_double m_TimeDelta;
private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	// idle attack run
	static _uint Alien_prawn_Id;
	
public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Alien_prawn1"; }
	static Alien_prawn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;


};
END

