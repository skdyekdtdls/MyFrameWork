
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
class FontUI;
class Image;
class SkillUI final : public CGameObject
{
public:
	typedef struct tagSkillUIDesc : public tagCGameObjectDesc
	{
		tagSkillUIDesc() : tagCGameObjectDesc() {}
		const _tchar* SkillTextureTag;
		_float2 fSize;
		_float fMaxCoolTime = { 10.f };
	};

private:
	SkillUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	SkillUI(const SkillUI& rhs);
	virtual ~SkillUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

public:
	_float GetCoolTime() { return m_pTimeCounter->TimeElapse(); }
	void ImageDepth(_float Depth);
	void UseSkill();

public:
	void SetMaxCoolTime(_float fMaxCoolTime) { m_fMaxCoolTime = fMaxCoolTime; }

private: /* For. Component */
	Image* m_pImage = { nullptr };
	FontUI* m_pFontUI = { nullptr };
	TimeCounter* m_pTimeCounter = { nullptr };

	_float m_fMaxCoolTime = { 10.f };
	_bool m_bIsRunning = { false };

private:
	HRESULT Add_Components(const tagSkillUIDesc& SkillDesc);
	void CoolTime();

private:
	static _uint SkillUI_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_SkillUI"; }
	static SkillUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END