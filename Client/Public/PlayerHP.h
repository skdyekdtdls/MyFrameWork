
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

class PlayerHP final : public CGameObject
{
public:
	typedef struct tagPlayerHPDesc
	{
		tagPlayerHPDesc() = default;
		_float2 Size;
		_float2 OffsetPos;
	};
protected:
	PlayerHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	PlayerHP(const PlayerHP& rhs);
	virtual ~PlayerHP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void SetRatio(_float fRatio) { m_fRatio = fRatio; }
	
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	
private:
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_float4					m_OffsetPos;
	_float					m_fRatio = { 1.f };
public:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_PlayerHP"; }
	/* 원형을 생성한다. */
	static PlayerHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END