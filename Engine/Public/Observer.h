
#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Observer final : public CComponent
{
public:
	using Functor = function<void()>;

public:
	typedef struct tagObserverDesc : public tagComponentDesc
	{
		tagObserverDesc() : tagComponentDesc() {};
		_int iMaxHp;
	}Observer_DESC;

private:
	explicit Observer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit Observer(const Observer& rhs);
	virtual ~Observer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;

public:
	// �̺�Ʈ ��� 
	void Subscribe(const _tchar * pTag, function<void()> CallBack)
	{
		m_SubscribersFunc.emplace(pTag, CallBack);
	}

	// �̺�Ʈ ����
	void UnSubscribe(const _tchar * pTag) {
		m_SubscribersFunc.erase(pTag);
	}

	// for�� ��ȸ���߿� �������� �ȵǴϱ� ����ó���ؼ� ��� ����
	void UnSubscribeDelay(const _tchar * pTag)
	{
		Functor Func = Find_Func(pTag);

		m_ToBeRemoved.emplace(pTag, Func);
	}

	void ResetPool();

private:
	unordered_map<const _tchar*, Functor> m_SubscribersFunc;
	unordered_map<const _tchar*, Functor> m_ToBeRemoved;


private:
	void Notify();
	Functor Find_Func(const _tchar * pTag);

public:
	// If this component is the VIBuffer Com or the Shader Com, Delete ProtoTag().
	static const _tchar* ProtoTag() { return L"Prototype_Component_Observer"; }
	static Observer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
