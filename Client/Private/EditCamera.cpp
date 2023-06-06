#include "EditCamera.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "Cube.h"
CEditCamera::CEditCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CEditCamera::CEditCamera(const CEditCamera& rhs)
	: CCamera(rhs)
{
}

HRESULT CEditCamera::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEditCamera::Initialize(void* pArg)
{
	EDIT_CAMERA_DESC CameraFreeDesc = *(EDIT_CAMERA_DESC*)pArg;

	FAILED_CHECK_RETURN(__super::Initialize(&CameraFreeDesc.CameraDesc), E_FAIL);

	m_iData = CameraFreeDesc.iData;

	return S_OK;
}

void CEditCamera::Tick(_double TimeDelta)
{
	Key_Input(TimeDelta);
	Mouse_Input(TimeDelta);

	__super::Tick(TimeDelta);
}

void CEditCamera::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
}

HRESULT CEditCamera::Render()
{
	return S_OK;
}

void CEditCamera::Mouse_Input(_double TimeDelta)
{
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

	if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		CTerrain* pTerrain = static_cast<CTerrain*>(pGameInstance->Get_GameObject(LEVEL_IMGUI, L"Layer_BackGround", "Terrain"));
		Engine::PICK_DESC tPickDesc;
		if (pTerrain->Picked(tPickDesc))
		{
			CCube::CLONE_DESC tCloneDesc;
			tCloneDesc.vPosition = {tPickDesc.vPickPos};
			pGameInstance->Add_GameObject(LEVEL_IMGUI, CCube::ProtoTag(), L"Layer_BackGround", &tCloneDesc);
		}
	}

	Safe_Release(pGameInstance);
}

void CEditCamera::Key_Input(_double TimeDelta)
{
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
}

CEditCamera* CEditCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEditCamera* pInstance = new CEditCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Faield to Created CEditCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEditCamera::Clone(void* pArg)
{
	CEditCamera* pInstance = new CEditCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Faield to Cloned CEditCamera");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEditCamera::Free(void)
{
	__super::Free();
}
