#include "EditObj_Mgr.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(CEditObj_Mgr);

HRESULT CEditObj_Mgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    m_pRenderer = CRenderer::Create(pDevice, pContext);

    return S_OK;
}

HRESULT CEditObj_Mgr::Add_EditObj(string tag, CEditObject* pEditObj)
{
    if (nullptr == pEditObj)
        return E_FAIL;

    while (Find_EditObject(tag))
    {
        ++m_iID;
    }

    if (0 != m_iID)
        tag = tag + to_string(m_iID);
    m_pEditObjects.emplace(tag, pEditObj);

    m_iID = 0;
    return S_OK;
}

void CEditObj_Mgr::Update(_double TimeDelta)
{
    NULL_CHECK(m_pDevice);

    for (auto& EditObject : m_pEditObjects)
        EditObject.second->Update(TimeDelta);
}

void CEditObj_Mgr::Late_Update(_double TimeDelta)
{
    for (auto& EditObject : m_pEditObjects)
        EditObject.second->Late_Update(TimeDelta);
}

void CEditObj_Mgr::Render()
{
    FAILED_CHECK(m_pRenderer->Draw_RenderGroup());
}

CEditObject* CEditObj_Mgr::Find_EditObject(string tag)
{
    auto iter = m_pEditObjects.find(tag);

    if (iter == m_pEditObjects.end())
        return nullptr;

    return iter->second;
}

void CEditObj_Mgr::Free(void)
{
    for (auto& EditObject : m_pEditObjects)
        Safe_Release(EditObject.second);
    m_pEditObjects.clear();
}
