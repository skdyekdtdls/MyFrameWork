
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
class P1Attack01 final : public Bullet
{
public:
	typedef struct tagP1Attack01Desc : public tagBulletDesc
	{
		tagP1Attack01Desc() : tagBulletDesc() {}
		_vector vTargetPos;
	};

private:
	P1Attack01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	P1Attack01(const P1Attack01& rhs);
	virtual ~P1Attack01() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void ResetPool(void* pArg);
	virtual void SetDead() override;

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
	static _uint P1Attack01_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_P1Attack01"; }
	static P1Attack01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Bullet* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END