
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
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class Bullet abstract : public CGameObject
{
public:
	typedef struct tagBulletDesc : public tagCGameObjectDesc
	{
		tagBulletDesc() : tagCGameObjectDesc() {}
		_float fDamage;
	};

protected:
	Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Bullet(const Bullet& rhs);
	virtual ~Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public: // Setter
	void Damage(CGameObject* pGameObejct);
	_bool isMonsterLayer(wstring ObjectName) {
		return ObjectName == L"Layer_Monster";
	}
	
	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

public: // Getter
	_float GetDamage() { return m_fDamage; }

protected:
	_float	m_fDamage = { 100.f };
	_bool	m_bEnable = { true };

public:
	virtual void Free(void) override;
};
END