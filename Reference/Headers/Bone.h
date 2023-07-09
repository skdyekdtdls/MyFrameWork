#pragma once

#include "Model.h"

BEGIN(Engine)

class ENGINE_DLL CBone final : public CBase
{
public:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;

public:
	void SaveAssimp(HANDLE hFile, DWORD& dwByte);
	void LoadAssimp(HANDLE hFile, DWORD& dwByte);
public: /* Getter */
	const char* GetName() const {
		return m_szName;
	}

	_float4x4 Get_OffsetMatrix() const {
		return m_OffsetMatrix;
	}

	_float4x4 Get_CombinedTransformationMatrix() const {
		return m_CombinedTransformationMatrix;
	}

	const _float4x4* Get_CombinedTransformationMatrixPtr() const {
		return &m_CombinedTransformationMatrix;
	}

	_int GetParentIndex() {
		return m_iParentIndex;
	}

	_int GetIndex() {
		return m_iIndex;
	}

	_bool HasParent() {
		return (-1 != m_iParentIndex) ? true : false;
	}

	_float3 GetScale();
	_float4 GetQuaternion();
	_float3 GetTranslation();

	_float GetTransformationMatrix_43() {
		return m_TransformationMatrix._43;
	}

	_float3 GetPosition() {
		_float3 vPos;
		memcpy(&vPos, m_TransformationMatrix.m[3], sizeof(_float3));
		return vPos;
	}

	BODY Get_Body() {
		return m_eBody;
	}

	_bool IsCombined() {
		return m_bCombined;
	}

public:/* Setter */
	void SetCombined() {
		m_bCombined = true;
	}

	void Set_Body(BODY eBody) {
		m_eBody = eBody;
	}

	void SetPosition(_float3 vPos) {
		memcpy(m_TransformationMatrix.m[3], &vPos, sizeof(_float3));
	}

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
	void FixBone();

	void Set_ParentIndex(_uint iParentIndex) {
		m_iParentIndex = iParentIndex;
	}

public:
	HRESULT Initialize(aiNode* pAINode, CBone* pParent, _uint iIndex);
	void Invalidate_CombinedTransformationMatrix(const CModel::BONES& Bones);

private:
	char			m_szName[MAX_PATH] = "";
	_float4x4		m_TransformationMatrix;
	KEYFRAME		m_tCurKeyFrameState;

	_bool			m_bCombined = { false };
	_float4x4		m_CombinedTransformationMatrix;
	_float4x4		m_OffsetMatrix;
	_int			m_iParentIndex = { -1 };
	_uint			m_iIndex = { 0 };
	BODY			m_eBody = { BODY_END };
public:
	static CBone* Create(aiNode* pAINode, CBone* pParent, _uint iIndex);
	CBone* Clone();
	virtual void Free() override;
};

END