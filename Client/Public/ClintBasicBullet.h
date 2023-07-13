
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
class ClintBasicBullet final : public CGameObject
{
public:
	typedef struct tagClintBasicBulletDesc : public tagCGameObjectDesc
	{
		tagClintBasicBulletDesc() : tagCGameObjectDesc() {}

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

private: /* For. Component */
	//CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CColliderSphere* m_pColliderCom = { nullptr };
	//CModel* m_pModelCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();
	void CheckCollision(_double TimeDelta);
private:
	_double m_TimeAcc = { 0.0 };
	_double m_LifeSpan = { 1.0 };
	static _uint ClintBasicBullet_Id;

private:
	void LifeSpan(_double TimeDelta);

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_ClintBasicBullet"; }
	static ClintBasicBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

