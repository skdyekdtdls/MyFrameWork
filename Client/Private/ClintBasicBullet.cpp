#include "ClintBasicBullet.h"
#include "GameInstance.h"

_uint ClintBasicBullet::ClintBasicBullet_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

ClintBasicBullet::ClintBasicBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

ClintBasicBullet::ClintBasicBullet(const ClintBasicBullet& rhs)
	: CGameObject(rhs)
{
}

HRESULT ClintBasicBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT ClintBasicBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++ClintBasicBullet_Id;
	m_tInfo.wstrName = TO_WSTR("ClintBasicBullet" + to_string(ClintBasicBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = ClintBasicBullet_Id;

	CLINT_BASIC_BULLET_DESC tClintBasicBulletDesc;
	if (nullptr != pArg)
		tClintBasicBulletDesc = *(CLINT_BASIC_BULLET_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tClintBasicBulletDesc.vPosition));
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, tClintBasicBulletDesc.vLook);
	//m_pModelCom->Set_RootNode(3);

	return S_OK;
}

void ClintBasicBullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);

	LifeSpan(TimeDelta);
	// �̵�
	m_pTransformCom->Go_Straight(TimeDelta);

	 if(nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void ClintBasicBullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);
	if(pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	CheckCollision(TimeDelta);

	Safe_Release(pGameInstance);
}

HRESULT ClintBasicBullet::Render()
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

	// ���࿡ �� ������Ʈ �Ⱦ��� �̰ɷ� ����ȴ�.
	// m_pShaderCom->Begin(0);

#ifdef _DEBUG
	 if(nullptr != m_pColliderCom)
		m_pColliderCom->Render();
#endif
}

HRESULT ClintBasicBullet::Add_Components()
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

HRESULT ClintBasicBullet::SetUp_ShaderResources()
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

void ClintBasicBullet::CheckCollision(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLayer* pLayer = pGameInstance->Find_LayerByName(pGameInstance->Get_CurLevelIndex(), L"Layer_Monster");
	for (auto iter = pLayer->Begin(); iter != pLayer->End(); ++iter)
	{
		CCollider* pCollider = { nullptr };
		if (pCollider = static_cast<CCollider*>((*iter)->Get_Component(L"Com_BodyColl")))
		{
			if (pCollider->Intersect(m_pColliderCom))
			{
				SetDead();
				CCollider::COLLISION_INFO tCollisionInfo;
				tCollisionInfo.pOtherCollider = { m_pColliderCom };
				tCollisionInfo.tInfo = { m_pOwner->GetInfo() };
				(*iter)->OnCollision(&tCollisionInfo);
				break;
			}
		}
	}
	Safe_Release(pGameInstance);
}

void ClintBasicBullet::LifeSpan(_double TimeDelta)
{
	m_TimeAcc += TimeDelta;
	if (m_LifeSpan < m_TimeAcc)
	{
		__super::SetDead();
	}
}

ClintBasicBullet* ClintBasicBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ClintBasicBullet* pInstance = new ClintBasicBullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ClintBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* ClintBasicBullet::Clone(void* pArg)
{
	ClintBasicBullet* pInstance = new ClintBasicBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ClintBasicBullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ClintBasicBullet::Free(void)
{
	__super::Free();

	--ClintBasicBullet_Id;
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);

}
