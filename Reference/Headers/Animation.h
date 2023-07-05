#pragma once

#include "Model.h"

/* Ư��(Idle, Attack, Run) �ִϸ��̼��� �����ϱ����� �ʿ��� ������ ������.  */
/* �����ϱ����� �ʿ��� ����
: ����ϰ��ִ� ���� ����m_iNumChannels, �� ���� �ð������� � ���¸� ���ؾ��ϴ���?!
: Ư��(Idle, Attack, Run) �ִϸ��̼��� �����ϴµ� �ɸ��� �� �ð�. Duration
: Ư��(Idle, Attack, Run) �ִϸ��̼��� ����ӵ�. TickPerSecond * TimeDelta
: �ִϸ��̼��� �̸�.
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

	// ä���ּҸ� ��ȯ, �ð� ���⵵O(1)
	class CChannel* Get_ChannelByIndex(_uint iIndex) {
		return m_Channels[iIndex];
	}
	_bool IsFinished() {
		return m_isFinished;
	}
	// ä���ּҸ� ��ȯ, �ð� ���⵵O(n), ��ã���� nullptr��ȯ
	class CChannel* Get_ChannelByName(string strName);

	_double* GetTickPerSecondPtr() {
		return &m_TickPerSecond;
	}
	_bool* GetIsLoopPtr() {
		return &m_isLoop;
	}

public: // Setter

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

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const CModel::BONES& Bones);
	CAnimation* Clone();
	virtual void Free();
};

END