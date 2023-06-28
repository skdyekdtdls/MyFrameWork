#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
	: m_isLoop(true)
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	: m_iNumChannels(rhs.m_iNumChannels)
	, m_Channels(rhs.m_Channels)
	, m_ChannelCurrentKeyFrames(rhs.m_ChannelCurrentKeyFrames)
	, m_Duration(rhs.m_Duration)
	, m_TickPerSecond(rhs.m_TickPerSecond)
	, m_TimeAcc(rhs.m_TimeAcc)
	, m_isFinished(rhs.m_isFinished)
	, m_isLoop(rhs.m_isLoop)
{
	strcpy_s(m_szName, rhs.m_szName);

	for (auto& pChannel : m_Channels)
		Safe_AddRef(pChannel);
}

void CAnimation::SaveAssimp(HANDLE hFile, DWORD& dwByte)
{
	WriteVoid(&m_szName[0], sizeof(char) * MAX_PATH);
	WriteVoid(&m_iNumChannels, sizeof(_uint));
	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->SaveAssimp(hFile, dwByte);
	}

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		WriteVoid(&m_ChannelCurrentKeyFrames[i], sizeof(_uint));
	}
	WriteVoid(&m_Duration, sizeof(_double));
	WriteVoid(&m_TickPerSecond, sizeof(_double));
	WriteVoid(&m_TimeAcc, sizeof(_double));

	WriteVoid(&m_isFinished, sizeof(_bool));
	WriteVoid(&m_isLoop, sizeof(_bool));
}

void CAnimation::LoadAssimp(HANDLE hFile, DWORD& dwByte)
{
	ReadVoid(&m_szName[0], sizeof(char) * MAX_PATH);
	ReadVoid(&m_iNumChannels, sizeof(_uint));

	m_Channels.resize(m_iNumChannels);
	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i] = new CChannel;
		m_Channels[i]->LoadAssimp(hFile, dwByte);
	}

	m_ChannelCurrentKeyFrames.resize(m_iNumChannels);
	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		ReadVoid(&m_ChannelCurrentKeyFrames[i], sizeof(_uint));
	}

	ReadVoid(&m_Duration, sizeof(_double));
	ReadVoid(&m_TickPerSecond, sizeof(_double));
	ReadVoid(&m_TimeAcc, sizeof(_double));

	ReadVoid(&m_isFinished, sizeof(_bool));
	ReadVoid(&m_isLoop, sizeof(_bool));
}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const CModel::BONES& Bones)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	m_Duration = pAIAnimation->mDuration;
	m_TickPerSecond = pAIAnimation->mTicksPerSecond;

	m_iNumChannels = pAIAnimation->mNumChannels;

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}

	m_ChannelCurrentKeyFrames.resize(m_iNumChannels);

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(CModel::BONES& Bones, _double TimeDelta)
{
	m_TimeAcc += m_TickPerSecond * TimeDelta;

	if (m_TimeAcc >= m_Duration)
	{
		if (true == m_isLoop)
		{
			m_TimeAcc = 0.f;
		}
		else
			m_isFinished = true;
	}



	/* 현재 재생된 시간에 맞도록 모든 뼈의 상태를 키프레임정보를 기반으로하여 갱신한다. */
	_uint		iChannelIndex = 0;
	for (auto& pChannel : m_Channels)
	{
		if (nullptr == pChannel)
			return;

		pChannel->Invalidate_TransformationMatrix(Bones, m_TimeAcc, &m_ChannelCurrentKeyFrames[iChannelIndex++]);
	}


}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const CModel::BONES& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, Bones)))
	{
		MSG_BOX("Failed to Created CAnimation");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	m_Channels.clear();

}