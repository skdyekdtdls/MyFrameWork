
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

public:
	HRESULT Add_Components();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_MonsterHP"; }
	/* ������ �����Ѵ�. */
	static MonsterHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻(���� ����� ��ü)�� �����Ѵ�. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END