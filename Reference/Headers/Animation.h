#pragma once


#pragma once

#include "Base.h"
#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const ANIMATION * pAnimation, const CModel::BONES & Bones);
	void Invalidate_TransformationMatrix(CModel::BONES & Bones, _double TimeDelta);

private:
	char			m_szName[MAX_PATH];
	_uint			m_iNumChannels = { 0 };
	vector<class CChannel*> m_Channels;
	vector<_uint>	m_ChannelCurrentKeyFrames;
	_double			m_Duration = { 0.0 };
	_double			m_TickPerSecond = { 0.0 };
	_double			m_TimeAcc = { 0.0 };

	_bool			m_isFinished = { false };
	_bool			m_isLoop = { false };

public:
	static CAnimation* Create(const ANIMATION * pAnimation, const CModel::BONES & Bones);
	CAnimation* Clone();
	virtual void Free() override;
};

END

