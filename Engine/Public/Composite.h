#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CComposite : public CComponent
{
public:
	typedef struct tagCCompositeDesc : public tagComponentDesc
	{
		tagCCompositeDesc() : tagComponentDesc() {};
	}CCOMPOSITE_DESC;
protected:
	explicit CComposite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CComposite(const CComposite& rhs);
	virtual ~CComposite() = default;

public:
	HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta);
	virtual void Late_Tick(_double TimeDelta);
	virtual HRESULT Render();

	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, _Inout_ CComponent** ppOut, void* pArg);
	CComponent* Get_Component(const _tchar* pComponent);

	template <typename T>
	T* GetComponent();

	template <typename T>
	void GetComponents(vector<T*>& vecComponent);

	void RemoveObjectByCallBack(std::function<bool(const CComponent&)> predicate);
	_bool HasTransformCom();

private:
	unordered_map<const _tchar*, CComponent*>	m_Components;

public:
	virtual CComponent* Clone(void* pArg) override = 0;
	virtual void Free(void) override;
};
END

template<typename T>
inline T* CComposite::GetComponent()
{
	for (auto& Pair : m_Components)	{
		if (dynamic_cast<T*>(Pair.second)) {
			return (T*)Pair.second;
		}
	}

	return nullptr;
}

template<typename T>
inline void CComposite::GetComponents(vector<T*>& vecComponent)
{
	for (auto& Pair : m_Components) {
		if (dynamic_cast<T*>(Pair.second)) {
			vecComponent.push_back(Pair.second);
		}
	}
}

