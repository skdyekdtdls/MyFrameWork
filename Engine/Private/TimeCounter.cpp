#include "TimeCounter.h"
#include "Timer_Manager.h"
TimeCounter::TimeCounter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

TimeCounter::TimeCounter(const TimeCounter& rhs)
	: CComponent(rhs)
	, m_bEnable(false)
	, m_bEqualResult(false)
{
	for (auto& iter : m_TimeAcc)
		iter = 0.0;
}

HRESULT TimeCounter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT TimeCounter::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void TimeCounter::Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	m_TimeAcc[NORMAL] += TimeDelta;
	m_TimeAcc[EVERY] += TimeDelta;
}

void TimeCounter::Reset()
{
	for (auto& iter : m_TimeAcc)
		iter = 0.0;

	m_bEqualResult = false;
}

// ���� �Ȱ����� true, epsilon�� ��������(TimeDelta�ִ°� ��õ)
_bool TimeCounter::isEuqalWith(_double Time)
{
	if (false == m_bEqualResult && isGreaterThan(Time))
	{
		m_bEqualResult = true;
		return true;
	}
	return false;
}

// �����ð� > Time = True
_bool TimeCounter::isGreaterThan(_double Time)
{
	return Time < m_TimeAcc[NORMAL];
}

// �����ð� < Time = True
_bool TimeCounter::isLessThan(_double Time)
{
	return !isGreaterThan(Time);
}

// �� Time���� true�� ��ȯ
_bool TimeCounter::EveryPer(_double Time)
{
	if (m_TimeAcc[EVERY] >= Time)
	{
		m_TimeAcc[EVERY] = 0.0;
		return true;
	}
	return false;
}

TimeCounter* TimeCounter::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	TimeCounter* pInstance = new TimeCounter(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created TimeCounter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* TimeCounter::Clone(void* pArg)
{
	TimeCounter* pInstance = new TimeCounter(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned TimeCounter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void TimeCounter::Free()
{
	__super::Free();
}

