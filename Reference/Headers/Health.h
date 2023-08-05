
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Health final : public CComponent
{
public:
	typedef struct tagHealthDesc : public tagComponentDesc
	{
		tagHealthDesc() : tagComponentDesc() {};
		_int iMaxHp;
	}HEALTH_DESC;

private:
	explicit Health(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	explicit Health(const Health& rhs);
	virtual ~Health() = default;

public:
	_int GetCurrentHp() const {
		return m_iCurrentHp;
	}

	_int GetMaxHp() const {
		return m_iMaxHp;
	}

	void SetMaxHP(_uint iMaxHp) { m_iMaxHp = iMaxHp; }
	void SetCurrentHP(_uint iCurrentHP) { m_iCurrentHp = iCurrentHP; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void TakeDamage(_uint iAmount);
	void Heal(_uint iAmount);
	_bool isZeroHP();
	_float HPPercent();

private:
	_int m_iMaxHp;
	_int m_iCurrentHp;
	
public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_Health"; }
	static Health* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

#ifdef _DEBUG
	void CoutHp() { cout << "CurrentHp : " << m_iCurrentHp << endl; }
#endif
};

END
