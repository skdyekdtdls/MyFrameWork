
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

class BossHP final : public CGameObject, public HP
{
public:
	typedef struct tagBossHPDesc : public tagCGameObjectDesc
	{
		tagBossHPDesc() = default;
	};

protected:
	BossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	BossHP(const BossHP& rhs);
	virtual ~BossHP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	virtual void TakeDamage(_uint iAmount) override;
	virtual void Heal(_uint iAmount) override;
	_bool isZeroHP() { return m_pHealthCom->isZeroHP(); };
	_float HPPercent() { return m_pHealthCom->HPPercent(); }
	Observer* GetObserver() { return m_pObserver; }
	void SetEnable(_bool bEnable) { m_bEnable = bEnable; }

private:
	DynamicImage* m_pDynamicImage = { nullptr };
	Image* m_pImage = { nullptr };
	Health* m_pHealthCom = { nullptr };
	Observer* m_pObserver = { nullptr };
	FontUI* m_pFontUI = { nullptr };

private:
	void SetAmount(const _uint& iAmount);

private:
	_bool m_bEnable = { true };
	_uint m_iAmount = { 0 };

public:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_BossHP"; }
	/* 원형을 생성한다. */
	static BossHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END