#include "ToolInstance.h"

IMPLEMENT_SINGLETON(CEditObj_Mgr);

HRESULT CToolInstance::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return E_NOTIMPL;
}

void CToolInstance::Update(_double TimeDelta)
{
}

void CToolInstance::Late_Update(_double TimeDelta)
{
}

void CToolInstance::Render()
{
}

void CToolInstance::Free(void)
{
}
