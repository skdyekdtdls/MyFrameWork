#include "CrystalGolem.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "Bullet.h"
#include "CrystalGolemAttackAreaBullet.h"
#include "CrystalGolemAttackArea02Bullet.h"
#include "CrystalGolemAttackRangeBullet.h"

_uint CrystalGolem::CrystalGolem_Id = 0;

CrystalGolem::CrystalGolem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CrystalGolem::CrystalGolem(const CrystalGolem& rhs)
	: CGameObject(rhs)
{
}

HRESULT CrystalGolem::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CrystalGolem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// 태그초기화
	++CrystalGolem_Id;
	m_tInfo.wstrName = TO_WSTR("CrystalGolem" + to_string(CrystalGolem_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = CrystalGolem_Id;

	// 상태 초기화
	m_pStateContextCom->TransitionTo(L"CrystalGolemAppear");

	// Desc초기화
	tagCrystalGolemDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagCrystalGolemDesc*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	// 노티파이 초기화
	Add_Notify();

	return S_OK;
}

void CrystalGolem::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	if (pGameInstance->Key_Down(DIK_P))
		m_pTransformCom->RotationBack();
	Safe_Release(pGameInstance);


	m_pModelCom->Play_Animation(TimeDelta);

	if (nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	if (m_pHealthCom->isZeroHP())
	{
		m_pStateContextCom->TransitionTo(L"CrystalGolemDead");
		return;
	}

	if (nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());


	if (nullptr != m_pColliderDectect)
	{
		m_pColliderDectect->Tick(m_pTransformCom->Get_WorldMatrix());
	}
}

void CrystalGolem::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (Facade->isRender(m_pRendererCom, m_pTransformCom))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

		//Dead상태가 아니면 콜라이더 넣음, Appear상태가 아니면 콜라이더를 넣음.
		if (0 != lstrcmp(L"CrystalGolemDead", m_pStateContextCom->GetCurState())
			|| 0 != lstrcmp(L"CrystalGolemAppear", m_pStateContextCom->GetCurState()))
		{
			m_pColliderCom->Add_ColliderGroup(COLL_GROUP::MONSTER_BODY);
		}
	}

#ifdef _DEBUG
	if (nullptr != m_pColliderCom)
		m_pRendererCom->Add_DebugGroup(m_pColliderCom);
	m_pRendererCom->Add_DebugGroup(m_pColliderDectect);
#endif
}

HRESULT CrystalGolem::Render()
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

void CrystalGolem::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	// 총알은 레이어가 없다..
	Bullet* pBullet;
	if (pBullet = dynamic_cast<Bullet*>(tCollisionInfo.pOtherGameObject))
	{
		if (m_pTransformCom->isFront(Facade->GetClintPosition()))
		{
			cout << "방어" << endl;

			m_pHealthCom->Heal(pBullet->GetDamage());
		}	
	}

	// 바디콜라이더 끼리 부딪친 경우.
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

HRESULT CrystalGolem::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 3.0, XMConvertToRadians(60.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_CrystalGolem", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(1.5f, 1.f, 1.5f);
	tColliderAABBDesc.vCenter = _float3(0.f, tColliderAABBDesc.Extents.y, 1.5f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	CColliderSphere::CCOLLIDER_SPHERE_DESC tDectDesc;
	tDectDesc.pOwner = this;
	tDectDesc.fRadius = 3.f;
	tDectDesc.vCenter = _float3(0.f, 1.f, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderSphere::ProtoTag(), L"Com_CollDetect", (CComponent**)&m_pColliderDectect, &tDectDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	CrystalGolemState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_CrystalGolemState"), L"Com_StateContext", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);

	Health::HEALTH_DESC tHealthDesc;
	tHealthDesc.pOwner = this;
	tHealthDesc.iMaxHp = 3000;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, Health::ProtoTag(), L"Com_Health", (CComponent**)&m_pHealthCom, &tHealthDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CrystalGolem::Add_Notify()
{
	m_pModelCom->Add_TimeLineEvent("CrystalGolem_attack_area", L"AttackArea", TIMELINE_EVENT(43.7, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CrystalGolemAttackAreaBullet::tagCrystalGolemAttackAreaBulletDesc tBulletDesc;
		tBulletDesc.pOwner = this;
		tBulletDesc.fDamage = 10.f;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPosition += vLook * 3.f;
		XMStoreFloat4(&tBulletDesc.vPosition, vPosition);

		Bullet* pBullet = ObjectPool<CrystalGolemAttackAreaBullet>::GetInstance()->PopPool(CrystalGolemAttackAreaBullet::ProtoTag(), &tBulletDesc);

		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));

	m_pModelCom->Add_TimeLineEvent("CrystalGolem_attack_area_02", L"AttackArea02", TIMELINE_EVENT(43.7, [this]() {
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CrystalGolemAttackArea02Bullet::tagCrystalGolemAttackArea02BulletDesc tBulletDesc;
		tBulletDesc.pOwner = this;
		tBulletDesc.fDamage = 10.f;
		_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPosition += vLook * 1.f;
		XMStoreFloat4(&tBulletDesc.vPosition, vPosition);

		Bullet* pBullet = ObjectPool<CrystalGolemAttackArea02Bullet>::GetInstance()->PopPool(CrystalGolemAttackArea02Bullet::ProtoTag(), &tBulletDesc);

		pGameInstance->AddToLayer(pGameInstance->Get_CurLevelIndex(), L"Layer_Bullet", pBullet);

		Safe_Release(pGameInstance);

		}));

	return S_OK;
}

HRESULT CrystalGolem::SetUp_ShaderResources()
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

CrystalGolem* CrystalGolem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CrystalGolem* pInstance = new CrystalGolem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created CrystalGolem");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CrystalGolem::Clone(void* pArg)
{
	CrystalGolem* pInstance = new CrystalGolem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned CrystalGolem");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CrystalGolem::Free(void)
{
	__super::Free();

	--CrystalGolem_Id;
	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateContextCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderDectect);
	Safe_Release(m_pHealthCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}