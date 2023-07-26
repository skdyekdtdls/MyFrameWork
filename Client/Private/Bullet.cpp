#include "Bullet.h"
#include "GameInstance.h"

/* Don't Forget Release for the VIBuffer or Model Component*/

Bullet::Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Bullet::Bullet(const Bullet& rhs)
	: CGameObject(rhs)
	, m_fDamage(rhs.m_fDamage)
{
}

HRESULT Bullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if(nullptr != pArg)
		m_fDamage = reinterpret_cast<CLINT_BASIC_BULLET_DESC*>(pArg)->fDamage;

	return S_OK;
}

void Bullet::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	// 	m_pModelCom->Play_Animation(TimeDelta);
}

void Bullet::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);


}

HRESULT Bullet::Render()
{
	if (FAILED(__super::Render()))
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
}

void Bullet::Damage(CGameObject* pGameObejct)
{
	Health* pHealth = dynamic_cast<Health*>(pGameObejct->Get_Component(L"Com_Health"));

	if (pHealth)
		pHealth->TakeDamage(m_fDamage);
}

void Bullet::OnCollision(CCollider::COLLISION_INFO tCollisionInfo, _double TimeDelta)
{
	if (isMonsterLayer(tCollisionInfo.OtherGameObjectLayerName))
	{
		Damage(tCollisionInfo.pOtherGameObject);
	}
}

void Bullet::Free(void)
{
	__super::Free();

}

