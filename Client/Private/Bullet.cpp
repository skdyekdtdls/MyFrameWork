#include "Bullet.h"
#include "GameInstance.h"

/* Don't Forget Release for the VIBuffer or Model Component*/

Bullet::Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Bullet::Bullet(const Bullet& rhs)
	: CGameObject(rhs)
	, m_TimeAcc(0.0)
	, m_LifeSpan(1.0)
	, m_fDamage(rhs.m_fDamage)
{
}

HRESULT Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	CLINT_BASIC_BULLET_DESC tClintBasicBulletDesc;
	if (nullptr != pArg)
		tClintBasicBulletDesc = *(CLINT_BASIC_BULLET_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tClintBasicBulletDesc.vPosition));
	//m_pModelCom->Set_RootNode(3);

	return S_OK;
}

void Bullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);

	LifeSpan(TimeDelta);
	// 이동
	m_pTransformCom->Go_Straight(TimeDelta);

	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
		m_pColliderCom->Add_ColliderGroup(COLL_GROUP::PLAYER_BULLET);
	}
}

void Bullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);
	if (pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif

	Safe_Release(pGameInstance);
}

HRESULT Bullet::Render()
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
	//	// m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

	//	m_pShaderCom->Begin(0);

	//	m_pModelCom->Render(i);
	//}

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);
}

void Bullet::Damage(CGameObject* pGameObejct)
{
	Health* pHealth = dynamic_cast<Health*>(pGameObejct->Get_Component(L"Com_Health"));

	if (pHealth)
		pHealth->TakeDamage(m_fDamage);
}

void Bullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		Damage(tCollisionInfo.pOtherGameObject);
	}
}

HRESULT Bullet::Add_Components()
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
	//FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, /*L"Prototype_Component_Model_", L"Com_Model"*/, (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tColliderSphereDesc;
	tColliderSphereDesc.pOwner = this;
	tColliderSphereDesc.fRadius = { 0.05f };
	tColliderSphereDesc.vCenter = _float3(0.0f, 0.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderSphereDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Bullet::SetUp_ShaderResources()
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

void Bullet::LifeSpan(_double TimeDelta)
{
	m_TimeAcc += TimeDelta;
	if (m_LifeSpan < m_TimeAcc)
	{
		__super::SetDead();
	}
}

void Bullet::Free(void)
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}

