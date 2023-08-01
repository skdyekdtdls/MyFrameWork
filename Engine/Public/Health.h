
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Health final : public CComponent, public IObserver
{
public:
	using Functor = function<void()>;

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
	void TakeDamage(_uint iAmount);
	void Heal(_uint iAmount);
	_bool isZeroHP();
	_float HPPercent();

public:
	// 이벤트 등록 
	void Subscribe(const _tchar* pTag, function<void()> CallBack)
	{
		m_SubscribersFunc.emplace(pTag, CallBack);
	}
	
	// 이벤트 해제
	void UnSubscribe(const _tchar* pTag) {
		m_SubscribersFunc.erase(pTag);  
	}

	// for문 순회도중에 지워지면 안되니까 지연처리해서 펑션 삭제
	void UnSubscribeDelay(const _tchar* pTag)
	{
		Functor Func = Find_Func(pTag);

		m_ToBeRemoved.emplace(pTag, Func);
	}

private:
	_int m_iMaxHp;
	_int m_iCurrentHp;
	
	unordered_map<const _tchar*, Functor> m_SubscribersFunc;
	unordered_map<const _tchar*, Functor> m_ToBeRemoved;
	//list<Functor*> m_SubscribersFunc;
	//list<Functor> m_ToBeRemoved;

private:
	void Notify();
	Functor Find_Func(const _tchar* pTag);

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
