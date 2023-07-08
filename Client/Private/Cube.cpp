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

	CGAMEOBJECT_DESC tCloneDesc;
	ZeroStruct(tCloneDesc);

	if (nullptr != pArg)
	{
		tCloneDesc = *static_cast<CGAMEOBJECT_DESC*>(pArg);
		
		_vector		vPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		vPosition += XMLoadFloat4(&tCloneDesc.vPosition);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
		
	}
	
	return S_OK;
}

void CCube::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	//m_pTransformCom->Go_Straight(TimeDelta);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if(pGameInstance->Get_DIKeyState(VK_SPACE))
		

	Safe_Release(pGameInstance);	
}

void CCube::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	_float4 tmp;
	XMStoreFloat4(&tmp, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
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
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL)

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(90.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CVIBuffer_Cube::CVIBUFFER_CUBE_DESC tVibufferCubeDesc; tVibufferCubeDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_IMGUI, CVIBuffer_Cube::ProtoTag(), L"Com_VIBuffer_Cube", (CComponent**)&m_pVIBufferCom, &tVibufferCubeDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxCol", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);


	return S_OK;
}

HRESULT CCube::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	_float4 MyFloat4 = { _float4(0.f, 1.f, 0.f, 0.f) };
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_Color", &MyFloat4, sizeof(_float4)), E_FAIL);

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
}

