#include "..\Public\EditObject.h"


CEditObject::CEditObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComposite(pDevice, pContext)
{
}

HRESULT CEditObject::Initialize(void* pArg)
{
    FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

    return S_OK;
}

void CEditObject::Update(_double TimeDelta)
{
}

void CEditObject::Late_Update(_double TimeDelta)
{
}

HRESULT CEditObject::Render()
{
    FAILED_CHECK_RETURN(__super::Render(), E_FAIL);
    return S_OK;
}

void CEditObject::Free(void)
{
    __super::Free();
}
