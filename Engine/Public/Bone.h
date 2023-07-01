#pragma once

#include "Model.h"

BEGIN(Engine)

class CBone final : public CBase
{
public:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;


public:
	void SaveAssimp(HANDLE hFile, DWORD& dwByte);
	void LoadAssimp(HANDLE hFile, DWORD& dwByte);
public:
	 
	const char* GetName() const {
		return m_szName;
	}

	_float4x4 Get_OffsetMatrix() const {
		return m_OffsetMatrix;
	}

	_float4x4 Get_CombinedTransformationMatrix() const {
		return m_CombinedTransformationMatrix;
	}

	_bool HasParent() {
		return (-1 != m_iParentIndex) ? true : false;
	}

	_float3 GetScale();
	_float4 GetQuaternion();
	_float3 GetTranslation();

	void Set_OffsetMatrix(const _float4x4& OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void Set_CurKeyFrameState(KEYFRAME tCurKeyFrameState) {
		m_tCurKeyFrameState = tCurKeyFrameState;
	}

	KEYFRAME Get_CurKeyFrameState() {
		return m_tCurKeyFrameState;
	}

public:
	HRESULT Initialize(aiNode* pAINode, CBone* pParent, _uint iIndex);
	void Invalidate_CombinedTransformationMatrix(const CModel::BONES& Bones);

private:
	char			m_szName[MAX_PATH] = "";
	_float4x4		m_TransformationMatrix;
	KEYFRAME		m_tCurKeyFrameState;

	_float4x4		m_CombinedTransformationMatrix;
	_float4x4		m_OffsetMatrix;
	_int			m_iParentIndex = { -1 };
	_uint			m_iIndex = { 0 };
	

public:
	static CBone* Create(aiNode* pAINode, CBone* pParent, _uint iIndex);
	CBone* Clone();
	virtual void Free() override;

};

END