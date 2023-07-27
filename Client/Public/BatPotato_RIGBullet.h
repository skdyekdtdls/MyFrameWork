
// 주의할점 : DESC이름, 
#pragma once

#include "Client_Defines.h"
#include "Bullet.h"
BEGIN(Engine)

class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CCollider;
class CNavigation;
class CModel;
class TimeCounter;
// Can declare VIBuffer or Model Com
END

BEGIN(Client)
class BatPotato_RIGBullet final : public Bullet
{
public:
	typedef struct tagBatPotato_RIGBulletDesc : public tagBulletDesc
	{
		tagBatPotato_RIGBulletDesc() : tagBulletDesc() {}
		_vector vLook;
	};

private:
	BatPotato_RIGBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BatPotato_RIGBullet(const BatPotato_RIGBullet& rhs);
	virtual ~BatPotato_RIGBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Ready(_fvector vLook, _fvector vPosition);
public:
	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

private: /* For. Component */
	//CShader* m_pShaderCom = { nullptr };
	//CModel* m_pModelCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	TimeCounter* m_pTimeCounterCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint BatPotato_RIGBullet_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_BatPotato_RIGBullet"; }
	static BatPotato_RIGBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Bullet* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END
