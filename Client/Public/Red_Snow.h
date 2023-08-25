#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect_Instance;
END

BEGIN(Client)

class CRed_Snow final : public CGameObject
{
protected:
	CRed_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRed_Snow(const CRed_Snow& rhs);
	virtual ~CRed_Snow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect_Instance* m_pVIBufferCom = { nullptr };

public:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	/* 원형을 생성한다. */
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_CRed_Snow"; }
	static CRed_Snow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END