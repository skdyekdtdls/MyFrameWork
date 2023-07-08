#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

void CChannel::SaveAssimp(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_szName[0], sizeof(char) * MAX_PATH);
	WriteVoid(&m_iNumKeyFrames, sizeof(_uint));

	for(size_t i = 0 ; i < m_iNumKeyFrames; ++i)
		WriteVoid(&m_KeyFrames[i], sizeof(KEYFRAME));

	WriteVoid(&m_iBoneIndex, sizeof(_uint));
}

void CChannel::LoadAssimp(HANDLE hFile, DWORD& dwByte)
{
	ReadVoid(&m_szName[0], sizeof(char) * MAX_PATH);
	ReadVoid(&m_iNumKeyFrames, sizeof(_uint));

	m_KeyFrames.resize(m_iNumKeyFrames);
	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
		ReadVoid(&m_KeyFrames[i], sizeof(KEYFRAME));

	ReadVoid(&m_iBoneIndex, sizeof(_uint));
}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, const CModel::BONES& Bones)
{
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	// 모델이 들고 있는 같은 이름을 가진 뼈를 찾느다. 
	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pValue)
		{
			if (0 != strcmp(m_szName, pValue->GetName()))
			{
				++m_iBoneIndex;
				return false;
			}
			else
				return true;
		});

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vTranslation;

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		KEYFRAME				Keyframe;

		if (pAIChannel->mNumScalingKeys > i)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			Keyframe.Time = pAIChannel->mScalingKeys[i].mTime;
		}

		if (pAIChannel->mNumRotationKeys > i)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			Keyframe.Time = pAIChannel->mRotationKeys[i].mTime;
		}

		if (pAIChannel->mNumPositionKeys > i)
		{
			memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			Keyframe.Time = pAIChannel->mPositionKeys[i].mTime;
		}

		Keyframe.vScale = vScale;
		Keyframe.vRotation = vRotation;
		Keyframe.vTranslation = vTranslation;

		m_KeyFrames.push_back(Keyframe);
	}

	return S_OK;
}

_uint CChannel::Invalidate_TransformationMatrix(CModel::BONES& Bones, _double TimeAcc, _uint* pCurrentKeyFrameIndex, BODY eBody)
{
	if (BODY_END != eBody && Bones[m_iBoneIndex]->Get_Body() != eBody)
		return 0;

	if (0.0 == TimeAcc)
		*pCurrentKeyFrameIndex = { 0 };

	KEYFRAME LastKeyFrame = m_KeyFrames.back();

	_float3			vScale;
	_float4			vRotation;
	_float3			vTranslation;

	_matrix TransformationMatrix = XMMatrixIdentity();
	_double Ratio = 0.0;

	Ratio = (TimeAcc - m_KeyFrames[(*pCurrentKeyFrameIndex)].Time) /
		(m_KeyFrames[(*pCurrentKeyFrameIndex) + 1].Time - m_KeyFrames[(*pCurrentKeyFrameIndex)].Time);

	if (TimeAcc >= LastKeyFrame.Time)
	{
		vScale = LastKeyFrame.vScale;
		vRotation = LastKeyFrame.vRotation;
		vTranslation = LastKeyFrame.vTranslation;
	}
	else
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

	// 애니메이션 전환 시 블랜딩을 하기 위해서 현재 상태 행렬의 키프레임 정보를 뼈에 기록한다.
	KEYFRAME tCurKeyFrameState;
	tCurKeyFrameState.vScale = vScale;
	tCurKeyFrameState.vRotation = vRotation;
	tCurKeyFrameState.vTranslation = vTranslation;
	tCurKeyFrameState.Time = TimeAcc - m_KeyFrames[(*pCurrentKeyFrameIndex)].Time;
	Bones[m_iBoneIndex]->Set_CurKeyFrameState(tCurKeyFrameState);

	// 상태 행렬을 만들기 위해서 계산한 SRT로부터 아핀변환을 한다.
	TransformationMatrix = XMMatrixAffineTransformation(XMLoadFloat3(&vScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation), XMLoadFloat3(&vTranslation));

	// 상태 행렬을 뼈에 기록한다.
	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

	return *pCurrentKeyFrameIndex;
}

void CChannel::InterAnimation_TransfomationMatrix(CModel::BONES& Bones, _double TimeAcc, BODY eBody)
{
	if (BODY_END != eBody && Bones[m_iBoneIndex]->Get_Body() != eBody)
		return;

	_double Ratio = TimeAcc / 0.2;

	KEYFRAME FrontKeyFrame = m_KeyFrames.front();
	KEYFRAME PrevKeyFrame = Bones[m_iBoneIndex]->Get_CurKeyFrameState();

	_vector SourScale, DestScale;
	_vector SourRotation, DestRotation;
	_vector SourTranslation, DestTranslation;
	// 이전 키프레임
	SourScale = XMLoadFloat3(&PrevKeyFrame.vScale);
	DestScale = XMLoadFloat3(&FrontKeyFrame.vScale);

	SourRotation = XMLoadFloat4(&PrevKeyFrame.vRotation);
	DestRotation = XMLoadFloat4(&FrontKeyFrame.vRotation);

	SourTranslation = XMLoadFloat3(&PrevKeyFrame.vTranslation);
	DestTranslation = XMLoadFloat3(&FrontKeyFrame.vTranslation);

	_vector Scale = XMVectorLerp(SourScale, DestScale, Ratio);
	_vector Rotation = XMQuaternionSlerp(SourRotation, DestRotation, Ratio);
	_vector Translation = XMVectorLerp(SourTranslation, DestTranslation, Ratio);
	_matrix TransformationMatrix = XMMatrixAffineTransformation(Scale, XMVectorZero(), Rotation, Translation);
	Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(const aiNodeAnim* pAIChannel, const CModel::BONES& Bones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, Bones)))
	{
		MSG_BOX("Failed to Created CChannel");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CChannel::Free()
{
}
