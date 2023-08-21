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
class Bullet01Effect final : public CGameObject
{
	typedef struct tagBullet01Desc : public tagCGameObjectDesc
	{
		tagBullet01Desc() : tagCGameObjectDesc() {}
	}BULLET01_EFFECT_DESC;
private:
	Bullet01Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Bullet01Effect(const Bullet01Effect& rhs);
	virtual ~Bullet01Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Bullet01Effect_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Bullet01Effect"; }
	static Bullet01Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

