
#include "ForceField.h"
#include "GameInstance.h"
#include "Dissolve.h"
#include "PlayerHP.h"
_uint ForceField::ForceField_Id = 0;

ForceField::ForceField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

ForceField::ForceField(const ForceField& rhs)
	: CGameObject(rhs)
	, m_bEnable(rhs.m_bEnable)
{
}

HRESULT ForceField::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT ForceField::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++ForceField_Id;
	m_tInfo.wstrName = TO_WSTR("ForceField" + to_string(ForceField_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = ForceField_Id;

	tagForceFieldDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagForceFieldDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));
	
	m_fRadius = tCloneDesc.fRadius;
	m_fOffsetY = m_fRadius * 0.5f;

	_float3 vScale = { m_fRadius , m_fRadius , m_fRadius };
	m_pTransformCom->Scaled(vScale);
	m_pOwnerTransform = m_pOwner->GetComponent<CTransform>();
	m_pDissolve->SetDissolveAmount(0.f);
	return S_OK;
}

void ForceField::Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;
	m_TimeDelta = TimeDelta;

	__super::Tick(TimeDelta);

	PlayerHP* HP = static_cast<PlayerHP*>( m_pOwner->Get_Component(L"Com_HP"));
	HP->SetInvin(true);

	m_dTimeAcc += TimeDelta;

	if (m_dTimeAcc >= m_dDuration)
	{
		SetEnable(false);
		HP->SetInvin(false);
	}

	if (m_dTimeAcc >= m_dDuration - 1.f)
	{
		m_bDelete = true;
	}

	_vector vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	vPos.m128_f32[1] += m_fOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void ForceField::Late_Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_BLEND, this);

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
}

HRESULT ForceField::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();
}

void ForceField::Use()
{
	m_dTimeAcc = 0.0;
	SetEnable(true);
	m_bStart = true;
	m_bDelete = false;
	m_pDissolve->SetDissolveAmount(0.f);
}

HRESULT ForceField::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxNorTex", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = m_fRadius;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_Coll", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	CTexture::tagCTextureDesc tTextureDesc;
	tTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Texture_Hex", L"Com_Texture", (CComponent**)&m_pTextureCom, &tTextureDesc), E_FAIL);

	CVIBuffer_Sphere::tagCVIBuffer_SphereDesc tBufferDesc;
	tBufferDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CVIBuffer_Sphere::ProtoTag(), L"Com_Buffer", (CComponent**)&m_pVIBufferCom, &tBufferDesc), E_FAIL);

	Dissolve::tagDissolveDesc tDissolveDesc;
	tDissolveDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Composite(Dissolve::ProtoTag(), L"Com_Dissolve", (CComponent**)&m_pDissolve, &tDissolveDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT ForceField::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	if (true == m_bStart)
	{
		if(0.99f <= m_pDissolve->GetDissolveAmount())
			m_bStart = false;

		FAILED_CHECK_RETURN(m_pDissolve->Bind_Values(m_pShaderCom, 0.5f * m_TimeDelta), E_FAIL);
	}

	if (true == m_bDelete)
	{
		if (-0.01f >= m_pDissolve->GetDissolveAmount())
			m_bDelete = false;

		FAILED_CHECK_RETURN(m_pDissolve->Bind_Values(m_pShaderCom, -1.0f * m_TimeDelta), E_FAIL);
	}
	
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HexTexture"), E_FAIL);
	_float fTimeAcc = m_dTimeAcc * 0.05f;

	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_fProgress", &fTimeAcc ,sizeof(_float)), E_FAIL);
	Safe_Release(pGameInstance);

	return S_OK;
}

ForceField* ForceField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ForceField* pInstance = new ForceField(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ForceField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* ForceField::Clone(void* pArg)
{
	ForceField* pInstance = new ForceField(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ForceField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ForceField::Free(void)
{
	__super::Free();

	--ForceField_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDissolve);
}

