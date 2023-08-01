
#include "PlayerLevel.h"

PlayerLevel::PlayerLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

PlayerLevel::PlayerLevel(const PlayerLevel& rhs)
	: CComponent(rhs)
{

}

HRESULT PlayerLevel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT PlayerLevel::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void PlayerLevel::LevelUp()
{
	++m_iLevel;
	m_fExp = { 0.f };
}


PlayerLevel* PlayerLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	PlayerLevel* pInstance = new PlayerLevel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created PlayerLevel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* PlayerLevel::Clone(void* pArg)
{
	PlayerLevel* pInstance = new PlayerLevel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned PlayerLevel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void PlayerLevel::Free()
{
	__super::Free();
}

