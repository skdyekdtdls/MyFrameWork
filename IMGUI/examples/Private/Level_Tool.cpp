#include "..\Public\Level_Tool.h"
#include "ToolInstance.h"
#include "Edit_Terrain.h"
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
    FAILED_CHECK_RETURN(Ready_Prototype_Component(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Prototype_GameObject(L"Layer_GameObject"), E_FAIL);

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

HRESULT CLevel_Tool::Ready_Prototype_Component()
{
    CToolInstance* pToolIntance = CToolInstance::GetInstance();
    Safe_AddRef(pToolIntance);

    Safe_Release(pToolIntance);

    return S_OK;
}

HRESULT CLevel_Tool::Ready_Prototype_GameObject(const _tchar* pLayerTag)
{
    CToolInstance* pToolIntance = CToolInstance::GetInstance();
    Safe_AddRef(pToolIntance);

    /* For Prototype */
    FAILED_CHECK_RETURN(pToolIntance->Add_Prototype(CEdit_Terrain::ProtoTag(), CEdit_Terrain::Create(m_pDevice, m_pContext)), E_FAIL);

    /* For Clone */
    FAILED_CHECK_RETURN(pToolIntance->Add_GameObject(LEVEL_TOOL, CEdit_Terrain::ProtoTag(), pLayerTag), E_FAIL);

    Safe_Release(pToolIntance);

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
