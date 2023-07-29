#ifdef _DEBUG
#include "EditCamera.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "Cube.h"
#include "Fiona.h"
#include "Cell.h"
#include "ImWindow_Manager.h"
#include "ImWindow_ObjectTool.h"
#include "ImWindow_MapTool.h"
#include "ImMode.h"
#include "Layer.h"
#include "Model.h"
#include "Bone.h"
CEditCamera::CEditCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CEditCamera::CEditCamera(const CEditCamera& rhs)
	: CCamera(rhs)
{
}

PICK_DESC CEditCamera::GetMinDistPickDesc()
{
	if (m_tPickDescs.empty())
	{
		return PICK_DESC();
	}

	m_tPickDescs.sort([](PICK_DESC& tSour, PICK_DESC& tDest)
		{
			return tSour.fDist < tDest.fDist;
		});

	return m_tPickDescs.front();
}

PICK_DESC CEditCamera::GetTerrainPickDesc()
{
	auto TerrainDescIter = find_if(m_tPickDescs.begin(), m_tPickDescs.end(), [](const PICK_DESC& tPickDesc)
		{
			if (dynamic_cast<CTerrain*>(tPickDesc.pPickedObject))
				return true;
		});

	if (TerrainDescIter != m_tPickDescs.end())
	{
		return *TerrainDescIter;
	}
	else
	{
		return PICK_DESC();
	}
}

void CEditCamera::AddPickDesc(PICK_DESC tPickDesc)
{
	Safe_AddRef(tPickDesc.pPickedObject);
	m_tPickDescs.push_back(tPickDesc);
}

void CEditCamera::DeletePickDescByPtr(CGameObject* pGameObject)
{
	m_tPickDescs.remove_if([pGameObject](const PICK_DESC& desc) {
		return desc.pPickedObject == pGameObject;
		});

	Safe_Release(pGameObject);
}

void CEditCamera::ClearPickDesc()
{
	for (auto& iter : m_tPickDescs)
	{
		Safe_Release(iter.pPickedObject);
	}
	m_tPickDescs.clear();
}

HRESULT CEditCamera::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CEditCamera::Initialize(void* pArg)
{
	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

	m_tInfo.ID = 0;
	m_tInfo.wstrName = L"EditCamera";
	m_tInfo.wstrKey = ProtoTag();
	CTransform::CTRANSFORM_DESC tTransformDesc;
	tTransformDesc.SpeedPerSec = 40.f;
	tTransformDesc.RotationPerSec = XMConvertToRadians(90.f);
	m_pTransform->Set_Desc(tTransformDesc);

	return S_OK;
}

void CEditCamera::Tick(_double TimeDelta)
{
	switch (m_eEditMode)
	{
	case Client::CEditCamera::EDIT_MODE:
		EditMode_Tick(TimeDelta);
		break;
	case Client::CEditCamera::PLAY_MODE:
		PlayMode_Tick(TimeDelta);
		break;
	}

	__super::Tick(TimeDelta);
}

void CEditCamera::Late_Tick(_double TimeDelta)
{
	if (g_hWnd != ::GetFocus())
		return;

	switch (m_eEditMode)
	{
	case Client::CEditCamera::EDIT_MODE:
		EditMode_Late_Tick(TimeDelta);
		break;
	case Client::CEditCamera::PLAY_MODE:
		PlayMode_Late_Tick(TimeDelta);
		break;
	}

	__super::Late_Tick(TimeDelta);
}

HRESULT CEditCamera::Render()
{
	return S_OK;
}

void CEditCamera::EditMode_Tick(_double TimeDelta)
{
	ClearPickDesc();
	m_isPicking = { false };
	Make_MouseRay();

	if (g_hWnd == ::GetFocus())
	{
		Key_Input(TimeDelta);
		Mouse_Input(TimeDelta);
	}
}

void CEditCamera::PlayMode_Tick(_double TimeDelta)
{
	_vector ClintPos = Facade->GetClintPosition();
	_vector Offset = XMLoadFloat4(&m_OffsetPos);
	_vector OffsetPos;
	
	OffsetPos = ClintPos + Offset;
	
	m_pTransform->Set_State(CTransform::STATE_POSITION, OffsetPos);
	m_pTransform->LookAt(ClintPos);
}

void CEditCamera::EditMode_Late_Tick(_double TimeDelta)
{
}

void CEditCamera::PlayMode_Late_Tick(_double TimeDelta)
{
}

// 어떤 객체를 클릭했는지에 대한 정보를 만들어준다.
void CEditCamera::Picking()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 레이어 순환 바꾸기
	_bool bResult = { false };
	m_isPicking = { true };
	for (auto iter = pGameInstance->LayerBegin(LEVEL_IMGUI); iter != pGameInstance->LayerEnd(LEVEL_IMGUI); ++iter)
	{
		for (auto& GameObject : iter->second->GetGameObjects())
		{
			PICK_DESC tPickDesc;
			if (GameObject->Picked(tPickDesc, m_tMouseRay))
			{
				AddPickDesc(tPickDesc);
			}
		}
	}

	Safe_Release(pGameInstance);
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

	if (pGameInstance->Mouse_Down(CInput_Device::DIMK_LBUTTON))
	{
		Picking();
	}

	Safe_Release(pGameInstance);
}

void CEditCamera::Key_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_UPARROW))
	{
		m_pTransform->Go_Straight(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_DOWNARROW))
	{
		m_pTransform->Go_Backward(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_RIGHTARROW))
	{
		m_pTransform->Go_Right(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_LEFTARROW))
	{
		m_pTransform->Go_Left(TimeDelta);
	}
	Safe_Release(pGameInstance);
}


void CEditCamera::Make_MouseRay()
{
	::POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	D3D11_VIEWPORT ViewPort;
	UINT iNumViewPorts = 1;

	ZeroStruct(ViewPort);
	m_pContext->RSGetViewports(&iNumViewPorts, &ViewPort);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_float4x4 projMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);

	_float vx = (2.f * ptMouse.x / ViewPort.Width - 1.0f) / projMatrix.m[0][0];
	_float vy = (-2.f * ptMouse.y / ViewPort.Height + 1.0f) / projMatrix.m[1][1];

	_vector vRayOrigin = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = XMVectorSet(vx, vy, 1.f, 0.f);

	_matrix invView = pGameInstance->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	vRayOrigin = XMVector3TransformCoord(vRayOrigin, invView);
	vRayDir = XMVector3TransformNormal(vRayDir, invView);

	XMVector3Normalize(vRayDir);

	XMStoreFloat4(&m_tMouseRay.vRayOrigin, vRayOrigin);
	XMStoreFloat4(&m_tMouseRay.vRayDir, vRayDir);
	Safe_Release(pGameInstance);
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

	ClearPickDesc();
}
#endif