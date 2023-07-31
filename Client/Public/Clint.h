#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "ISerializable.h"


BEGIN(Engine)

class CShader;
class CRenderer;
class CTransform;
class CModel;
class CCollider;
class CNavigation;
class Raycast;
class Health;
class TimeCounter;
END

BEGIN(Client)
class Pistola;
class ClintUltimate01Bullet;
class PlayerHP;
template <typename OWNER, typename ANIM_ENUM>
class StateContext;
END

BEGIN(Client)

class Clint final : public CGameObject, public ISerializable
{
	typedef StateContext<Clint, CLINT_ANIM> ClintState;
public:
	typedef struct tagClintDesc : public tagCGameObjectDesc
	{
		tagClintDesc() : tagCGameObjectDesc() {}
	}CLINT_DESC;

private:
	Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Clint(const Clint& rhs);
	virtual ~Clint() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_vector GetPosition();
	_vector GetLook();

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;

private: /* For. Component */
	CModel* m_pModelCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	Pistola* m_pPistolaComL = { nullptr };
	Pistola* m_pPistolaComR = { nullptr };
	Raycast* m_pRaycastCom = { nullptr };
	ClintUltimate01Bullet* m_pUltBulletCom = { nullptr };
	ClintState* m_pStateContextCom = { nullptr };
	Health* m_pHealthCom = { nullptr };
	list<PlayerHP*> m_PlayerHPs = { nullptr };

private:
	static _uint Clint_Id;

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta) override;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Clint"; }
	static Clint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END