#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const NODE_ANIM* pChannel, const CModel::BONES Bones)
{
	strcpy_s(m_szName, pChannel->m_NodeName.m_data);

	auto iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pValue)
		{
			if (0 != strcmp(m_szName, pValue->Get_Name()))
			{
				++m_iBoneIndex;
				return false;
			}
			else
				return true;
		});
	
	m_iNumKeyFrames = max(max(pChannel->m_NumPositionKeys, pChannel->m_NumRotationKeys), pChannel->m_NumScalingKeys);
	
	_float3	vScale;
	_float4 vRotation;
	_float3 vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME	KeyFrame;

		if (pChannel->m_NumScalingKeys > i)
		{
			memcpy(&vScale, &pChannel->m_ScalingKeys[i].m_Value, sizeof(_float3));
			KeyFrame.Time = pChannel->m_ScalingKeys[i].m_Time;
		}

		if (pChannel->m_NumRotationKeys > i)
		{
			vRotation.x = pChannel->m_RotationKeys[i].m_Value.x;
			vRotation.y = pChannel->m_RotationKeys[i].m_Value.y;
			vRotation.z = pChannel->m_RotationKeys[i].m_Value.z;
			vRotation.w = pChannel->m_RotationKeys[i].m_Value.w;
			KeyFrame.Time = pChannel->m_RotationKeys[i].m_Time;
		}

		if (pChannel->m_NumPositionKeys > i)
		{
			memcpy(&vTranslation, &pChannel->m_PositionKeys[i].m_Value, sizeof(_float3));
			KeyFrame.Time = pChannel->m_PositionKeys[i].m_Time;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPos = vTranslation;

		m_KeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(CModel::BONES Bones, _double TimeAcc, _uint* pCurrentKeyFrameIndex)
{
	if (0.0 == TimeAcc)
		*pCurrentKeyFrameIndex = 0;

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector vScale;
	_vector vRotation;
	_vector vTranslation;
	
	if (TimeAcc >= LastKeyFrame.Time)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMLoadFloat3(&LastKeyFrame.vPos);
	}
	else
	{
		while (TimeAcc >= m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].Time)
			++(*pCurrentKeyFrameIndex);

		KEYFRAME CurrentKeyFrame = m_KeyFrames[*pCurrentKeyFrameIndex];
		KEYFRAME NextKeyFrame = m_KeyFrames[(*pCurrentKeyFrameIndex) + 1];		

		_double Ratio = (TimeAcc - CurrentKeyFrame.Time) /
			(NextKeyFrame.Time - CurrentKeyFrame.Time);

		_float3 vSourScale = CurrentKeyFrame.vScale;
		_float4 vSourRotation = CurrentKeyFrame.vRotation;
		_float3 vSourPos = CurrentKeyFrame.vPos;

		_float3 vDestScale = NextKeyFrame.vScale;
		_float4 vDestRotation = NextKeyFrame.vRotation;
		_float3 vDestPos = NextKeyFrame.vPos;
		
		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), Ratio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), Ratio);
		vTranslation = XMVectorLerp(XMLoadFloat3(&vSourPos), XMLoadFloat3(&vDestPos), Ratio);
	}
	
	_matrix	TransformationMatrix = XMMatrixAffineTransformation(
		vScale
		, XMVectorSet(0.f, 0.f, 0.f, 1.f)
		, vRotation
		, vTranslation
	);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(const NODE_ANIM* pChannel, const CModel::BONES Bones)
{
	CChannel* pInstance = new CChannel();

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