#include "CannonSpider.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "CannonSpiderBullet.h"
_uint CannonSpider::CannonSpider_Id = 0;

CannonSpider::CannonSpider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CannonSpider::CannonSpider(const CannonSpider& rhs)
	: CGameObject(rhs)
{
}

HRESULT CannonSpider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CannonSpider::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// �±��ʱ�ȭ
	++CannonSpider_Id;
	m_tInfo.wstrName = TO_WSTR("CannonSpider" + to_string(CannonSpider_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CannonSpider_Id;

	// ���� �ʱ�ȭ
	m_pStateContextCom->TransitionTo(L"CannonSpiderSearch");

	// Desc�ʱ�ȭ
	tagCannonSpiderDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagCannonSpiderDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void CannonSpider::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	m_pModelCom->Play_Animation(TimeDelta);

	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	if (m_pHealthCom->isZeroHP())
	{
		m_pStateContextCom->TransitionTo(L"CannonSpiderDead");
		return;
	}

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	// m_pBullet->Tick(TimeDelta);

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}
}

void CannonSpider::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Facade->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

		// Dead���°� �ƴϸ� �ݶ��̴� ����
		if (0 != lstrcmp(L"CannonSpiderDead", m_pStateContextCom->GetCurState()))
			m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BODY);
	}

	m_pBullet->Late_Tick(TimeDelta);

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	m_pRendererCom->Add_DebugGroup(m_pRaycastCom);
#endif
}

HRESULT CannonSpider::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, TextureType_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
}

void CannonSpider::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// �Ѿ��� ���̾ ����..
	if (dynamic_cast<Bullet*>(tCollisionInfo.pOtherGameObject))
	{
		m_pStateContextCom->TransitionTo(TEXT("CannonSpiderIdle"));
		m_pStateContextCom->TransitionTo(TEXT("CannonSpiderHit"));
	}

	// �ٵ��ݶ��̴� ���� �ε�ģ ���.
	if (tCollisionInfo.MyCollName == TEXT("Com_BodyColl")
		&& tCollisionInfo.OtherCollName == TEXT("Com_BodyColl")
		&& tCollisionInfo.OtherGameObjectLayerName == TEXT("Layer_Monster"))
	{
		tCollisionInfo.vOverLapVector.y = 0.f;
		m_pTransformCom->Go_Direction(TimeDelta, -XMLoadFloat3(&tCollisionInfo.vOverLapVector)
			, XMVectorGetX(XMVector3Length(XMLoadFloat3(&tCollisionInfo.vOverLapVector))));
	}

	m_pStateContextCom->OnCollision(tCollisionInfo, TimeDelta);
}

HRESULT CannonSpider::Add_Components()
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
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_CannonSpider", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(-0.15f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.0f, 1.f, 0.f);
	tRaycastDesc.fLength = { 6.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RayDetect", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);

	CannonSpiderState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_CannonSpiderState"), L"Com_StateContext", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);
	
	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 1200;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	CannonSpiderBullet::tagCannonSpiderBulletDesc tBulletDesc;
	tBulletDesc.pOwner = this;
	tBulletDesc.fDamage = 10.f;
	XMStoreFloat4(&tBulletDesc.vPosition, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	FAILED_CHECK_RETURN(__super::Add_Composite(CannonSpiderBullet::ProtoTag(), L"Com_SpiderBullet", (CComponent**)&m_pBullet, &tBulletDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CannonSpider::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &MyMatrix), E_FAIL);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &MyMatrix), E_FAIL);

	MyMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &MyMatrix), E_FAIL);

	Safe_Release(pGameInstance);

	return S_OK;
}

CannonSpider* CannonSpider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CannonSpider* pInstance = new CannonSpider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CannonSpider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CannonSpider::Clone(void* pArg)
{
	CannonSpider* pInstance = new CannonSpider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CannonSpider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CannonSpider::Free(void)
{
	__super::Free();

	--CannonSpider_Id;

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateContextCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pHealthCom);
	Safe_Release(m_pBullet);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}