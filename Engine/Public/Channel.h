#pragma once

#include "Base.h"
#include "Model.h"
BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const NODE_ANIM* pChannel, const CModel::BONES Bones);
	void Invalidate_TransformationMatrix(CModel::BONES Bones, _double TimeAcc, _uint* pCurrentKeyFrameIndex);

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;
	_uint				m_iBoneIndex = { 0 };

public:
	static CChannel* Create(const NODE_ANIM* pChannel, const CModel::BONES Bones);
	virtual void Free() override;
};

END