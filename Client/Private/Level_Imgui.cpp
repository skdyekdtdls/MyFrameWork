#ifdef _USE_IMGUI
#include "Level_Imgui.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "EditCamera.h"
#include "Terrain.h"
#include "Cube.h"
CLevel_Imgui::CLevel_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Imgui::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	return S_OK;
}

void CLevel_Imgui::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);
}

void CLevel_Imgui::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);
	SetWindowText(g_hWnd, TEXT("���� �����Դϴ�."));
}

HRESULT CLevel_Imgui::Render()
{
	return S_OK;
}
HRESULT CLevel_Imgui::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLight::LIGHTDESC LightDesc;
	ZeroStruct(LightDesc);

	LightDesc.eType = { CLight::TYPE_DIRECTION };
	LightDesc.vDir = { 1.f, -1.f, 1.f, 0.f };
	LightDesc.vAmbient = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.vDiffuse = { 1.f, 1.f, 1.f, 1.f };
	LightDesc.vSpecular = { 1.f, 1.f, 1.f, 1.f };

	if (FAILED(pGameInstance->Add_Lights(LightDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Imgui::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_IMGUI, CTerrain::ProtoTag(), pLayerTag)))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_GameObject(LEVEL_IMGUI, CCube::ProtoTag(), pLayerTag)))
	//	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Imgui::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CEditCamera::EDIT_CAMERA_DESC Edit_Camera_Desc;

	Edit_Camera_Desc.iData = { 0 };
	Edit_Camera_Desc.CameraDesc.vEye = _float4(0.f, 20.f, -15.f, 1.f);
	Edit_Camera_Desc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Edit_Camera_Desc.CameraDesc.vUp = _float4(0.f, 1.f, 0.f, 0.f);
	Edit_Camera_Desc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	Edit_Camera_Desc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	Edit_Camera_Desc.CameraDesc.fNear = 0.2f;
	Edit_Camera_Desc.CameraDesc.fFar = 300.f;
	Edit_Camera_Desc.CameraDesc.TransformDesc.SpeedPerSec = 10.f;
	Edit_Camera_Desc.CameraDesc.TransformDesc.RotationPerSec = XMConvertToRadians(90.0f);

	FAILED_CHECK_RETURN(pGameInstance->Add_GameObject(LEVEL_IMGUI, CEditCamera::ProtoTag(), pLayerTag, &Edit_Camera_Desc), E_FAIL);

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Imgui* CLevel_Imgui::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Imgui* pInstance = new CLevel_Imgui(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CLevel_Imgui");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Imgui::Free()
{
	__super::Free();
}
#endif