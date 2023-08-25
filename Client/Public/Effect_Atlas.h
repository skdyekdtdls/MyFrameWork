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
class Effect_Atlas final : public CGameObject
{
	enum PLAY_MODE { ONCE, LOOP, PLAY_MODE_END };

public:
	typedef struct tagEffect_AtlasDesc : public tagCGameObjectDesc
	{
		tagEffect_AtlasDesc() : tagCGameObjectDesc() {}
		_uint iRow = { 1 };
		_uint iCol = { 1 };
		const _tchar* pTextureTag;
		//_uint iNumInstance;
	}EFFECT_ATLAS_DESC;

private:
	Effect_Atlas(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Effect_Atlas(const Effect_Atlas& rhs);
	virtual ~Effect_Atlas() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta, _fvector vPos);
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	void Reset();
	void Disable();

	void Play_Once(_uint iStartIndex = 0, _uint iLastIndex = 0);
	void Play_Loop(_uint iStartIndex = 0, _uint iLastIndex = 0);
	void Scale(_float2 vScale) {
		m_fSizeX = vScale.x;
		m_fSizeY = vScale.y;
	}
	void SetAnimTime(_double dAnimSpeed) {
		m_AnimSpeed = dAnimSpeed;
	}
	void SetPass(_uint iPass) { m_iPass = iPass; }

private:
	void OnceMode(const _double& TimeDelta);
	void LoopMode(const _double& TimeDelta);
	
private:
	EFFECT_ATLAS_DESC  m_tEffectDesc;
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
	PLAY_MODE		m_ePlayMode = { LOOP };

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
	static Effect_Atlas* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

