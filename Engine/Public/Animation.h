#pragma once

#include "Model.h"

/* 특정(Idle, Attack, Run) 애니메이션을 구동하기위해 필요한 정보를 가지낟.  */
/* 구동하기위해 필요한 정보
: 사용하고있는 뼈의 갯수m_iNumChannels, 각 뼈가 시간에따라 어떤 상태를 취해야하는지?!
: 특정(Idle, Attack, Run) 애니메이션을 구동하는데 걸리는 총 시간. Duration
: 특정(Idle, Attack, Run) 애니메이션의 재생속도. TickPerSecond * TimeDelta
: 애니메이션의 이름.
*/

BEGIN(Engine)

class CAnimation final : public CBase
{
	friend class CModel;
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	char* GetName() {
		return m_szName;
	}
	_uint Get_NumChannels() {
		return m_Channels.size();
	}

	// 채널주소를 반환, 시간 복잡도O(1)
	class CChannel* Get_ChannelByIndex(_uint iIndex) {
		return m_Channels[iIndex];
	}
	_bool IsFinished() {
		return m_isFinished;
	}
	// 채널주소를 반환, 시간 복잡도O(n), 못찾으면 nullptr반환
	class CChannel* Get_ChannelByName(string strName);
public:
	void SaveAssimp(HANDLE hFile, DWORD& dwByte);
	void LoadAssimp(HANDLE hFile, DWORD& dwByte);

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const CModel::BONES& Bones);
	void Reset();
	void Invalidate_TransformationMatrix(CModel::BONES& Bones, _double TimeDelta);
	void InterAnimation_TransfomationMatrix(CModel::BONES& Bones, _double TimeAcc);

private:
	char						m_szName[MAX_PATH];
	_uint						m_iNumChannels = { 0 };
	vector<class CChannel*>		m_Channels;
	vector<_uint>				m_ChannelCurrentKeyFrames;
	_double						m_Duration = { 0.0 };
	_double						m_TickPerSecond = { 0.0 };
	_double						m_TimeAcc = { 0.0 };

	_bool						m_isFinished = { false };
	_bool						m_isLoop = { false };

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const CModel::BONES& Bones);
	CAnimation* Clone();
	virtual void Free();

};

END