#include "CrystalGolemAttackArea02Bullet.h"
#include "GameInstance.h"

_uint CrystalGolemAttackArea02Bullet::CrystalGolemAttackArea02Bullet_Id = 0;

CrystalGolemAttackArea02Bullet::CrystalGolemAttackArea02Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

CrystalGolemAttackArea02Bullet::CrystalGolemAttackArea02Bullet(const CrystalGolemAttackArea02Bullet& rhs)
	: Bullet(rhs)
{
}

HRESULT CrystalGolemAttackArea02Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT CrystalGolemAttackArea02Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++CrystalGolemAttackArea02Bullet_Id;
	m_tInfo.wstrName = TO_WSTR("CrystalGolemAttackArea02Bullet" + to_string(CrystalGolemAttackArea02Bullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CrystalGolemAttackArea02Bullet_Id;

	tagCrystalGolemAttackArea02BulletDesc tCrystalGolemAttackArea02BulletDesc;
	if (nullptr != pArg)
		tCrystalGolemAttackArea02BulletDesc = *(tagCrystalGolemAttackArea02BulletDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCrystalGolemAttackArea02BulletDesc.vPosition));

	return S_OK;
}

void CrystalGolemAttackArea02Bullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 수명
	__super::SetDead();

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BULLET);
	}

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void CrystalGolemAttackArea02Bullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);
	#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif
	Safe_Release(pGameInstance);
}

HRESULT CrystalGolemAttackArea02Bullet::Render()
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

void CrystalGolemAttackArea02Bullet::ResetPool(void* pArg)
{
	m_bDead = false;
	_vector vPosition = XMLoadFloat4(&((tagCrystalGolemAttackArea02BulletDesc*)pArg)->vPosition);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

void CrystalGolemAttackArea02Bullet::SetDead()
{
	__super::SetDead();
	ObjectPool<CrystalGolemAttackArea02Bullet>::GetInstance()->PushPool(this);
}

HRESULT CrystalGolemAttackArea02Bullet::Add_Components()
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
	tColliderSphereDesc.fRadius = { 5.f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CrystalGolemAttackArea02Bullet::SetUp_ShaderResources()
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

void CrystalGolemAttackArea02Bullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (__super::isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

CrystalGolemAttackArea02Bullet* CrystalGolemAttackArea02Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CrystalGolemAttackArea02Bullet* pInstance = new CrystalGolemAttackArea02Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CrystalGolemAttackArea02Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* CrystalGolemAttackArea02Bullet::Clone(void* pArg)
{
	CrystalGolemAttackArea02Bullet* pInstance = new CrystalGolemAttackArea02Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CrystalGolemAttackArea02Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CrystalGolemAttackArea02Bullet::Free(void)
{
	__super::Free();

	--CrystalGolemAttackArea02Bullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pColliderCom);
}
