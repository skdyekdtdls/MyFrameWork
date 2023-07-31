#include "P1Attack03.h"
#include "GameInstance.h"

_uint P1Attack03::P1Attack03_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

P1Attack03::P1Attack03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

P1Attack03::P1Attack03(const P1Attack03& rhs)
	: Bullet(rhs)
{
}

HRESULT P1Attack03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT P1Attack03::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++P1Attack03_Id;
	m_tInfo.wstrName = TO_WSTR("P1Attack03" + to_string(P1Attack03_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = P1Attack03_Id;

	tagP1Attack03Desc tP1Attack03Desc;
	if (nullptr != pArg)
		tP1Attack03Desc = *(tagP1Attack03Desc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tP1Attack03Desc.vPosition));

	m_pTimeCounterCom->Enable();

	return S_OK;
}

void P1Attack03::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTimeCounterCom->Tick(TimeDelta);

	// 수명
	__super::SetDead();

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
	}

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void P1Attack03::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);
#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
	Safe_Release(pGameInstance);
}

HRESULT P1Attack03::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (size_t i = 0; i < iNumMeshes; i++)
	//{
	//	m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

	//	m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TextureType_DIFFUSE);
	//	

	//	m_pShaderCom->Begin(0);

	//	m_pModelCom->Render(i);
	//}

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);

#ifdef _DEBUG

#endif
}

void P1Attack03::ResetPool(void* pArg)
{
	m_bDead = false;
	_vector vPosition = XMLoadFloat4(&((tagP1Attack03Desc*)pArg)->vPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTimeCounterCom->Enable();
}

void P1Attack03::SetDead()
{
	__super::SetDead();
	ObjectPool<P1Attack03>::GetInstance()->PushPool(this);
	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
}

HRESULT P1Attack03::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 20.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh*/", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_VtxAnimMesh", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = { 0.3f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	TimeCounter::TIME_COUNTER_DESC tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounterCom, &tTimeCounterDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT P1Attack03::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	//FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	Safe_Release(pGameInstance);

	return S_OK;
}

void P1Attack03::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (0 == strcmp(tCollisionInfo.OtherGameObejctName.c_str(), "Clint1"))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

P1Attack03* P1Attack03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	P1Attack03* pInstance = new P1Attack03(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created P1Attack03");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* P1Attack03::Clone(void* pArg)
{
	P1Attack03* pInstance = new P1Attack03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned P1Attack03");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void P1Attack03::Free(void)
{
	__super::Free();

	--P1Attack03_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTimeCounterCom);
}
