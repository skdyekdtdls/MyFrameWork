
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
END

BEGIN(Client)

class PlayerHP final : public CGameObject, public HP
{
public:
	typedef struct tagPlayerHPDesc : public tagCGameObjectDesc
	{
		tagPlayerHPDesc() = default;
	};

protected:
	PlayerHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	PlayerHP(const PlayerHP& rhs);
	virtual ~PlayerHP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;
	virtual void Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

	virtual void TakeDamage(_uint iAmount) override;
	virtual void Heal(_uint iAmount) override;

	void SetInvin(_bool Invin) {
		m_pHealthCom->m_bInvin = Invin;
	}

private:
	DynamicImage* m_pDynamicImage = { nullptr };
	Image* m_pImage = { nullptr };
	Health* m_pHealthCom = { nullptr };
	Observer* m_pObserver = { nullptr };

private:
	void SetAmount(const _uint& iAmount);

private:
	_uint m_iAmount = { 0 };


public:
	HRESULT Add_Components();
	HRESULT SetUp_ShaderResources();

public:
	static const _tchar* ProtoTag() { return L"Prototype_GameObject_PlayerHP"; }
	/* 원형을 생성한다. */
	static PlayerHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본(실제 사용할 객체)을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END