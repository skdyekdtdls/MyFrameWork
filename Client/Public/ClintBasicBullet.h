
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
class ClintBasicBullet final : public Bullet
{
public:
	typedef struct tagClintBasicBulletDesc : public tagBulletDesc
	{
		tagClintBasicBulletDesc() : tagBulletDesc() {}

		_vector vLook;
	}CLINT_BASIC_BULLET_DESC;

private:
	ClintBasicBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintBasicBullet(const ClintBasicBullet& rhs);
	virtual ~ClintBasicBullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Reset(void* pArg);
	

	virtual void SetDead() override
	{
		__super::SetDead();
		ObjectPool<ClintBasicBullet>::GetInstance()->PushPool(this);
	}

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
	static _uint ClintBasicBullet_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_ClintBasicBullet"; }
	static ClintBasicBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Bullet* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END

