
#include "Health.h"

Health::Health(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

Health::Health(const Health& rhs)
	: CComponent(rhs)
	, m_iMaxHp(0)
	, m_iCurrentHp(0)
	, m_SubscribersFunc(rhs.m_SubscribersFunc)
	, m_ToBeRemoved(rhs.m_ToBeRemoved)
{

}

HRESULT Health::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Health::Initialize(void* pArg)
{
	if (__super::Initialize(pArg))
		return E_FAIL;
	
	NULL_CHECK_RETURN(pArg, E_FAIL);
	
	m_iMaxHp = static_cast<HEALTH_DESC*>(pArg)->iMaxHp;
	m_iCurrentHp = m_iMaxHp;
	return S_OK;
}

void Health::TakeDamage(_uint iAmount)
{
	Saturate(m_iCurrentHp -= iAmount, 0, m_iMaxHp);
	Notify();
}

void Health::Heal(_uint iAmount)
{
	Saturate(m_iCurrentHp += iAmount, 0, m_iMaxHp);
	Notify();
}

_bool Health::isZeroHP()
{
	return (0 == m_iCurrentHp);
}

_float Health::HPPercent()
{
	// 어차피 CurrentHp가 0이하로 내려갈 일은 없으므로 Saturate는 생략함.
	return m_iCurrentHp / static_cast<_float>(m_iMaxHp) * 100.f;
}

void Health::Notify()
{
	// 순회하면서 이벤트 발생.
	for (auto Pair : m_SubscribersFunc)
		Pair.second();

	// 지연삭제(UnSubscribeDelay) 
	if (false == m_ToBeRemoved.empty()) // 지워질 목록이 있는지 체크
	{
		for (auto iter = m_SubscribersFunc.begin(); iter != m_SubscribersFunc.end();)
		{
			// 지워질 해쉬맵과 일치하는 토큰을 순회하면서 찾음.
			if (m_ToBeRemoved.end() != find_if(m_ToBeRemoved.begin(), m_ToBeRemoved.end(), CTag_Finder(iter->first)))
			{
				// 순서중요!! Unsubscribe보다 remove가 먼저 선행되어야함!!
				m_ToBeRemoved.erase(iter->first); // 지워질 리스트에서도 삭제
				auto tmpIter = ++iter; // size_t를 반환해서 이렇게 다음꺼를 임시저장함.
				--iter; // 다시 이전꺼로 돌아옴.
				m_SubscribersFunc.erase(iter->first);
				iter = tmpIter;
				
			}
			else
				++iter; // 아니라면 증가
		}
	}
}

Health::Functor Health::Find_Func(const _tchar* pTag)
{
	//auto iter = m_SubscribersFunc.find(pTag);
	auto iter = find_if(m_SubscribersFunc.begin(), m_SubscribersFunc.end(), CTag_Finder(pTag));

	if (iter == m_SubscribersFunc.end())
		return nullptr;

	return iter->second;
}

Health* Health::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Health* pInstance = new Health(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Health");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* Health::Clone(void* pArg)
{
	Health* pInstance = new Health(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Health");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Health::Free()
{
	__super::Free();
}