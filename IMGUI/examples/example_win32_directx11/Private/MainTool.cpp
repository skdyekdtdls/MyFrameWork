#include "MainTool.h"

CMainTool::CMainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice(pDevice)
    , m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

void CMainTool::Tick(_double TimeDelta)
{
    Update(TimeDelta);
    LateUpdate(TimeDelta);
    Render();
}

HRESULT CMainTool::Initialize()
{
    return S_OK;
}

void CMainTool::Update(_double TimeDelta)
{
}

void CMainTool::LateUpdate(_double TimeDelta)
{
}

void CMainTool::Render()
{
}

CMainTool* CMainTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMainTool* pInstance = New CMainTool(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("MainTool Created Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMainTool::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
