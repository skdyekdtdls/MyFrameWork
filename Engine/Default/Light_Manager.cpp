#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{

}

const CLight::LIGHTDESC* CLight_Manager::Get_Light(_uint iIndex)
{
    if (iIndex >= m_Lights.size())
        return nullptr;

    auto iter = m_Lights.begin();

    for (size_t i = 0; i < iIndex; ++i)
        ++iter;

    return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    return S_OK;
}

HRESULT CLight_Manager::Add_Lights(const CLight::LIGHTDESC& LightDesc)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);

    NULL_CHECK_RETURN(pLight, E_FAIL);

    m_Lights.push_back(pLight);

    return S_OK;
}

void CLight_Manager::Free(void)
{
    for (auto Light : m_Lights)
    {
        Safe_Release(Light);
    }
    m_Lights.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
