#include "..\Public\PipeLine.h"
#include "Transform.h"
#include "VIBuffer_Terrain.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

_matrix CPipeLine::Get_TransformMatrix(D3DTRANSFORMSTATE eTransformState)
{
	return XMLoadFloat4x4(&m_TransformState[eTransformState]);
}

_float4x4 CPipeLine::Get_TransformFloat4x4(D3DTRANSFORMSTATE eTransformState)
{
	return m_TransformState[eTransformState];
}

_matrix CPipeLine::Get_TransformMatrix_Inverse(D3DTRANSFORMSTATE eTransformState)
{
	return XMLoadFloat4x4(&m_TransformState_Inverse[eTransformState]);
}

_float4x4 CPipeLine::Get_TransformFloat4x4_Inverse(D3DTRANSFORMSTATE eTransformState)
{
	return m_TransformState_Inverse[eTransformState];
}

_float4 CPipeLine::GetCamLookFloat4(_vector vTargetPos)
{
	_vector vCamPos = Get_CamPositionVector();
	_vector vCamLook = XMVector3Normalize(vTargetPos - vCamPos);
	_float4 vResultLook;
	XMStoreFloat4(&vResultLook, vCamLook);
	return vResultLook;
}

_float4 CPipeLine::GetCamLookFloat4()
{
	_float4 vCamLook;
	memcpy(&vCamLook, &m_CameraWorldMatrix.m[2][0], sizeof _float4);
	return vCamLook;
}

_vector CPipeLine::GetCamLookVector()
{
	_float4 vCamLook;
	memcpy(&vCamLook, &m_CameraWorldMatrix.m[2][0], sizeof _float4);
	return XMVector4Normalize(XMLoadFloat4(&vCamLook));
}

void CPipeLine::Tick()
{
	for (_uint i = 0; i < D3DTS_END; ++i)
	{
		XMStoreFloat4x4(&m_TransformState_Inverse[i], XMMatrixInverse(nullptr, Get_TransformMatrix(D3DTRANSFORMSTATE(i))));
	}

	memcpy(&m_CameraWorldMatrix, &m_TransformState_Inverse[D3DTS_VIEW], sizeof _float4x4);
}

void CPipeLine::Free()
{

}