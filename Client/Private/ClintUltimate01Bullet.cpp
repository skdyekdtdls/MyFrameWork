#include "ClintUltimate01Bullet.h"
#include "GameInstance.h"

_uint ClintUltimate01Bullet::ClintUltimate01Bullet_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

ClintUltimate01Bullet::ClintUltimate01Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

ClintUltimate01Bullet::ClintUltimate01Bullet(const ClintUltimate01Bullet& rhs)
	: Bullet(rhs)
{
}

HRESULT ClintUltimate01Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ClintUltimate01Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++ClintUltimate01Bullet_Id;
	m_tInfo.wstrName = TO_WSTR("ClintUltimate01Bullet" + to_string(ClintUltimate01Bullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = ClintUltimate01Bullet_Id;

	CLINT_ULTIMATE01_BULLET_DESC tClintUltimate01BulletDesc;
	if (nullptr != pArg)
		tClintUltimate01BulletDesc = *(CLINT_ULTIMATE01_BULLET_DESC*)pArg;

	return S_OK;
}

void ClintUltimate01Bullet::Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;

	CTransform* pOwnerTransform = static_cast<CTransform*>(m_pOwner->Get_Component(L"Com_Transform"));
	m_pTransformCom->Set_WorldMatrix(pOwnerTransform->Get_WorldMatrix());
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void ClintUltimate01Bullet::Late_Tick(_double TimeDelta)
{
	if (false == m_bEnable)
		return;
	__super::Late_Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	m_pColliderCom->Add_ColliderGroup(COLL_GROUP::PLAYER_BULLET);

	// 	m_pModelCom->Play_Animation(TimeDelta);
	if (pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

#ifdef _DEBUG
	m_pRendererCom->Add_DebugGroup(m_pColliderCom);
#endif

	Safe_Release(pGameInstance);
	m_bEnable = false;
}

HRESULT ClintUltimate01Bullet::Render()
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

HRESULT ClintUltimate01Bullet::Add_Components()
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

	CColliderSphere::CCOLLIDER_SPHERE_DESC tUltCollider;
	tUltCollider.pOwner = this;
	tUltCollider.fRadius = 5.f;
	tUltCollider.vCenter = _float3(0.f, 1.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_Coll", (CComponent**)&m_pColliderCom, &tUltCollider), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT ClintUltimate01Bullet::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

ClintUltimate01Bullet* ClintUltimate01Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ClintUltimate01Bullet* pInstance = new ClintUltimate01Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ClintUltimate01Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* ClintUltimate01Bullet::Clone(void* pArg)
{
	ClintUltimate01Bullet* pInstance = new ClintUltimate01Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ClintUltimate01Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ClintUltimate01Bullet::Free(void)
{
	__super::Free();

	--ClintUltimate01Bullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);

}
