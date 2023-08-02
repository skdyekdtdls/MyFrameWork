#include "..\Public\Observer.h"


HRESULT Observer::Initialize_Prototype()
{
	return S_OK;
}

void Observer::ResetPool()
{
	m_SubscribersFunc.clear();
	m_ToBeRemoved.clear();
}

void Observer::Notify()
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

Observer::Functor Observer::Find_Func(const _tchar* pTag)
{
	auto iter = find_if(m_SubscribersFunc.begin(), m_SubscribersFunc.end(), CTag_Finder(pTag));

	if (iter == m_SubscribersFunc.end())
		return nullptr;

	return iter->second;
}

Observer* Observer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Observer* pInstance = new Observer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Observer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* Observer::Clone(void* pArg)
{
	Observer* pInstance = new Observer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Observer");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Observer::Free()
{
	__super::Free();
	m_SubscribersFunc.clear();
	m_ToBeRemoved.clear();
}