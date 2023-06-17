#include "Animation.h"
#include "Channel.h"
CAnimation::CAnimation()
	: m_isLoop(true)
{
}

HRESULT CAnimation::Initialize(const ANIMATION* pAnimation, const CModel::BONES& Bones)
{
	strcpy_s(m_szName, pAnimation->m_Name.m_data);
	m_Duration = pAnimation->m_Duration;
	m_TickPerSecond = pAnimation->m_TicksPerSecond;

	m_iNumChannels = pAnimation->m_NumChannels;

	for (size_t i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(&pAnimation->m_Channels[i], Bones);
		NULL_CHECK_RETURN(pChannel, E_FAIL);
		
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
			m_isFinished = { true };
	}
	_uint iChannelIndex = 0;

	for (auto& pChannel : m_Channels)
	{
		if (nullptr == pChannel)
			return;
		
		pChannel->Invalidate_TransformationMatrix(Bones, m_TimeAcc, &m_ChannelCurrentKeyFrames[iChannelIndex++]);
	}
}

CAnimation* CAnimation::Create(const ANIMATION* pAnimation, const CModel::BONES& Bones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAnimation, Bones)))
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
}

