#include "Terrain.h"
#include "GameInstance.h"
CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}


HRESULT CTerrain::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CTerrain::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);



	m_pVIBufferCom->Render();
}

HRESULT CTerrain::Add_Components()
{	
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);

	CTransform::TRANSFORMDESC TransformDesc{7.0, XMConvertToRadians(90.f)};
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, CVIBuffer_Terrain::ProtoTag(), L"Com_VIBuffer_Terrain", (CComponent**)&m_pVIBufferCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Terrain", L"Com_Texture", (CComponent**)&m_pTextureCom), E_FAIL);

	return S_OK;
}

HRESULT CTerrain::SetUp_ShaderResources()
{
	_float4x4 tmp;
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	FAILED_CHECK_RETURN_CONSOLE_MSG(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture"), E_FAIL, "Error! CTerrain::SetUp_ShaderResources : " << __LINE__);

	tmp = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN_CONSOLE_MSG(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &tmp), E_FAIL, "Error! CTerrain::SetUp_ShaderResources : " << __LINE__);

	tmp = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN_CONSOLE_MSG(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &tmp), E_FAIL, "Error! CTerrain::SetUp_ShaderResources : " << __LINE__);

	tmp = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN_CONSOLE_MSG(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &tmp), E_FAIL, "Error! CTerrain::SetUp_ShaderResources : " << __LINE__);
	Safe_Release(pGameInstance);
	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CTerrain");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain::Free(void)
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
