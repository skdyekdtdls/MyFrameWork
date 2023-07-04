#pragma once

#include "Base.h"
#include "Model.h"
BEGIN(Engine)

class CChannel final : public CBase
{
public:
	CChannel();
	virtual ~CChannel() = default;

public:
	void SaveAssimp(HANDLE hFile, DWORD& dwByte);
	void LoadAssimp(HANDLE hFile, DWORD& dwByte);

	KEYFRAME Get_FrontKeyFrame() {
		return m_KeyFrames.front();
	}
	_uint Get_BoneIndex() {
		return m_iBoneIndex;
	}
	const char* GetName() {
		return m_szName;
	}
public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const CModel::BONES& Bones);
	void Invalidate_TransformationMatrix(CModel::BONES& Bones, _double TimeAcc, _uint* pCurrentKeyFrameIndex, BODY eBody = BODY_END);
	void InterAnimation_TransfomationMatrix(CModel::BONES& Bones, _double TimeAcc, BODY eBody = BODY_END);

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iNumKeyFrames = { 0 };
	vector<KEYFRAME>	m_KeyFrames;
	_uint				m_iBoneIndex = { 0 };

public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const CModel::BONES& Bones);
	virtual void Free() override;
};

END