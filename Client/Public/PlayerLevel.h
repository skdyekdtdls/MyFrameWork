
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
class DynamicImage;
class FontUI;
class PlayerLevel final : public CGameObject
{
public:
	typedef struct tagPlayerLevelDesc : public tagCGameObjectDesc
	{
		tagPlayerLevelDesc() : tagCGameObjectDesc() {}
		_float fScale;
	};
private:
	PlayerLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	PlayerLevel(const PlayerLevel& rhs);
	virtual ~PlayerLevel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

	void AddExp(_float fExp);

private: /* For. Component */
	DynamicImage* m_pDynamicImage = { nullptr };
	CRenderer* m_pRendererCom = { nullptr };
	FontUI* m_pFontUI = { nullptr };

private:
	_bool m_bNoLevelUp = { false };
	_uint m_iLevel = { 1 };
	_float m_fExp = { 0.f };
	_float m_fMaxExp = { 100.f };
	
private:
	void LevelUp();
	HRESULT Add_Components(_float4 vPosition, _float fScale);

private:
	static _uint PlayerLevel_Id;

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_PlayerLevel"; }
	static PlayerLevel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free(void) override;
};
END

