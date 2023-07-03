#include "Clint.h"
#include "GameInstance.h"
#include "ClintAnimState.h"
#include "ClintAnimIdle.h"
#include "ClintAnimRun.h"
#include "ClintAnimDash.h"
_uint Clint::Clint_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Clint::Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Clint::Clint(const Clint& rhs)
	: CGameObject(rhs)
	, m_pClintAnimStates(rhs.m_pClintAnimStates)
	, m_eClintAnimState(rhs.m_eClintAnimState)
{
	for (auto ClintAnimState : m_pClintAnimStates)
		Safe_AddRef(ClintAnimState);
}

HRESULT Clint::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_pClintAnimStates.resize(static_cast<_int>(CLINT_ANIM::CLINT_ANIM_END));
	m_pClintAnimStates[static_cast<_uint>(CLINT_ANIM::IDLE)] = ClintAnimIdle::Create(m_pDevice, m_pContext, this);
	m_pClintAnimStates[static_cast<_uint>(CLINT_ANIM::RUN)] = ClintAnimRun::Create(m_pDevice, m_pContext, this);
	m_pClintAnimStates[static_cast<_uint>(CLINT_ANIM::DASH)] = ClintAnimDash::Create(m_pDevice, m_pContext, this);

	return S_OK;
}

HRESULT Clint::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	// 원본에서 결정하면 컴포넌트가 없으므로 클론에서 소유자를 결정해줘야함.
	for (auto& ClintAnimState : m_pClintAnimStates)
	{
		if (ClintAnimState)
			ClintAnimState->Set_Owner(this);
	}

	++Clint_Id;
	m_tInfo.wstrName = TO_WSTR("Clint" + to_string(Clint_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Clint_Id;

	CGAMEOBJECT_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(CGAMEOBJECT_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void Clint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 현재 애니메이션 상태에 맞는 틱 호출
	m_pClintAnimStates[static_cast<_uint>(m_eClintAnimState)]->Tick(TimeDelta);

	// 렌더러 그룹에 추가
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	// TransfomationMatirx의 값을 갱신하고 CombinedTransformationMatrix를 순차적으로 갱신
	m_pModelCom->Play_Animation(TimeDelta);

	// 	if(nullptr != m_pColliderCom)
	//		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void Clint::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	// 현재 애니메이션 상태에 맞는 틱 호출
	m_pClintAnimStates[static_cast<_uint>(m_eClintAnimState)]->Late_Tick(TimeDelta);
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

void Clint::Set_ClintAnimState(CLINT_ANIM eClintAnim)
{
	m_eClintAnimState = eClintAnim;

	switch (eClintAnim)
	{
	case Client::CLINT_ANIM::DASH:
		m_pModelCom->Set_AnimByIndex(27);
		break;
	case Client::CLINT_ANIM::DEATH:
		m_pModelCom->Set_AnimByIndex(29);
		break;
	case Client::CLINT_ANIM::GRANADE:
		m_pModelCom->Set_AnimByIndex(50);
		break;
	case Client::CLINT_ANIM::HIT:
		m_pModelCom->Set_AnimByIndex(53);
		break;
	case Client::CLINT_ANIM::IDLE:
		m_pModelCom->Set_AnimByIndex(64);
		break;
	case Client::CLINT_ANIM::MVP:
		m_pModelCom->Set_AnimByIndex(74);
		break;
	case Client::CLINT_ANIM::RUN:
		m_pModelCom->Set_AnimByIndex(89);
		break;
	case Client::CLINT_ANIM::SKILL_01:
		m_pModelCom->Set_AnimByIndex(109);
		break;
	case Client::CLINT_ANIM::SKILL_02:
		m_pModelCom->Set_AnimByIndex(122);
		break;
	case Client::CLINT_ANIM::WEAPONCHANGE:
		m_pModelCom->Set_AnimByIndex(152);
		break;
	default:
		break;
	}
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

	for (auto ClintAnimState : m_pClintAnimStates)
		Safe_Release(ClintAnimState);

	--Clint_Id;
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

