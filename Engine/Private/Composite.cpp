#include "..\Public\Composite.h"
#include "GameInstance.h"

CComposite::CComposite(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CComposite::CComposite(const CComposite& rhs)
	: CComponent(rhs)
{

}

HRESULT CComposite::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CComposite::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	return S_OK;
}

void CComposite::Tick(_double TimeDelta)
{
}

void CComposite::Late_Tick(_double TimeDelta)
{
}

HRESULT CComposite::Render()
{
	return S_OK;
}

HRESULT CComposite::Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(pComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

CComponent* CComposite::Get_Component(const _tchar* pComponent)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTag_Finder(pComponent));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CComposite::RemoveObjectByCallBack(std::function<bool(const CComponent&)> predicate)
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ) {
		if (predicate(*(it->second))) {
			Safe_Release(it->second);
			it = m_Components.erase(it);
		}
		else {
			++it;
		}
	}
}

_bool CComposite::HasTransformCom()
{
	for (auto& Pair : m_Components)
	{
		if (dynamic_cast<CTransform*>(Pair.second))
			return true;
	}
	
	return false;
}

void CComposite::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();
}
