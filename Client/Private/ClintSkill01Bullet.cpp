#include "ClintSkill01Bullet.h"
#include "GameInstance.h"

_uint ClintSkill01Bullet::ClintBasicBullet_Id = 0;

ClintSkill01Bullet::ClintSkill01Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Bullet(pDevice, pContext)
{
}

ClintSkill01Bullet::ClintSkill01Bullet(const ClintSkill01Bullet& rhs)
	: Bullet(rhs)
{
}

HRESULT ClintSkill01Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT ClintSkill01Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++ClintBasicBullet_Id;
	m_tInfo.wstrName = TO_WSTR("ClintSkill01Bullet" + to_string(ClintBasicBullet_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = ClintBasicBullet_Id;

	CLINT_BASIC_BULLET_DESC tClintBasicBulletDesc;
	if (nullptr != pArg)
		tClintBasicBulletDesc = *(CLINT_BASIC_BULLET_DESC*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, tClintBasicBulletDesc.vLook);
	//m_pModelCom->Set_RootNode(3);
	m_fDamage = 1000.f;
	return S_OK;
}

void ClintSkill01Bullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void ClintSkill01Bullet::Late_Tick(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	__super::Late_Tick(TimeDelta);
	if (pGameInstance->isIn_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 1.f))
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	Safe_Release(pGameInstance);
}

HRESULT ClintSkill01Bullet::Render()
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

#ifdef _DEBUG

#endif
}

HRESULT ClintSkill01Bullet::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh*/", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	//FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, /*L"Prototype_Component_Model_", L"Com_Model"*/, (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT ClintSkill01Bullet::SetUp_ShaderResources()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

ClintSkill01Bullet* ClintSkill01Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ClintSkill01Bullet* pInstance = new ClintSkill01Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created ClintSkill01Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Bullet* ClintSkill01Bullet::Clone(void* pArg)
{
	ClintSkill01Bullet* pInstance = new ClintSkill01Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ClintSkill01Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void ClintSkill01Bullet::Free(void)
{
	__super::Free();

	--ClintBasicBullet_Id;
	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pModelCom);

}

