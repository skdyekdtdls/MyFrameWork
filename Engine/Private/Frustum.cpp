﻿#include "..\Public\Frustum.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(Frustum)

Frustum::Frustum()
{
}

HRESULT Frustum::Initialize()
{
	m_vOriginal_Points[0] = _float3(-1.f, 1.f, 0.f);
	m_vOriginal_Points[1] = _float3(1.f, 1.f, 0.f);
	m_vOriginal_Points[2] = _float3(1.f, -1.f, 0.f);
	m_vOriginal_Points[3] = _float3(-1.f, -1.f, 0.f);
	m_vOriginal_Points[4] = _float3(-1.f, 1.f, 1.f);
	m_vOriginal_Points[5] = _float3(1.f, 1.f, 1.f);
	m_vOriginal_Points[6] = _float3(1.f, -1.f, 1.f);
	m_vOriginal_Points[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void Frustum::Tick()
{
	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	Safe_AddRef(pPipeLine);

	_vector			vPoints[8];

	_matrix			ProjMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_PROJ);
	_matrix			ViewMatrixInv = pPipeLine->Get_TransformMatrix_Inverse(CPipeLine::D3DTS_VIEW);

	for (size_t i = 0; i < 8; i++)
	{
		vPoints[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vOriginal_Points[i]), ProjMatrixInv);
		vPoints[i] = XMVector3TransformCoord(vPoints[i], ViewMatrixInv);
	}


	XMStoreFloat4(&m_vWorldPlanes[0], XMPlaneFromPoints(vPoints[1], vPoints[5], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlanes[1], XMPlaneFromPoints(vPoints[4], vPoints[0], vPoints[3]));
	XMStoreFloat4(&m_vWorldPlanes[2], XMPlaneFromPoints(vPoints[4], vPoints[5], vPoints[1]));
	XMStoreFloat4(&m_vWorldPlanes[3], XMPlaneFromPoints(vPoints[3], vPoints[2], vPoints[6]));
	XMStoreFloat4(&m_vWorldPlanes[4], XMPlaneFromPoints(vPoints[5], vPoints[4], vPoints[7]));
	XMStoreFloat4(&m_vWorldPlanes[5], XMPlaneFromPoints(vPoints[0], vPoints[1], vPoints[2]));

	Safe_Release(pPipeLine);
}

_bool Frustum::isIn_WorldSpace(_fvector vWorldPos, _float fRange)
{
		for (size_t i = 0; i < 6; i++)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vWorldPlanes[i]), vWorldPos)))
			return false;
	}

	return true;
}

void Frustum::Free()
{
}