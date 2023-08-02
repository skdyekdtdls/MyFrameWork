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
	// ��ȸ�ϸ鼭 �̺�Ʈ �߻�.
	for (auto Pair : m_SubscribersFunc)
		Pair.second();

	// ��������(UnSubscribeDelay) 
	if (false == m_ToBeRemoved.empty()) // ������ ����� �ִ��� üũ
	{
		for (auto iter = m_SubscribersFunc.begin(); iter != m_SubscribersFunc.end();)
		{
			// ������ �ؽ��ʰ� ��ġ�ϴ� ��ū�� ��ȸ�ϸ鼭 ã��.
			if (m_ToBeRemoved.end() != find_if(m_ToBeRemoved.begin(), m_ToBeRemoved.end(), CTag_Finder(iter->first)))
			{
				// �����߿�!! Unsubscribe���� remove�� ���� ����Ǿ����!!
				m_ToBeRemoved.erase(iter->first); // ������ ����Ʈ������ ����
				auto tmpIter = ++iter; // size_t�� ��ȯ�ؼ� �̷��� �������� �ӽ�������.
				--iter; // �ٽ� �������� ���ƿ�.
				m_SubscribersFunc.erase(iter->first);
				iter = tmpIter;
			}
			else
				++iter; // �ƴ϶�� ����
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