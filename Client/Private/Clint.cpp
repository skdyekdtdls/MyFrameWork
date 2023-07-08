#include "Clint.h"
#include "GameInstance.h"
#include "Pistola.h"
#include "ClintIdle.h"
#include "ClintRun.h"
#include "ClintDash.h"
#include "ClintShoot.h"
_uint Clint::Clint_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Clint::Clint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Clint::Clint(const Clint& rhs)
	: CGameObject(rhs)
	, m_pClintStates(rhs.m_pClintStates)
	, m_pCurState(rhs.m_pCurState)
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

	// 상태 초기화
	Add_State(L"ClintIdle", ClintIdle::Create(m_pDevice, m_pContext, this));
	Add_State(L"ClintRun",  ClintRun::Create(m_pDevice, m_pContext, this));
	Add_State(L"ClintDash", ClintDash::Create(m_pDevice, m_pContext, this));
	Add_State(L"ClintShoot", ClintShoot::Create(m_pDevice, m_pContext, this));
	TransitionTo(L"ClintIdle");

	CGAMEOBJECT_DESC tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(CGAMEOBJECT_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&tCloneDesc.vPosition));

	return S_OK;
}

void Clint::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 렌더러 그룹에 추가
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	if (nullptr != m_pCurState)
		m_pCurState->OnStateTick(TimeDelta);

	// TransfomationMatirx의 값을 갱신하고 CombinedTransformationMatrix를 순차적으로 갱신
	m_pModelCom->Play_Animation(TimeDelta);

	if(nullptr != m_pColliderCom)
		m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	if (nullptr != m_pPistolaComL && nullptr != m_pPistolaComR)
	{
		m_pPistolaComL->Tick(TimeDelta);
		m_pPistolaComR->Tick(TimeDelta);
	}
}

void Clint::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

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
#endif
	 return S_OK;
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

void Clint::Add_State(const _tchar* pTag, ClintState* pState)
{
	if (nullptr != Find_State(pTag))
		assert(false);

	m_pClintStates.emplace(pTag, pState);
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

	ClintModel::CLINT_MODEL_DESC tModelDesc; tModelDesc.pOwner = this;
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

	Safe_Release(pGameInstance);
	return S_OK;
}

ClintState* Clint::Find_State(const _tchar* pTag)
{
	auto iter = find_if(m_pClintStates.begin(), m_pClintStates.end(), CTag_Finder(pTag));

	if (iter == m_pClintStates.end())
		return nullptr;

	return iter->second;
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

void Clint::TransitionTo(const _tchar* pTag)
{
	if(nullptr != m_pCurState)
		m_pCurState->OnStateExit();
	m_pCurState = Find_State(pTag);
	NULL_CHECK_RETURN(m_pCurState);
	m_pCurState->OnStateEnter();
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
	Safe_Release(m_pPistolaComL);
	Safe_Release(m_pPistolaComR);

	for (auto& Pair : m_pClintStates)
	{
		Safe_Release(Pair.second);
	}
	m_pClintStates.clear();

	/* Don't Forget Release for the VIBuffer or Model Component*/
}

