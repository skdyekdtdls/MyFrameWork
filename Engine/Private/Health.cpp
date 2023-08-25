
#include "Health.h"

Health::Health(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

Health::Health(const Health& rhs)
	: CComponent(rhs)
	, m_iMaxHp(0)
	, m_iCurrentHp(0)
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
	if (true == m_bInvin)
		return;
	Saturate(m_iCurrentHp -= iAmount, 0, m_iMaxHp);
}

void Health::Heal(_uint iAmount)
{
	Saturate(m_iCurrentHp += iAmount, 0, m_iMaxHp);
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