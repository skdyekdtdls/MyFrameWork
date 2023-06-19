#include "..\Public\Camera_Free.h"
#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free& rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	CAMERAFREEDESC CameraFreeDesc = *(CAMERAFREEDESC*)pArg;

	FAILED_CHECK_RETURN(__super::Initialize(&CameraFreeDesc.CameraDesc), E_FAIL);

	m_iData = CameraFreeDesc.iData;

	return S_OK;
}

void CCamera_Free::Tick(_double TimeDelta)
{
	if (g_hWnd != ::GetFocus())
		return;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_pTransform->Go_Straight(TimeDelta);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pTransform->Go_Backward(TimeDelta);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pTransform->Go_Left(TimeDelta);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pTransform->Go_Right(TimeDelta);
	}
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_long		MouseMove = { 0 };

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMM_X))
	{
		m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * TimeDelta * 0.2f);
	}

	if (MouseMove = pGameInstance->Get_DIMouseMove(CInput_Device::DIMM_Y))
	{
		m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), MouseMove * TimeDelta * 0.2f);
	}

	Safe_Release(pGameInstance);
	__super::Tick(TimeDelta);
}

void CCamera_Free::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Faield to Created CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Faield to Cloned CCamera_Free");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCamera_Free::Free(void)
{
	__super::Free();
}
