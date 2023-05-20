#include "Timer_Manager.h"
#include "Timer.h"
IMPLEMENT_SINGLETON(CTimer_Manager)


CTimer_Manager::CTimer_Manager()
{

}

HRESULT CTimer_Manager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (pTimer != nullptr)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (pTimer == nullptr)
		return E_FAIL;

	m_pTimers.emplace(pTimerTag, pTimer);

	return S_OK;
}

_double CTimer_Manager::Get_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return 0.0;

	return pTimer->Get_TimeDelta();
}

void CTimer_Manager::Set_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

CTimer* CTimer_Manager::Find_Timer(const _tchar* pTimerTag)
{
	auto iter = find_if(m_pTimers.begin(), m_pTimers.end(), CTag_Finder(pTimerTag));

	if (iter == m_pTimers.end())
		return nullptr;

	return iter->second;
}

void CTimer_Manager::Free(void)
{
	for (auto& Pair : m_pTimers)
	{
		Safe_Release(Pair.second);
	}
	m_pTimers.clear();
}
