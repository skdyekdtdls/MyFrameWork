
#include "RimLight.h"
#include "GameInstance.h"

_uint RimLight::RimLight_Id = 0;

RimLight::RimLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

RimLight::RimLight(const RimLight& rhs)
	: CGameObject(rhs)
{
}

HRESULT RimLight::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}


HRESULT RimLight::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	++RimLight_Id;
	m_tInfo.wstrName = TO_WSTR("RimLight" + to_string(RimLight_Id));
	m_tInfo.wstrKey = ProtoTag();
	m_tInfo.ID = RimLight_Id;

	return S_OK;
}

HRESULT RimLight::Bind_Values(CShader* pShader, _float fRimPower, _float4 vRimColor)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_fRimPower", &fRimPower, sizeof(_float)), E_FAIL);

	_float4 vCamPos = pGameInstance->Get_CamPositionFloat4();
	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_vRimPower", &vCamPos, sizeof(_float4)), E_FAIL);

	FAILED_CHECK_RETURN(pShader->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4)), E_FAIL);

	Safe_Release(pGameInstance);
	return S_OK;
}

RimLight* RimLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	RimLight* pInstance = new RimLight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created RimLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* RimLight::Clone(void* pArg)
{
	RimLight* pInstance = new RimLight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned RimLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void RimLight::Free(void)
{
	__super::Free();

	--RimLight_Id;
	/* Don't Forget Release for the VIBuffer or Model Component*/
}