#pragma once

#include "Model.h"

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}

	_float4x4 Get_OffsetMatrix() const {
		return m_OffsetMatrix;
	}

	_float4x4 Get_CombinedTransformationMatrix() const {
		return m_CombinedTransformationMatrix;
	}

	void Set_OffsetMatrix(const _float4x4& OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const NODE* pNODE, CBone* pParent, _uint iIndex);
	void Invalidate_CombinedTransformationMatrix(const CModel::BONES& Bones);
private:
	char			m_szName[MAX_PATH] = "";
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;
	_float4x4		m_OffsetMatrix;
	_int			m_iParentIndex = { -1 };
	_uint			m_iIndex = { 0 };

public:
	static CBone* Create(const NODE* pNODE, CBone* pParent, _uint iIndex);
	CBone* Clone();
	virtual void Free() override;

};

END