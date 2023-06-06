#include "..\Public\GameObject.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComposite(pDevice, pContext)
	, m_strName("")
{
}

CGameObject::CGameObject(const CGameObject& rhs)
	: CComposite(rhs)
	, m_strName(rhs.m_strName.c_str())
{
}

HRESULT CGameObject::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	return S_OK;
}

void CGameObject::Tick(_double TimeDelta)
{
}

void CGameObject::Late_Tick(_double TimeDelta)
{
}

void CGameObject::Free()
{
	__super::Free();
}
