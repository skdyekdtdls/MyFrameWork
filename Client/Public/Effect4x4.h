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
class Effect4x4 final : public CGameObject
{
public:
	typedef struct tagEffect4x4Desc : public tagCGameObjectDesc
	{
		tagEffect4x4Desc() : tagCGameObjectDesc() {}
		const _tchar* pTextureTag;
		//_uint iNumInstance;
	}EFFECT4X4_DESC;
private:
	Effect4x4(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Effect4x4(const Effect4x4& rhs);
	virtual ~Effect4x4() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta, _fvector vPos);
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Reset();
	void Disable();

private:
	EFFECT4X4_DESC m_tEffectDesc;
	_bool			m_bEnable = { false };
	_int			m_iTextureIndex = { -1 };
	_double			m_TimeAcc = { 0.0 };
private: /* For. Component */
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint Effect4x4_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_Effect4x4"; }
	static Effect4x4* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

