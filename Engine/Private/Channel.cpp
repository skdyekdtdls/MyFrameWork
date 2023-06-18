#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const NODE_ANIM* pChannel, const CModel::BONES Bones)
{
	strcpy_s(m_szName, pChannel->m_NodeName.m_data);

	// 모델이 들고 있는 같은 이름을 가진 뼈를 찾느다. 
	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pValue)
	{
		if (0 != strcmp(m_szName, pValue->Get_Name()))
		{
			++m_iBoneIndex;
			return false;
		}
		else
			return true;
	});

	m_iNumKeyFrames = max(pChannel->m_NumScalingKeys, pChannel->m_NumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pChannel->m_NumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME				Keyframe;

		if (pChannel->m_NumScalingKeys > i)
		{
			memcpy(&vScale, &pChannel->m_ScalingKeys[i].m_Value, sizeof(_float3));
			Keyframe.Time = pChannel->m_ScalingKeys[i].m_Time;
		}

		if (pChannel->m_NumRotationKeys > i)
		{
			vRotation.x = pChannel->m_RotationKeys[i].m_Value.x;
			vRotation.y = pChannel->m_RotationKeys[i].m_Value.y;
			vRotation.z = pChannel->m_RotationKeys[i].m_Value.z;
			vRotation.w = pChannel->m_RotationKeys[i].m_Value.w;			
			Keyframe.Time = pChannel->m_RotationKeys[i].m_Time;
		}

		if (pChannel->m_NumPositionKeys > i)
		{
			memcpy(&vTranslation, &pChannel->m_PositionKeys[i].m_Value, sizeof(_float3));
			Keyframe.Time = pChannel->m_PositionKeys[i].m_Time;
		}

		Keyframe.vScale = vScale;
		Keyframe.vRotation = vRotation;
		Keyframe.vTranslation = vTranslation;

		m_KeyFrames.push_back(Keyframe);
	}

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(CModel::BONES Bones, _double TimeAcc, _uint* pCurrentKeyFrameIndex)
{
	if (0.0 == TimeAcc)
		*pCurrentKeyFrameIndex = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_float3			vScale;
	_float4			vRotation;
	_float3			vTranslation;

	if (TimeAcc >= LastKeyFrame.Time)
	{
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vTranslation = LastKeyFrame.vTranslation;
	}
	else /* 현재 존재하는 키프레임의 상태를 좌우 키프레임정보를 이용하여 선형보간한다. */
	{
		while (TimeAcc >= m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].Time)
			++(*pCurrentKeyFrameIndex);

		_double		Ratio = (TimeAcc - m_KeyFrames[(*pCurrentKeyFrameIndex)].Time) /
			(m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].Time - m_KeyFrames[(*pCurrentKeyFrameIndex)].Time);

		_float3		vSourScale = m_KeyFrames[(*pCurrentKeyFrameIndex)].vScale;
		_float4		vSourRotation = m_KeyFrames[(*pCurrentKeyFrameIndex)].vRotation;
		_float3		vSourTranslation = m_KeyFrames[(*pCurrentKeyFrameIndex)].vTranslation;

		_float3		vDestScale = m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vScale;
		_float4		vDestRotation = m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vRotation;
		_float3		vDestTranslation = m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].vTranslation;

		XMStoreFloat3(&vScale, XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), (_float)Ratio));
		XMStoreFloat4(&vRotation, XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), (_float)Ratio));
		XMStoreFloat3(&vTranslation, XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), (_float)Ratio));
	}

	/* 진행된 시간에 맞는 뼈의 행렬을 만들어낸다. */
	_matrix		TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMLoadFloat3(&vTranslation));

	/* 같은 이름을 가진 모델이 들고 있는 뼈에게 전달해준다. */
	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

CChannel * CChannel::Create(const NODE_ANIM* pChannel, const CModel::BONES Bones)
{
	CChannel*	pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pChannel, Bones)))
	{
		MSG_BOX("Failed to Created CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
}
