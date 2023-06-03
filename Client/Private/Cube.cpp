#include "Cube.h"
#include "GameInstance.h"

/* Don't Forget Release for the VIBuffer Component*/

CCube::CCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CCube::CCube(const CCube& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCube::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CCube::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CCube::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CCube::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCube::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
}

HRESULT CCube::Add_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);

	CTransform::TRANSFORMDESC TransformDesc{ 7.0, XMConvertToRadians(90.f) };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_IMGUI, CVIBuffer_Cube::ProtoTag(), L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBufferCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxCol", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);

	return S_OK;
}

HRESULT CCube::SetUp_ShaderResources()
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

	_float4 MyFloat4 = { _float4(0.f, 1.f, 0.f, 0.f) };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color",
		&MyFloat4, sizeof(_float4))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CCube* CCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCube* pInstance = new CCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCube::Clone(void* pArg)
{
	CCube* pInstance = new CCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CCube");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCube::Free(void)
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	/* Don't Forget Release for the VIBuffer Component*/
}

