
#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "HP.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
class Observer;
END

BEGIN(Client)
class DynamicImage;
class Image;
class FontUI;
END

BEGIN(Client)

class MonsterHP final : public CGameObject, public HP
{
public:
	typedef struct tagMonsterHPDesc : public tagCGameObjectDesc
	{
		tagMonsterHPDesc() = default;
		_float2 fSize;
		_float fMaxHP = 1000.f;
		_float2 vOffset = _float2(0.f, 75.f);
	};

protected:
	MonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	MonsterHP(const MonsterHP& rhs);
	virtual ~MonsterHP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;

public:
	virtual void TakeDamage(_uint iAmount) override;
	virtual void Heal(_uint iAmount) override;
	
	_bool isZeroHP() { return m_pHealthCom->isZeroHP(); };
	_float HPPercent() { return m_pHealthCom->HPPercent(); }
	Observer* GetObserver() { return m_pObserver; }

	void Reset();
	void FullHp(); 
	void Enable();
	void Disable();

private:
	void SetEnable(_bool bEnable) { m_bEnable = bEnable; }

private:
	DynamicImage* m_pDynamicImage = { nullptr };
	Image* m_pImage = { nullptr };
	Health* m_pHealthCom = { nullptr };
	Observer* m_pObserver = { nullptr };
	FontUI* m_pFontUI = { nullptr };
	TimeCounter* m_pTimeCounter = { nullptr };

	CTransform* m_pOwnerTransform = { nullptr };
private:
	void SetAmount(const _uint& iAmount);

private:
	_bool m_bEnable = { true };
	_uint m_iAmount = { 0 };
	_float2 m_vOffset;
public:
	HRESULT Add_Components(_float2 fSize);

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_MonsterHP"; }
	/* 원형을 생성한다. */
	static MonsterHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END