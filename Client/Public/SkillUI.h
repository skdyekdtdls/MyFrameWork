
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

	_float GetCoolTime() { return m_fCoolTime; }
	void ImageDepth(_float Depth);
	void CoolTime();

public:
	void SetMaxCoolTime(_float fMaxCoolTime) { m_fMaxCoolTime = fMaxCoolTime; }
	void SetCoolTime(_float fCoolTime) { m_fCoolTime = fCoolTime; }

private: /* For. Component */
	Image* m_pImage = { nullptr };
	FontUI* m_pFontUI = { nullptr };

	_float m_fMaxCoolTime = { 10.f };
	_float m_fCoolTime = { 0.f };

private:
	HRESULT Add_Components(const tagSkillUIDesc& SkillDesc);

private:
	static _uint SkillUI_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_SkillUI"; }
	static SkillUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

