#include "CannonSpiderBullet.h"
#include "GameInstance.h"
#include "EnergyBallEffect2.h"
_uint CannonSpiderBullet::CannonSpiderBullet_Id = 0;

CannonSpiderBullet::CannonSpiderBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

CannonSpiderBullet::CannonSpiderBullet(const CannonSpiderBullet& rhs)
	: Bullet(rhs)
{
}

HRESULT CannonSpiderBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CannonSpiderBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++CannonSpiderBullet_Id;
	m_tInfo.wstrName = TO_WSTR("CannonSpiderBullet" + to_string(CannonSpiderBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CannonSpiderBullet_Id;

	tagCannonSpiderBulletDesc tCannonSpiderBulletDesc;
	if (nullptr != pArg)
		tCannonSpiderBulletDesc = *(tagCannonSpiderBulletDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCannonSpiderBulletDesc.vPosition));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, tCannonSpiderBulletDesc.vLook);

	m_pTimeCounterCom->Disable();

	m_fDamage = 3.f;

	return S_OK;
}

void CannonSpiderBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTimeCounterCom->Tick(TimeDelta);

	if (m_pTimeCounterCom->isGreaterThan(2.0))
	{
		SetDead();
	}

	m_pTransformCom->Go_Straight(TimeDelta);

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
	}

	if (nullptr != m_pEnergyBallEffect2Com)
		m_pEnergyBallEffect2Com->Tick(TimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

}

void CannonSpiderBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);

	if (nullptr != m_pEnergyBallEffect2Com)
		m_pEnergyBallEffect2Com->Late_Tick(TimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif

	Safe_Release(pGameInstance);
}

HRESULT CannonSpiderBullet::Render()
{

#ifdef _DEBUG

#endif
	return S_OK;
}

void CannonSpiderBullet::ResetPool(void* pArg)
{
	m_bDead = false;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, ((tagCannonSpiderBulletDesc*)(pArg))->vLook);

	_vector vPos = XMLoadFloat4(&((tagCannonSpiderBulletDesc*)pArg)->vPosition);
	vPos.m128_f32[1] += 0.5f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	m_pTimeCounterCom->Enable();
}

void CannonSpiderBullet::SetDead()
{
	__super::SetDead();
	ObjectPool<CannonSpiderBullet>::GetInstance()->PushPool(this);
	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
}

HRESULT CannonSpiderBullet::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 10.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh*/", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_VtxAnimMesh", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = { 0.5f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	TimeCounter::TIME_COUNTER_DESC tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounterCom, &tTimeCounterDesc), E_FAIL);

	EnergyBallEffect2::tagEnergyBallEffectDesc tEffectDesc;
	tEffectDesc.iRow = 3;
	tEffectDesc.iCol = 3;
	tEffectDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Composite(EnergyBallEffect2::ProtoTag(), L"Com_Effect", (CComponent**)&m_pEnergyBallEffect2Com, &tEffectDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CannonSpiderBullet::SetUp_ShaderResources()
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

void CannonSpiderBullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (0 == strcmp(tCollisionInfo.OtherGameObejctName.c_str(), "Clint1"))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

CannonSpiderBullet* CannonSpiderBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CannonSpiderBullet* pInstance = new CannonSpiderBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CannonSpiderBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* CannonSpiderBullet::Clone(void* pArg)
{
	CannonSpiderBullet* pInstance = new CannonSpiderBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CannonSpiderBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CannonSpiderBullet::Free(void)
{
	__super::Free();

	--CannonSpiderBullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTimeCounterCom);
	Safe_Release(m_pEnergyBallEffect2Com);
}