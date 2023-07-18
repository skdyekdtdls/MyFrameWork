
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
	}CLINT_BASIC_BULLET_DESC;

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

protected:
	_double m_LifeSpan = { 1.0 };
	_double m_TimeAcc = { 0.0 };
protected: /* For. Component */
//CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CColliderSphere* m_pColliderCom = { nullptr };

private:
	void LifeSpan(_double TimeDelta);
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	virtual void Free(void) override;
};
END