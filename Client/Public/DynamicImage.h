
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class DynamicImage final : public CGameObject
{
public:
	typedef struct tagDynamicImageDesc
	{
		tagDynamicImageDesc() = default;
		const _tchar* pTextureProtoTag;
		_float2 Size;
		_float2 Pos;
		IObserver* pObserver;
	};
protected:
	DynamicImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	DynamicImage(const DynamicImage& rhs);
	virtual ~DynamicImage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void SetRatio(_float fRatio) { m_fRatio = fRatio; }
	void SetPass(_uint iPass) { m_iPass = iPass; }
	
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_DynamicRect* m_pVIBufferCom = { nullptr };

private:
	IObserver* m_pObserver = { nullptr };

private:
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_float					m_fRatio = { 1.f };
	_uint					m_iPass = { 1 };
public:
	//TEXT("Prototype_Component_Texture_xx")를 전달하면된다.
	HRESULT Add_Components(const _tchar* pTextureProtoTag);
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_DynamicImage"; }
	/* 원형을 생성한다. */
	static DynamicImage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END