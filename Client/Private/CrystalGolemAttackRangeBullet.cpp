#include "CrystalGolemAttackRangeBullet.h"
#include "GameInstance.h"

_uint CrystalGolemAttackRangeBullet::CrystalGolemAttackRangeBullet_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

CrystalGolemAttackRangeBullet::CrystalGolemAttackRangeBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

CrystalGolemAttackRangeBullet::CrystalGolemAttackRangeBullet(const CrystalGolemAttackRangeBullet& rhs)
	: Bullet(rhs)
{
}

HRESULT CrystalGolemAttackRangeBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CrystalGolemAttackRangeBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++CrystalGolemAttackRangeBullet_Id;
	m_tInfo.wstrName = TO_WSTR("CrystalGolemAttackRangeBullet" + to_string(CrystalGolemAttackRangeBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CrystalGolemAttackRangeBullet_Id;

	tagCrystalGolemAttackRangeBulletDesc tCrystalGolemAttackRangeBulletDesc;
	if (nullptr != pArg)
		tCrystalGolemAttackRangeBulletDesc = *(tagCrystalGolemAttackRangeBulletDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCrystalGolemAttackRangeBulletDesc.vPosition));

	m_pTimeCounterCom->Enable();

	return S_OK;
}

void CrystalGolemAttackRangeBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
	m_pTimeCounterCom->Tick(TimeDelta);

	// 수명
	if (m_pTimeCounterCom->isEuqalWith(1.0))
		__super::SetDead();

	m_pTransformCom->Go_Straight(TimeDelta);

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
	}

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void CrystalGolemAttackRangeBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_DebugGroup(m_pColliderCom);

	Safe_Release(pGameInstance);
}

HRESULT CrystalGolemAttackRangeBullet::Render()
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

void CrystalGolemAttackRangeBullet::ResetPool(void* pArg)
{
	m_bDead = false;
	_vector vPosition = XMLoadFloat4(&((tagCrystalGolemAttackRangeBulletDesc*)pArg)->vPosition);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);



	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
	m_pTimeCounterCom->Enable();
}

void CrystalGolemAttackRangeBullet::SetDead()
{
	__super::SetDead();
	ObjectPool<CrystalGolemAttackRangeBullet>::GetInstance()->PushPool(this);
	m_pTimeCounterCom->Reset();
	m_pTimeCounterCom->Disable();
}

HRESULT CrystalGolemAttackRangeBullet::Add_Components()
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
	tColliderSphereDesc.fRadius = { 0.2f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	TimeCounter::TIME_COUNTER_DESC tTimeCounterDesc;
	tTimeCounterDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, TimeCounter::ProtoTag(), L"Com_TimeCounter", (CComponent**)&m_pTimeCounterCom, &tTimeCounterDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CrystalGolemAttackRangeBullet::SetUp_ShaderResources()
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

void CrystalGolemAttackRangeBullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (__super::isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

CrystalGolemAttackRangeBullet* CrystalGolemAttackRangeBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CrystalGolemAttackRangeBullet* pInstance = new CrystalGolemAttackRangeBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CrystalGolemAttackRangeBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* CrystalGolemAttackRangeBullet::Clone(void* pArg)
{
	CrystalGolemAttackRangeBullet* pInstance = new CrystalGolemAttackRangeBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CrystalGolemAttackRangeBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CrystalGolemAttackRangeBullet::Free(void)
{
	__super::Free();

	--CrystalGolemAttackRangeBullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTimeCounterCom);
}