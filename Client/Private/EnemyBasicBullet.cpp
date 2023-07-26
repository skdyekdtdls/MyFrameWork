#include "EnemyBasicBullet.h"
#include "GameInstance.h"

_uint EnemyBasicBullet::EnemyBasicBullet_Id = 0;

EnemyBasicBullet::EnemyBasicBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

EnemyBasicBullet::EnemyBasicBullet(const EnemyBasicBullet& rhs)
	: Bullet(rhs)
{
}

HRESULT EnemyBasicBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT EnemyBasicBullet::Initialize(void* pArg)
{
	ENEMY_BASIC_BULLET_DESC tEnemyBasicBulletDesc;
	
	tEnemyBasicBulletDesc = *(ENEMY_BASIC_BULLET_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components(tEnemyBasicBulletDesc.fRadius)))
		return E_FAIL;

	++EnemyBasicBullet_Id;
	m_tInfo.wstrName = TO_WSTR("EnemyBasicBullet" + to_string(EnemyBasicBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = EnemyBasicBullet_Id;

	return S_OK;
}

void EnemyBasicBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void EnemyBasicBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	Safe_Release(pGameInstance);
}

HRESULT EnemyBasicBullet::Render()
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

HRESULT EnemyBasicBullet::Add_Components(_float& vExtents)
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
	
	CColliderSphere::CCOLLIDER_SPHERE_DESC tCollider;
	tCollider.pOwner = this;
	tCollider.fRadius = 5.f;
	tCollider.vCenter = _float3(0.f, 1.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tCollider), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT EnemyBasicBullet::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

void EnemyBasicBullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (__super::isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		__super::Damage(tCollisionInfo.pOtherGameObject);
		SetDead();
	}
}

EnemyBasicBullet* EnemyBasicBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	EnemyBasicBullet* pInstance = new EnemyBasicBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created EnemyBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* EnemyBasicBullet::Clone(void* pArg)
{
	EnemyBasicBullet* pInstance = new EnemyBasicBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned EnemyBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void EnemyBasicBullet::Free(void)
{
	__super::Free();

	--EnemyBasicBullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}