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
// Can declare VIBuffer or Model Com
END

BEGIN(Client)

class EnemyBasicBullet : public Bullet
{
public:
	typedef struct tagEnemyBasicBulletDesc : public tagBulletDesc
	{
		tagEnemyBasicBulletDesc() : tagBulletDesc() {};
		_float fRadius;
	}ENEMY_BASIC_BULLET_DESC;

private:
	explicit EnemyBasicBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit EnemyBasicBullet(const EnemyBasicBullet& rhs);
	virtual ~EnemyBasicBullet() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta);

private:
	//CShader* m_pShaderCom = { nullptr };
	//CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };

private:
	HRESULT Add_Components(_float& vExtents);
	HRESULT SetUp_ShaderResources();

private:
	static _uint EnemyBasicBullet_Id;
public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_EnemyBasicBullet"; }
	static EnemyBasicBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Bullet* Clone(void* pArg) override;
	virtual void Free() override;
};

END


