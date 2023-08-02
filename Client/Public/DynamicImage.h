
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

	void SetRatio(_float fRatio);
	void SetRatioLerp(_float fRatio);
	void SetPass(_uint iPass) { m_iPass = iPass; }
	void ImageDepth(_float Depth);
	void SetPosition(_float2 vPos);

	void SetEnable(_bool bEnable) { m_bEnable = bEnable; }
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform2D* m_pTransformCom = { nullptr };
	CVIBuffer_DynamicRect* m_pVIBufferCom = { nullptr };

private:
	_float m_TimeAcc = 0.f;
	_float m_fRatio;
	_float m_fTargetRatio;
	_bool m_bLerp = false;
	_bool m_bEnable = { true };

	VTXPOSTEX* m_pVertices;

private:
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_uint					m_iPass = { 1 };

public:
	//TEXT("Prototype_Component_Texture_xx")�� �����ϸ�ȴ�.
	HRESULT Add_Components(const _tchar* pTextureProtoTag);
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_DynamicImage"; }
	/* ������ �����Ѵ�. */
	static DynamicImage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻(���� ����� ��ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END