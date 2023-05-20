#include "GameInstance.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "GameObject.h"
#include "Object_Manager.h"
IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
{
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pObject_Manager);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHICDESC& GraphicDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iViewportSizeX, GraphicDesc.iViewportSizeY, ppDevice, ppDeviceContext)))
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Reserve_Containers(iNumLevels)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Tick(TimeDelta);
	m_pLevel_Manager->Late_Tick(TimeDelta);
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

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);

	return S_OK;
}

HRESULT CGameInstance::Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pObject_Manager->Add_Prototype(pPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	if (FAILED(m_pObject_Manager->Add_GameObject(iLevelIndex, pPrototypeTag, pLayerTag, pArg)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Clear_LevelResources(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear_LevelResources(iLevelIndex);
}

void CGameInstance::Release_Engine()
{
	CObject_Manager::DestroyInstance();
	CLevel_Manager::DestroyInstance();
	CGraphic_Device::DestroyInstance();
	CGameInstance::DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}