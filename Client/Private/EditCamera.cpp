#ifdef _USE_IMGUI
#include "EditCamera.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "Cube.h"
#include "ImWindow_Manager.h"
#include "ImWindow_Transform.h"
#include "ImWindow_Navigation.h"

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
	EDIT_CAMERA_DESC CameraFreeDesc;
	if (nullptr != pArg)
	{
		CameraFreeDesc = *(EDIT_CAMERA_DESC*)pArg;
	}
	FAILED_CHECK_RETURN(__super::Initialize(&CameraFreeDesc.CameraDesc), E_FAIL);

	m_iData = CameraFreeDesc.iData;
	
	Add_Components();

	m_tInfo.ID = 0;
	m_tInfo.wstrName = L"EditCamera";
	m_tInfo.wstrKey = ProtoTag();
	
	return S_OK;
}

void CEditCamera::Tick(_double TimeDelta)
{
	if (m_bStart)
	{
		m_bStart = false;
		m_pTerrain = static_cast<CTerrain*>(CGameInstance::GetInstance()->Get_GameObject(LEVEL_IMGUI, L"Layer_BackGround", "CTerrain1"));

		NULL_CHECK(m_pTerrain);
		Set_Terrain(m_pTerrain);
	}

	Make_MouseRay();

	if (g_hWnd == ::GetFocus())
	{
		Key_Input(TimeDelta);
		Mouse_Input(TimeDelta);
	}

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	__super::Tick(TimeDelta);
}

void CEditCamera::Late_Tick(_double TimeDelta)
{
	if (g_hWnd != ::GetFocus())
		return;

	__super::Late_Tick(TimeDelta);

	Late_Mouse_Input(TimeDelta);
}

HRESULT CEditCamera::Render()
{

	return S_OK;
}

void CEditCamera::Mouse_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImWindowInstance = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImWindowInstance);
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

	switch (pImWindowInstance->CurretMode())
	{
	case Client::OBJECT_PLACE_MODE:
		Object_Place(pGameInstance);
		break;
	case Client::NAVIGATION_MESH_MODE:
		Edit_Navigation_Mesh(pGameInstance);
		break;
	}


	Safe_Release(pImWindowInstance);
	Safe_Release(pGameInstance);
}

void CEditCamera::Key_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W))
	{
		m_pTransform->Go_Straight(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_S))
	{
		m_pTransform->Go_Backward(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_A))
	{
		m_pTransform->Go_Left(TimeDelta);
	}
	if (pGameInstance->Get_DIKeyState(DIK_D))
	{
		m_pTransform->Go_Right(TimeDelta);
	}
	Safe_Release(pGameInstance);
}

void CEditCamera::Late_Mouse_Input(_double TimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	CImWindow_Manager* pImWindowInstance = CImWindow_Manager::GetInstance();
	Safe_AddRef(pImWindowInstance);
	Safe_AddRef(pGameInstance);

	if(Client::TRANSFORM_MODE == pImWindowInstance->CurretMode())
		Edit_Transform(pGameInstance);

	Safe_Release(pImWindowInstance);
	Safe_Release(pGameInstance);
}

HRESULT CEditCamera::Add_Components()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_STATIC, CRenderer::ProtoTag(), L"Com_Renderer", (CComponent**)&m_pRendererCom), E_FAIL);
}

void CEditCamera::Object_Place(CGameInstance* pGameInstance)
{
	if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		Engine::PICK_DESC tPickDesc;
		if (m_pTerrain->Picked(tPickDesc, m_tMouseRay))
		{
			CCube::CLONE_DESC tCloneDesc;
			tCloneDesc.vPosition = { tPickDesc.vPickPos };
			pGameInstance->Add_GameObject(LEVEL_IMGUI, CCube::ProtoTag(), L"Layer_BackGround", &tCloneDesc);
		}
	}
}

void CEditCamera::Edit_Navigation_Mesh(CGameInstance* pGameInstance)
{
	CImWindow_Manager* pWindowMgr= CImWindow_Manager::GetInstance();
	Safe_AddRef(pWindowMgr);

	 CImWindow_Navigation* pImWindow_Transform = pWindowMgr->Get_ImWindow<CImWindow_Navigation>();

	 switch (pImWindow_Transform->m_eMode)
	 {
	 case CImWindow_Navigation::CREATE_MODE:
		 CreateTriangleStrip(pGameInstance);
		 break;
	 case CImWindow_Navigation::SELECT_POINT_MODE:
		 break;
	 case CImWindow_Navigation::SELECT_CELL_MODE:
		 break;
	 }

	Safe_Release(pWindowMgr);
}

void CEditCamera::Edit_Transform(CGameInstance* pGameInstance)
{
	if (pGameInstance->Get_DIMouseState(CInput_Device::DIMK_LBUTTON))
	{
		Engine::PICK_DESC tPickDesc;
		tPickDesc.fDist = FLT_MAX;
		_bool bResult = { false };
		for (_uint i = 0; i < CRenderer::RENDER_END; ++i)
		{
			for (auto& iter : m_pRendererCom->Get_RenderObjects(i))
			{
				if (iter->Picked(tPickDesc, m_tMouseRay))
				{
					bResult = true;
				}
			}
		}

		if (bResult)
		{
			CImWindow_Transform* pImWindow_Transform = static_cast<CImWindow_Transform*>(CImWindow_Manager::GetInstance()->Get_ImWindow(L"CImWindow_Transform"));
			pImWindow_Transform->Set_GameObject(tPickDesc.pPickedObject);
		}
	}
}

void CEditCamera::CreateTriangleStrip(CGameInstance* pGameInstance)
{
	if (pGameInstance->Mouse_Down(CInput_Device::DIMK_LBUTTON))
	{
		PICK_DESC tPickDesc;
		ZeroStruct(tPickDesc);

		if (m_pTerrain->Picked(tPickDesc, m_tMouseRay))
		{
			if (m_iClickCount == 2)
			{
				m_vClickPoint[m_iClickCount] = tPickDesc.vPickPos;
				m_iClickCount = 0;
				m_pTerrain->AddCell(m_vClickPoint);			
			}
			else
			{
				m_vClickPoint[m_iClickCount++] = tPickDesc.vPickPos;
			}
		}
	}
}

void CEditCamera::Make_MouseRay()
{
	POINT	ptMouse{};
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

void CEditCamera::Set_Terrain(CTerrain* pTerrain)
{
	m_pTerrain = pTerrain;

	CImWindow_Manager* pWindowMgr = CImWindow_Manager::GetInstance();
	Safe_AddRef(pWindowMgr);
	CImWindow_Navigation* pImWindow_Transform = pWindowMgr->Get_ImWindow<CImWindow_Navigation>();

	pImWindow_Transform->Set_Terrain(m_pTerrain);

	Safe_Release(pWindowMgr);
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

	Safe_Release(m_pTerrain);
	Safe_Release(m_pRendererCom);
}
#endif