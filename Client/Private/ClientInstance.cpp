#include "ClientInstance.h"
#include "GameInstance.h"
#include "Clint.h"
#include "Terrain.h"
IMPLEMENT_SINGLETON(ClientInstance)

_vector ClientInstance::GetClintPosition()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CTransform* pTransform = static_cast<CTransform*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Transform"));

	_vector vClintPos = pTransform->Get_State(CTransform::STATE_POSITION);
	Safe_Release(pGameInstance);

	return vClintPos;
}

Clint* ClientInstance::GetClint()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	Clint* pClint = static_cast<Clint*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1"));
	
	Safe_Release(pGameInstance);
	return pClint;
}

CColliderAABB* ClientInstance::GetClintBodyColl()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance); 
	CColliderAABB* pCollAABB = static_cast<CColliderAABB*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_BodyColl"));
	Safe_Release(pGameInstance);

	return pCollAABB;
}

CTerrain* ClientInstance::GetTerrain()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CTerrain* pTerrain = static_cast<CTerrain*>(pGameInstance->Get_GameObject(pGameInstance->Get_CurLevelIndex(), L"Layer_BackGround", "CTerrain1"));
	Safe_Release(pGameInstance);
	return pTerrain;
}

_vector ClientInstance::PickPosOnTerrain()
{
	CTerrain* pTerrain = GetTerrain();
	return pTerrain->PickPos();
}

RAY ClientInstance::GetMouseRay(ID3D11DeviceContext* pContext)
{
	::POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	D3D11_VIEWPORT ViewPort;
	UINT iNumViewPorts = 1;
	RAY  tMouseRay;
	ZeroStruct(ViewPort);
	pContext->RSGetViewports(&iNumViewPorts, &ViewPort);

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

	XMStoreFloat4(&tMouseRay.vRayOrigin, vRayOrigin);
	XMStoreFloat4(&tMouseRay.vRayDir, vRayDir);
	Safe_Release(pGameInstance);

	return tMouseRay;
}

// 렌더러가 nullptr이 아니고 카메라 안에 들어오면 렌더함.
_bool ClientInstance::isRender(CRenderer* pRenderer, CTransform* pTransform, _float fRange)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr != pRenderer && true == pGameInstance->isIn_WorldSpace(pTransform->Get_State(CTransform::STATE_POSITION), fRange))
	{
		Safe_Release(pGameInstance);
		return true;
	}

	Safe_Release(pGameInstance);
	return false;
}

CNavigation* ClientInstance::GetClintNavigation()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CNavigation* pNavigation = static_cast<CNavigation*>(pGameInstance->Get_ComponentOfClone(pGameInstance->Get_CurLevelIndex(), L"Layer_Player", "Clint1", L"Com_Navigation"));
	Safe_Release(pGameInstance);

	return pNavigation;
}

void ClientInstance::Free(void)
{
}