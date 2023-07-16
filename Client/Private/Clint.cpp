#include "Clint.h"
#include "GameInstance.h"
#include "StateContext.h"
#include "Pistola.h"
#include "ClintIdle.h"
#include "ClintRun.h"
#include "ClintDash.h"
#include "ClintShoot.h"
#include "Animation.h"

_uint Clint::Clint_Id = 0;

Clint::Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
	
}

Clint::Clint(const Clint& rhs)
	: CGameObject(rhs)
{

}

HRESULT Clint::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Clint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Clint_Id;
	m_tInfo.wstrName = TO_WSTR("Clint" + to_string(Clint_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Clint_Id;

	// 노티파이.
	_double AttackInterval = { 12.0 };
	_double Start = { 1.0 };
	m_pModelCom->Add_TimeLineEvent("Clint_basic_shoot", L"RShoot1", TIMELINE_EVENT(Start, [this]() {
		m_pPistolaComL->Attack(GetLook());
		}), UPPER);
	m_pModelCom->Add_TimeLineEvent("Clint_basic_shoot", L"LShoot1", TIMELINE_EVENT(Start += AttackInterval, [this]() {
		m_pPistolaComR->Attack(GetLook());
		}), UPPER);
	m_pModelCom->Add_TimeLineEvent("Clint_basic_shoot", L"RShoot2", TIMELINE_EVENT(Start += AttackInterval, [this]() {
		m_pPistolaComL->Attack(GetLook());
		}), UPPER);
	m_pModelCom->Add_TimeLineEvent("Clint_basic_shoot", L"LShoot2", TIMELINE_EVENT(Start += AttackInterval, [this]() {
		m_pPistolaComR->Attack(GetLook());
		}), UPPER);

	if(nullptr != m_pStateContextCom)
		m_pStateContextCom->TransitionTo(L"ClintIdle");

	// 모델 재생속도 조정
	m_pModelCom->SetAnimationPlaySpeedByIndex(1.5, CLINT_ULTIMATE01, LOWER);
	m_pModelCom->SetAnimationPlaySpeedByIndex(1.5, CLINT_ULTIMATE01, UPPER);
	

	// 총쏘기 지속시간을 조정.
	CAnimation* pAnimation;
	pAnimation  = m_pModelCom->GetAnimationByName("Clint_basic_shoot", UPPER);
	pAnimation->SetDuration(25.0);

	// 초기정보 세팅(위치)
	CGAMEOBJECT_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(CGAMEOBJECT_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void Clint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	if(nullptr != m_pStateContextCom)
		m_pStateContextCom->Tick(TimeDelta);

	// TransfomationMatirx의 값을 갱신하고 CombinedTransformationMatrix를 순차적으로 갱신
	m_pModelCom->Play_Animation(TimeDelta, LOWER);
	m_pModelCom->Play_Animation(TimeDelta, UPPER);

	if(nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (nullptr != m_pPistolaComL && nullptr != m_pPistolaComR)
	{
		m_pPistolaComL->Tick(TimeDelta);
		m_pPistolaComR->Tick(TimeDelta);
	}

	if (nullptr != m_pRaycastCom)
	{
		m_pRaycastCom->Tick(m_pTransformCom->Get_State(CTransform::STATE_POSITION),
			m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	}
}

void Clint::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	// 렌더러 그룹에 추가
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pColliderCom->Add_ColliderGroup(COLL_GROUP::PLAYER_BODY);

	if (nullptr != m_pPistolaComL && nullptr != m_pPistolaComR)
	{
		m_pPistolaComL->Late_Tick(TimeDelta);
		m_pPistolaComR->Late_Tick(TimeDelta);
	}
}

HRESULT Clint::Render()
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
		// m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}
	
#ifdef _DEBUG
	//m_pNavigationCom->Render_Navigation();
	 if(nullptr != m_pColliderCom)
		m_pColliderCom->Render();
	 if (nullptr != m_pRaycastCom)
		 m_pRaycastCom->Render();
#endif
	 return S_OK;
}

_vector Clint::GetPosition()
{
	return m_pTransformCom->Get_State(CTransform::STATE_POSITION);
}

_vector Clint::GetLook()
{
	return m_pTransformCom->Get_State(CTransform::STATE_LOOK);
}

void Clint::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void Clint::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT Clint::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);
	// no texture now, you have to add texture later

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Clint", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	CNavigation::CNAVIGATION_DESC tNavigationdesc; tNavigationdesc.pOwner = this;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Pistola::PISTOLA_DESC tPistolaDesc;
	tPistolaDesc.pOwner = this;
	tPistolaDesc.pAttachedBoneName = "Prop2";
	FAILED_CHECK_RETURN(__super::Add_Composite(Pistola::ProtoTag(), L"Com_Pistola_L", (CComponent**)&m_pPistolaComL, &tPistolaDesc), E_FAIL);
	tPistolaDesc.pAttachedBoneName = "Prop1";
	FAILED_CHECK_RETURN(__super::Add_Composite(Pistola::ProtoTag(), L"Com_Pistola_R", (CComponent**)&m_pPistolaComR, &tPistolaDesc), E_FAIL);

	CColliderAABB::CCOLLIDER_AABB_DESC tColliderAABBDesc;
	tColliderAABBDesc.pOwner = this;
	tColliderAABBDesc.Extents = _float3(0.3f, 1.f, 0.3f);
	tColliderAABBDesc.vCenter = _float3(-0.15f, tColliderAABBDesc.Extents.y, 0.f);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CColliderAABB::ProtoTag(), L"Com_BodyColl", (CComponent**)&m_pColliderCom, &tColliderAABBDesc), E_FAIL);

	Raycast::RAYCAST_DESC tRaycastDesc;
	tRaycastDesc.pOwner = this;
	tRaycastDesc.vCenter = _float3(0.15f, 1.f, 0.f);
	tRaycastDesc.fLength = { 1.f };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, Raycast::ProtoTag(), L"Com_RaycastTest", (CComponent**)&m_pRaycastCom, &tRaycastDesc), E_FAIL);
	Safe_Release(pGameInstance);
	
	ClintState::STATE_CONTEXT_DESC tStateContextDesc;
	tStateContextDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_ClintState"), L"Com_ClintState", (CComponent**)&m_pStateContextCom, &tStateContextDesc), E_FAIL);
	
	return S_OK;
}

HRESULT Clint::SetUp_ShaderResources()
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

void Clint::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{

	m_pStateContextCom->OnCollision(tCollisionInfo, TimeDelta);
}

Clint* Clint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Clint* pInstance = new Clint(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Clint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Clint::Clone(void* pArg)
{
	Clint* pInstance = new Clint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Clint");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Clint::Free(void)
{
	__super::Free();

	--Clint_Id;
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pPistolaComL);
	Safe_Release(m_pPistolaComR);
	Safe_Release(m_pRaycastCom);
	Safe_Release(m_pStateContextCom);
}

