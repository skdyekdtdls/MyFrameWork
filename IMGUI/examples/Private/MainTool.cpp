#include "..\Public\MainTool.h"
#include "Level_Tool.h"
#include "ToolInstance.h"
#include "ImWindow_Demo.h"

CMainTool::CMainTool()
{
    
}

HRESULT CMainTool::Initialize()
{
    CToolInstance* pToolInstance = CToolInstance::GetInstance();
    Safe_AddRef(pToolInstance);

    FAILED_CHECK_RETURN(pToolInstance->Initialize(), E_FAIL);

    Safe_Release(pToolInstance);

    return S_OK;
}

void CMainTool::Tick(_double TimeDelta)
{
    CToolInstance* pToolInstance = CToolInstance::GetInstance();
    Safe_AddRef(pToolInstance);

    pToolInstance->Tick(TimeDelta);

    Safe_Release(pToolInstance);

}

HRESULT CMainTool::Render()
{
    CToolInstance* pToolInstance = CToolInstance::GetInstance();
    Safe_AddRef(pToolInstance);

    FAILED_CHECK_RETURN(pToolInstance->Render(), E_FAIL);

    Safe_Release(pToolInstance);
    return S_OK;
}

CMainTool* CMainTool::Create()
{
    CMainTool* pInstance = New CMainTool;

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created CMainTool");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMainTool::Free()
{
    CToolInstance::Release_Tool();
}
