
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
	// ������ CurrentHp�� 0���Ϸ� ������ ���� �����Ƿ� Saturate�� ������.
	return m_iCurrentHp / static_cast<_float>(m_iMaxHp) * 100.f;
}

void Health::Notify()
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