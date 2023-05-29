#include "..\Public\Level_Tool.h"

CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel(pDevice, pContext)
{
}

CLevel_Tool::CLevel_Tool(const CLevel_Tool& rhs)
    : CLevel(rhs)
{
}

HRESULT CLevel_Tool::Initialize()
{
    return S_OK;
}

void CLevel_Tool::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);
}

void CLevel_Tool::Late_Tick(_double TimeDelta)
{
    __super::Late_Tick(TimeDelta);
}

HRESULT CLevel_Tool::Render()
{
    return S_OK;
}

CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLevel_Tool* pInstance = New CLevel_Tool(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created CLevel_Tool");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CLevel_Tool::Free(void)
{
    __super::Free();
}
