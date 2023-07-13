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

class ENGINE_DLL CAnimation final : public CBase
{
	friend class CModel;
public:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public: // Getter
	char* GetName() {
		return m_szName;
	}

	_uint Get_NumChannels() {
		return m_Channels.size();
	}

	_float Get_Duration() {
		return m_Duration;
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

	_double* GetTickPerSecondPtr() {
		return &m_TickPerSecond;
	}
	_bool* GetIsLoopPtr() {
		return &m_isLoop;
	}

	_int* GetNextIndexPtr() {
		return &m_iNextIndex;
	}

public: // Setter
	void SetDuration(_double Duration) {
		m_Duration = Duration;
	}
public: /* For. Assimp */
	void SaveAssimp(HANDLE hFile, DWORD& dwByte);
	void LoadAssimp(HANDLE hFile, DWORD& dwByte);

public: /* For. Client*/
	void SaveData(HANDLE hFile, DWORD& dwByte);
	void LoadData(HANDLE hFile, DWORD& dwByte);

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const CModel::BONES& Bones);
	void Reset();
	void Invalidate_TransformationMatrix(CModel::BONES& Bones, _double TimeDelta, BODY eBody = BODY_END);
	void InterAnimation_TransfomationMatrix(CModel::BONES& Bones, _double TimeAcc, BODY eBody = BODY_END);
	
	// 타임라인 관리 함수들
	HRESULT Add_TimeLineEvent(const _tchar* pTag, TIMELINE_EVENT timeLineEvent);
	void Delete_TimeLineEvent(const _tchar* pTag);
	const TIMELINE_EVENT* Get_TimeLineEvent(const _tchar* pTag);

private:
	TIMELINE_EVENT* Find_TimeLine(const _tchar* pTag);

private:
	char						m_szName[MAX_PATH];
	_uint						m_iNumChannels = { 0 };
	vector<class CChannel*>		m_Channels;
	vector<_uint>				m_ChannelCurrentKeyFrames;
	_double						m_Duration = { 0.0 };
	_double						m_TickPerSecond = { 0.0 };
	_double						m_TimeAcc = { 0.0 };

	_bool						m_isFinished = { false };
	_bool						m_isLoop;
	_uint						m_iCurKeyFrame = { 0 };
	_int						m_iNextIndex = { -1 };

	unordered_map<const _tchar*, TIMELINE_EVENT>	m_TimeLineEvents;

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const CModel::BONES& Bones);
	CAnimation* Clone();
	virtual void Free();
};

END