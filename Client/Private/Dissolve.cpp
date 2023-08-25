
#include "Dissolve.h"
#include "GameInstance.h"

_uint Dissolve::Dissolve_Id = 0;

Dissolve::Dissolve(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

Dissolve::Dissolve(const Dissolve& rhs)
	: CGameObject(rhs)
{
}

HRESULT Dissolve::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT Dissolve::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	++Dissolve_Id;
	m_tInfo.wstrName = TO_WSTR("Dissolve" + to_string(Dissolve_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = Dissolve_Id;

	tagDissolveDesc tCloneDesc;
	if (nullptr != pArg)
		tCloneDesc = *(tagDissolveDesc*)pArg;

	return S_OK;
}

HRESULT Dissolve::Bind_Values(CShader* pShader, _float fSumAmount, _float fEdgeThickness, _float4 vDissolveColor)
{
	m_fDissolveAmount += fSumAmount;

	// -0.01f인 이유는 0.f로하면 깔끔하게 다 안지워지기 때문에 상수값으로 -0.01f을 주었다.
	Saturate(m_fDissolveAmount, -0.01f - m_fEdgeThickness, 1.f);

	FAILED_CHECK_RETURN(m_pTextureCom->Bind_ShaderResource(pShader, "g_NoiseTexture"), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_fEdgeThickness", &m_fEdgeThickness, sizeof(_float)), E_FAIL);
	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_vDissolveColor", &m_vDissolveColor, sizeof(_float4)), E_FAIL);
}

void Dissolve::Reset()
{
	m_fDissolveAmount = 1.f;
}

HRESULT Dissolve::Add_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	LEVELID eLevelID = static_cast<LEVELID>(pGameInstance->Get_NextLevelIndex());

	CTexture::CTEXTURE_DESC tNoiseTextureDesc;
	tNoiseTextureDesc.pOwner = this;
	FAILED_CHECK_RETURN(__super::Add_Component(eLevelID, TEXT("Prototype_Component_Texture_T_ky_noise4"), L"Com_NoiseTexture", (CComponent**)&m_pTextureCom, &tNoiseTextureDesc), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

Dissolve* Dissolve::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Dissolve* pInstance = new Dissolve(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created Dissolve");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* Dissolve::Clone(void* pArg)
{
	Dissolve* pInstance = new Dissolve(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned Dissolve");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void Dissolve::Free(void)
{
	__super::Free();

	--Dissolve_Id;
	Safe_Release(m_pTextureCom);
	/* Don't Forget Release for the VIBuffer or Model Component*/
}

