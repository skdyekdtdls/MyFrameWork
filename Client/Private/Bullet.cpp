#include "Bullet.h"
#include "GameInstance.h"
#include "HP.h"
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
		m_fDamage = reinterpret_cast<tagBulletDesc*>(pArg)->fDamage;

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

void Bullet::Damage(CGameObject* pGameObejct)
{
	HP* pHP = dynamic_cast<HP*>(pGameObejct->Get_Component(L"Com_HP"));

	if (pHP)
		pHP->TakeDamage(static_cast<_uint>(m_fDamage));
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

