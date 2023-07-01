#include "Clint.h"
#include "GameInstance.h"
_uint Clint::Clint_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Clint::Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Clint::Clint(const Clint& rhs)
	: CGameObject(rhs)
	, m_eCurState(rhs.m_eCurState)
	, m_ePreState(rhs.m_ePreState)
{
}

void Clint::Set_AnimState(CLINT_ANIM eAnimState)
{
	if (m_eCurState == eAnimState)
		return;

	m_eCurState = eAnimState;
	m_pModelCom->Set_AnimByIndex(static_cast<_uint>(eAnimState));
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

	CLONE_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(CLONE_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void Clint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	switch (m_eCurState)
	{
	case Client::CLINT_ANIM::DASH:
		Dash_FSM(TimeDelta);
		break;
	case Client::CLINT_ANIM::DEATH:
		break;
	case Client::CLINT_ANIM::GRANADE:
		break;
	case Client::CLINT_ANIM::HIT:
		break;
	case Client::CLINT_ANIM::IDLE:
		Idle_FSM(TimeDelta);
		break;
	case Client::CLINT_ANIM::MVP:
		break;
	case Client::CLINT_ANIM::RUN:
		Run_FSM(TimeDelta);
		break;
	case Client::CLINT_ANIM::SKILL_01:
		break;
	case Client::CLINT_ANIM::SKILL_02:
		break;
	case Client::CLINT_ANIM::WEAPONCHANGE:
		break;
	default:
		break;
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void Clint::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	m_pModelCom->Play_Animation(TimeDelta);
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
	// if(nullptr != m_pColliderCom)
	//	m_pColliderCom->Render();
#endif
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

void Clint::KeyInput(_double& TimeDelta)
{
	if (g_hWnd != ::GetFocus())
		return;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	//BasicAttack(TimeDelta);

	Safe_Release(pGameInstance);
}


void Clint::Shoot_FSM(_double TimeDelta)
{
}

void Clint::Run_FSM(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	Safe_Release(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_NORTH, m_pNavigationCom);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_A))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_WEST, m_pNavigationCom);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_S))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_SOUTH, m_pNavigationCom);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_EAST, m_pNavigationCom);
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	}
	else if (pGameInstance->Get_DIKeyState(DIK_SPACE))
	{
		Set_AnimState(CLINT_ANIM::DASH);
	}
	else
	{
		Set_AnimState(CLINT_ANIM::IDLE);
	}

	Safe_AddRef(pGameInstance);
}

void Clint::Idle_FSM(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_NORTH, m_pNavigationCom);
		Set_AnimState(CLINT_ANIM::RUN);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_A))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_WEST, m_pNavigationCom);
		Set_AnimState(CLINT_ANIM::RUN);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_S))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_SOUTH, m_pNavigationCom);
		Set_AnimState(CLINT_ANIM::RUN);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_EAST, m_pNavigationCom);
		Set_AnimState(CLINT_ANIM::RUN);
	}
	else if (pGameInstance->Get_DIKeyState(DIK_SPACE))
	{
		Set_AnimState(CLINT_ANIM::DASH);
	}

	Safe_Release(pGameInstance);
}

void Clint::Dash_FSM(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransformCom->Go_NSEW(TimeDelta, CTransform::DIR_NORTH, m_pNavigationCom);
		Set_AnimState(CLINT_ANIM::RUN);
	}
	else if (m_pModelCom->IsAnimationFinished())
	{
		Set_AnimState(CLINT_ANIM::IDLE);
	}

	_float3 pos = m_pModelCom->Get_RootTranslation();
	m_pTransformCom->RootMotion(TimeDelta, pos);
	//m_pTransformCom->

	Safe_Release(pGameInstance);
}

HRESULT Clint::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);

	// no texture now, you have to add texture later

	CTransform::TRANSFORMDESC TransformDesc{ 7.0, XMConvertToRadians(720.f) };
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", (CComponent**)&m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Clint", L"Com_Model", (CComponent**)&m_pModelCom), E_FAIL);
	CNavigation::NAVIGATIONDESC tNavigationdesc;
	tNavigationdesc.iCurrentIndex = { 0 };
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, CNavigation::ProtoTag(), L"Com_Navigation", (CComponent**)&m_pNavigationCom, &tNavigationdesc), E_FAIL);

	Safe_Release(pGameInstance);
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
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

