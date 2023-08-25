
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
class FontUI final : public CGameObject
{
public:
	typedef struct tagFontUIDesc : public tagCGameObjectDesc
	{
		tagFontUIDesc() : tagCGameObjectDesc() {}
		_float fScale;
	};
private:
	FontUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	FontUI(const FontUI& rhs);
	virtual ~FontUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void SetPosition(_float2 vPos);

	void Go_Right(const _double& TimeDelta);
	void Go_Left(const _double& TimeDelta);
	void Go_Up(const _double& TimeDelta);
	void Go_Down(const _double& TimeDelta);
	void Rotation(_float fDegree);
	void TurnCCW(const _double& TimeDelta);
	void TurnCW(const _double& TimeDelta);

	void SetEnable(_bool bEnable) { m_bEnable = bEnable; }
	void SetStyle(const _tchar* FontTag) { m_Style = FontTag; }
	void SetColor(_fvector vColor) {
		XMStoreFloat4(&m_vColor, vColor);
	}
	void Font(const _tchar* pFont) { m_Font = pFont; }

	_bool isInScreen();

private: /* For. Component */
	CRenderer* m_pRendererCom = { nullptr };
	CTransform2D* m_pTransformCom = { nullptr };

private:
	_bool m_bEnable = { true };
	wstring m_Style = L"Font_135";
	wstring m_Font = L"Hello World!";
	_float4 m_vColor = _float4(1.f, 1.f, 1.f, 1.f);
	_float2 m_vOrigin = _float2(0.f, 0.f);
	_float m_fScale;
	CRenderer::RENDERGROUP m_eRenderGroup = { CRenderer::RENDER_FONT };
private:
	HRESULT Add_Components();

private:
	static _uint FontUI_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_FontUI"; }
	static FontUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END