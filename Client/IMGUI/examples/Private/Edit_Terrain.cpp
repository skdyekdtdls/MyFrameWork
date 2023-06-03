#include "Edit_Terrain.h"
#include "ToolInstance.h"
/* Don't Forget Release for the VIBuffer Component*/

CEdit_Terrain::CEdit_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject(pDevice, pContext)
{
}

CEdit_Terrain::CEdit_Terrain(const CEdit_Terrain& rhs)
    : CGameObject(rhs)
{
}

HRESULT CEdit_Terrain::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}


HRESULT CEdit_Terrain::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CEdit_Terrain::Tick(_double TimeDelta)
{
    __super::Tick(TimeDelta);
}

void CEdit_Terrain::Late_Tick(_double TimeDelta)
{
    __super::Late_Tick(TimeDelta);

    if(nullptr != m_pRendererCom)
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CEdit_Terrain::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    if (FAILED(SetUp_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();
}

HRESULT CEdit_Terrain::Add_Components()
{


    return S_OK;
}

HRESULT CEdit_Terrain::SetUp_ShaderResources()
{
    _float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix)))
        return E_FAIL;

    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix",
        &MyMatrix)))
        return E_FAIL;

    MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix",
        &MyMatrix)))
        return E_FAIL;

    _float4 MyFloat4 = pGameInstance->Get_CamPosition();
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition",
        &MyFloat4, sizeof(_float4))))
        return E_FAIL;

    Safe_Release(pGameInstance);

    if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
        return E_FAIL;

    return S_OK;
}

CEdit_Terrain* CEdit_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEdit_Terrain* pInstance = new CEdit_Terrain(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created CEdit_Terrain");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CEdit_Terrain::Clone(void* pArg)
{
    CEdit_Terrain* pInstance = new CEdit_Terrain(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned CEdit_Terrain");
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CEdit_Terrain::Free(void)
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pRendererCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    /* Don't Forget Release for the VIBuffer Component*/
}

