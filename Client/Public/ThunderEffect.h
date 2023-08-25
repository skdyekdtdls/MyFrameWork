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
class ThunderEffect final : public CGameObject
{
public:
	typedef struct tagThunderEffectDesc : public tagCGameObjectDesc
	{
		tagThunderEffectDesc() : tagCGameObjectDesc() {}
		_uint iRow = { 1 };
		_uint iCol = { 1 };
		const _tchar* pTextureTag;
		//_uint iNumInstance;
	}ThunderEffect_DESC;

private:
	ThunderEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ThunderEffect(const ThunderEffect& rhs);
	virtual ~ThunderEffect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta, _fvector vPos);
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Reset();
	void Disable();

	void Scale(_float2 vScale) {
		m_fSizeX = vScale.x;
		m_fSizeY = vScale.y;
	}
	void SetAnimTime(_double dAnimSpeed) {
		m_AnimSpeed = dAnimSpeed;
	}
	void SetPass(_uint iPass) { m_iPass = iPass; }

private:
	ThunderEffect_DESC  m_tEffectDesc;
	_uint			m_iPass = { 1 };
	_uint			m_iStartIndex = 0;
	_uint			m_iLastIndex = 0;
	_uint			m_iMaxIndex;
	_bool			m_bEnable = { false };
	_int			m_iCurrentIndex = { -1 };
	_float			m_fSizeX = 0.5f;
	_float			m_fSizeY = 0.5f;
	_double			m_TimeAcc = { 0.0 };
	_double			m_AnimSpeed = { 0.1 };

private: /* For. Component */
	CTransform* m_pTransformCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };
	// Can declare VIBuffer or Model Com

private:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

private:
	static _uint ThunderEffect_ID;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_ThunderEffect"; }
	static ThunderEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

