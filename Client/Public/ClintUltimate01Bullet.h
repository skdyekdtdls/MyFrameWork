
#pragma once

#include "Client_Defines.h"
#include "Bullet.h"
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
class ClintUltimate01Bullet final : public Bullet
{
	typedef struct tagClintUltimate01BulletDesc : public tagBulletDesc
	{
		tagClintUltimate01BulletDesc() : tagBulletDesc() {}
	}CLINT_ULTIMATE01_BULLET_DESC;
private:
	ClintUltimate01Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ClintUltimate01Bullet(const ClintUltimate01Bullet& rhs);
	virtual ~ClintUltimate01Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Enable() { m_bEnable = true; }
	void Disable() { m_bEnable = false; };

private: /* For. Component */
	//CShader* m_pShaderCom = { nullptr };
	//CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint ClintUltimate01Bullet_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_ClintUltimate01Bullet"; }
	static ClintUltimate01Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;


};
END

