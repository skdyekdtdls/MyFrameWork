#include "GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "GameObject.h"
#include "Object_Manager.h"
#include "Timer_Manager.h"
#include "Component_Manager.h"
#include "Frustum.h"
#include "Layer.h"
#include "CollisionMgr.h"
#include "Target_Manager.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pFrustum(Frustum::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pCollision_Manager(CollisionMgr::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pFont_Manager(CFont_Manager::GetInstance())
{
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pCollision_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pFrustum);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL; 

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iViewportSizeX, GraphicDesc.iViewportSizeY, ppDevice, ppDeviceContext)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_DInput(GraphicDesc.hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	m_pLight_Manager->Initialize(*ppDevice, *ppDeviceContext);
	m_pFrustum->Initialize();

	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager ||
		nullptr == m_pObject_Manager ||
		nullptr == m_pPipeLine ||
		nullptr == m_pInput_Device)
		return;

	m_pInput_Device->Update_DInput();
	m_pObject_Manager->Tick(TimeDelta);
	m_pPipeLine->Tick();
	m_pFrustum->Tick();
	m_pObject_Manager->Late_Tick(TimeDelta);
	m_pCollision_Manager->Late_Tick(TimeDelta);
	m_pLevel_Manager->Tick(TimeDelta);
	m_pLevel_Manager->Late_Tick(TimeDelta);
}

void CGameInstance::Clear_LevelResources(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager ||
		nullptr == m_pComponent_Manager)
		return;

	m_pObject_Manager->Clear_LevelResources(iLevelIndex);
	m_pComponent_Manager->Clear_LevelResources(iLevelIndex);
}

CLayer* CGameInstance::Find_LayerByName(_uint iLevelIndex, const _tchar* pLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Find_LayerByName(iLevelIndex, pLayerTag);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, string strName)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	CLayer* pLayer = m_pObject_Manager->Find_LayerByName(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	CGameObject* pGameObject = pLayer->FindByName(strName);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

CComponent* CGameInstance::Get_ComponentOfClone(_uint iLevelIndex, const _tchar* pLayerTag, string pCloneObjName, const _tchar* pCloneComName)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_ComponentOfClone(iLevelIndex, pLayerTag, pCloneObjName, pCloneComName);
}

_uint CGameInstance::GetNumLayers(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return 0;

	return m_pObject_Manager->GetNumLayers(iLevelIndex);
}

LAYERS* CGameInstance::GetLayers()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->GetLayers();
}

CGameObject* CGameInstance::Clone_GameObject(const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Clone_GameObject(pPrototypeTag, pArg);
}

void CGameInstance::AddToLayer(_uint iLevelIndex, const _tchar* pLayerTag, CGameObject* pGameObject)
{
	if (nullptr == m_pObject_Manager)
		return;

	return m_pObject_Manager->AddToLayer(iLevelIndex, pLayerTag, pGameObject);
}

unordered_map<const _tchar*, CLayer*>::iterator CGameInstance::LayerBegin(_uint iLevelIndex)
{
	return m_pObject_Manager->LayerBegin(iLevelIndex);
}

unordered_map<const _tchar*, CLayer*>::iterator CGameInstance::LayerEnd(_uint iLevelIndex)
{
	return m_pObject_Manager->LayerEnd(iLevelIndex);
}

list<CGameObject*>::iterator CGameInstance::GetLayerBegin(_uint iLevelIndex, const _tchar* pTag)
{
	return m_pObject_Manager->GetLayerBegin(iLevelIndex, pTag);
}

list<CGameObject*>::iterator CGameInstance::GetLayerEnd(_uint iLevelIndex, const _tchar* pTag)
{
	return m_pObject_Manager->GetLayerEnd(iLevelIndex, pTag);
}

void CGameInstance::Serialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Serialization(hFile, dwByte, iLevelIndex);
}

void CGameInstance::Deserialization(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Deserialization(hFile, dwByte, iLevelIndex);
}

void CGameInstance::ResizeBuffer(_uint& g_ResizeWidth, _uint& g_ResizeHeight)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->ResizeBuffer(g_ResizeWidth, g_ResizeHeight);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}

_uint2 CGameInstance::GetViewPortSize(ID3D11DeviceContext* pContext)
{
	_uint2 WinSize;

	_uint iNumViews = { 1 };
	D3D11_VIEWPORT ViewPortDesc;
	pContext->RSGetViewports(&iNumViews, &ViewPortDesc);

	WinSize.x = ViewPortDesc.Width;
	WinSize.y = ViewPortDesc.Height;

	return WinSize;
}

_byte CGameInstance::Get_DIKeyState(_ubyte ubyKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(ubyKeyID);
}

_byte CGameInstance::Get_DIMouseState(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(eMouseID);
}

_long CGameInstance::Get_DIMouseMove(CInput_Device::MOUSEMOVESTATE eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMove(eMouseMoveID);
}

bool CGameInstance::Mouse_Down(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Down(eMouseID);
}

bool CGameInstance::Mouse_Pressing(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Pressing(eMouseID);
}

bool CGameInstance::Mouse_Up(CInput_Device::MOUSEKEYSTATE eMouseID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Up(eMouseID);
}

bool CGameInstance::Key_Pressing(_ubyte ubyKey)
{
	if (nullptr == m_pInput_Device)
		return false;
	return m_pInput_Device->Key_Pressing(ubyKey);
}

bool CGameInstance::Key_Down(_ubyte ubyKey)
{
	if (nullptr == m_pInput_Device)
		return false;
	return m_pInput_Device->Key_Down(ubyKey);
}

bool CGameInstance::Key_Up(_ubyte ubyKey)
{
	if (nullptr == m_pInput_Device)
		return false;
	return m_pInput_Device->Key_Up(ubyKey);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

const _uint& CGameInstance::Get_CurLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurLevelIndex();
}

const _uint& CGameInstance::Get_NextLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_NextLevelIndex();
}

void CGameInstance::Set_NextLevelIndex(_uint iIndex)
{
	if (nullptr == m_pLevel_Manager)
		return;

	return m_pLevel_Manager->Set_NextLevelIndex(iIndex);
}

HRESULT CGameInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, string strName)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_GameObject(iLevelIndex, pLayerTag, strName);
}

_double CGameInstance::Get_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0;

	return m_pTimer_Manager->Get_Timer(pTimerTag);
}

void CGameInstance::Set_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return;

	m_pTimer_Manager->Set_Timer(pTimerTag);
}

HRESULT CGameInstance::Ready_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Ready_Timer(pTimerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

CComponent* CGameInstance::Get_ProtoComponent(_uint iLevelIndex, const _tchar* pProtoTag)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Get_ProtoComponent(iLevelIndex, pProtoTag);
}

_float4 CGameInstance::Get_CamPositionFloat4() const
{
	return m_pPipeLine->Get_CamPositionFloat4();
}

_vector CGameInstance::Get_CamPositionVector()
{
	return m_pPipeLine->Get_CamPositionVector();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eTransformState, _fmatrix TransformStateMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eTransformState, TransformStateMatrix);
}

_matrix CGameInstance::Get_TransformMatrix(CPipeLine::D3DTRANSFORMSTATE eTransformState)
{
	if (nullptr == m_pPipeLine)
		return _matrix();

	return m_pPipeLine->Get_TransformMatrix(eTransformState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4(CPipeLine::D3DTRANSFORMSTATE eTransformState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4(eTransformState);
}

_matrix CGameInstance::Get_TransformMatrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState)
{
	if (nullptr == m_pPipeLine)
		return _matrix();

	return m_pPipeLine->Get_TransformMatrix_Inverse(eTransformState);
}

_float4x4 CGameInstance::Get_TransformFloat4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eTransformState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_TransformFloat4x4_Inverse(eTransformState);
}

_float4 CGameInstance::GetCamLookFloat4(_vector vTargetPos)
{
	return m_pPipeLine->GetCamLookFloat4(vTargetPos);
}

_float4 CGameInstance::GetCamLookFloat4()
{
	return m_pPipeLine->GetCamLookFloat4();
}

_vector CGameInstance::GetCamLookVector()
{
	return m_pPipeLine->GetCamLookVector();
}

_bool CGameInstance::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldSpace(vWorldPos, fRange);
}

const CLight::LIGHTDESC* CGameInstance::Get_Light(_uint iIndex)
{
	if(nullptr == m_pLight_Manager)
		return nullptr;

	m_pLight_Manager->Get_Light(iIndex);
}

HRESULT CGameInstance::Add_Lights(const CLight::LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Lights(LightDesc);
}

void CGameInstance::Add_ColliderGroup(CCollider* pCollider, COLL_GROUP eCollGroup)
{
	if (nullptr == m_pCollision_Manager)
		return;

	m_pCollision_Manager->Add_ColliderGroup(pCollider, eCollGroup);
}

HRESULT CGameInstance::Add_Fonts(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontTag, const _tchar* pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Fonts(pDevice, pContext, pFontTag, pFontFilePath);
}

HRESULT CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, float fRotation, const _float2& vOrigin, _float fScale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render_Font(pFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}

void CGameInstance::Release_Engine()
{
	CTimer_Manager::DestroyInstance();
	CFont_Manager::DestroyInstance();
	CTarget_Manager::DestroyInstance();
	CInput_Device::DestroyInstance();
	CObject_Manager::DestroyInstance();
	CComponent_Manager::DestroyInstance();
	CLight_Manager::DestroyInstance();
	CLevel_Manager::DestroyInstance();
	CGraphic_Device::DestroyInstance();
	Frustum::DestroyInstance();
	CPipeLine::DestroyInstance();
	CollisionMgr::DestroyInstance();
	CGameInstance::DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}