#include "Pistola.h"
#include "GameInstance.h"
#include "Model.h"
#include "Bone.h"
#include "ClintBasicBullet.h"

_uint Pistola::Pistola_Id = 0;

/* Don't Forget Release for the VIBuffer or Model Component*/

Pistola::Pistola(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_PivotMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

Pistola::Pistola(const Pistola& rhs)
	: CGameObject(rhs)
	, m_OffsetMatrix(rhs.m_OffsetMatrix)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT Pistola::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Pistola::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Pistola_Id;
	m_tInfo.wstrName = TO_WSTR("Pistola" + to_string(Pistola_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Pistola_Id;
	
	PISTOLA_DESC tPistolaDesc = *static_cast<PISTOLA_DESC*>(pArg);

	CModel* pModel = m_pOwner->GetComponent<CModel>();
	CTransform* pParentTransform = m_pOwner->GetComponent<CTransform>();
	CBone* pBone = pModel->GetBoneByName(tPistolaDesc.pAttachedBoneName); // hand_r, hand_l
	
	m_PivotMatrix = pModel->GetPivotMatrix();
	m_OffsetMatrix = pBone->Get_OffsetMatrix();
	m_pCombindTransformationMatrix = pBone->Get_CombinedTransformationMatrixPtr();
	m_pParentWorldMatrix = pParentTransform->Get_WorldFloat4x4Ptr();

	m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

	return S_OK;
}

void Pistola::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 렌더그룹 추가
	if(nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	AttachingWeapon();

	ReleaseIf([](ClintBasicBullet* pClintBasicBullet) {
		return pClintBasicBullet->GetDead();
		});

	for (auto& Bullet : m_Bullets)
		Bullet->Tick(TimeDelta);
}

void Pistola::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	for (auto& Bullet : m_Bullets)
		Bullet->Late_Tick(TimeDelta);
}

HRESULT Pistola::Render()
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

	// 만약에 모델 컴포넌트 안쓰면 이걸로 쓰면된다.
	// m_pShaderCom->Begin(0);

#ifdef _DEBUG
	// if(nullptr != m_pColliderCom)
	//	m_pColliderCom->Render();
#endif
}

void Pistola::Attack(_fvector vLook)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	ClintBasicBullet::CLINT_BASIC_BULLET_DESC tClintBasicBulletDesc;

	tClintBasicBulletDesc.pOwner = this;
	tClintBasicBulletDesc.vLook = vLook;
	XMStoreFloat4(&tClintBasicBulletDesc.vPosition, XMLoadFloat4x4(&m_WorldMatrix).r[3]);

	CGameObject* pBullet = pGameInstance->Clone_GameObject(ClintBasicBullet::ProtoTag(), &tClintBasicBulletDesc);
	PushBackBullet(static_cast<ClintBasicBullet*>(pBullet));
	Safe_Release(pGameInstance);
}

void Pistola::Save(HANDLE hFile, DWORD& dwByte)
{
	m_tInfo.Save(hFile, dwByte);
	m_pTransformCom->Save(hFile, dwByte);
}

void Pistola::Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	m_pTransformCom->Load(hFile, dwByte, iLevelIndex);
}

HRESULT Pistola::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	/* For.Com_Renderer */
	CRenderer::CRENDERER_DESC tRendererDesc; tRendererDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom, &tRendererDesc), E_FAIL);

	CTransform::CTRANSFORM_DESC TransformDesc{ 7.0, XMConvertToRadians(720.f) }; TransformDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CTransform::ProtoTag(), L"Com_Transform", (CComponent**)&m_pTransformCom
		, &TransformDesc), E_FAIL);

	CShader::CSHADER_DESC tShaderDesc; tShaderDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", (CComponent**)&m_pShaderCom, &tShaderDesc), E_FAIL);

	CModel::CMODEL_DESC tModelDesc; tModelDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, L"Prototype_Component_Model_Pistola", L"Com_Model", (CComponent**)&m_pModelCom, &tModelDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT Pistola::SetUp_ShaderResources()
{
	_float4x4 MyMatrix = m_WorldMatrix;
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

void Pistola::PushBackBullet(ClintBasicBullet* pClintBasicBullet)
{
	m_Bullets.push_back(pClintBasicBullet);
}

void Pistola::AttachingWeapon()
{
	_matrix BoneMatrix;

	BoneMatrix = XMLoadFloat4x4(&m_OffsetMatrix) *
		XMLoadFloat4x4(m_pCombindTransformationMatrix) *
		XMLoadFloat4x4(&m_PivotMatrix);

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() *
		BoneMatrix * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void Pistola::ReleaseIf(function<bool(ClintBasicBullet* pClintBasicBullet)> func)
{
	for (auto iter = m_Bullets.begin(); iter != m_Bullets.end();)
	{
		if (func(*iter))
		{
			Safe_Release(*iter);
			iter = m_Bullets.erase(iter);
		}
		else
			++iter;
	}
}

Pistola* Pistola::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Pistola* pInstance = new Pistola(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Pistola");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Pistola::Clone(void* pArg)
{
	Pistola* pInstance = new Pistola(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Pistola");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Pistola::Free(void)
{
	__super::Free();

	--Pistola_Id;
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/

	for (auto& Bullet : m_Bullets)
		Safe_Release(Bullet);
}