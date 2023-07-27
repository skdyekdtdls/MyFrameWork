
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Health final : public CComponent
{
public:
	using HealthToken = list<function<void(int)>>::iterator;

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

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	void TakeDamage(int iAmount);
	void Heal(int iAmount);
	_bool isZeroHP();

public:
	HealthToken Subscribe(function<void(int)> CallBack)
	{
		m_SubscribersFunc.push_back(CallBack);
		return --m_SubscribersFunc.end();
	}
	
	void UnSubscribe(HealthToken Token)
	{
		m_SubscribersFunc.erase(Token);
	}

private:
	_int m_iMaxHp;
	_int m_iCurrentHp;
	list<function<void(int)> > m_SubscribersFunc;

private:
	void Notify();

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