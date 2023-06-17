#pragma once

#include "Base.h"
#include "Model.h"

BEGIN(Engine)
class CBone final : public CBase
{
private:
	explicit CBone();
	virtual ~CBone() = default;

public:
	const char* Get_Name() const {
		return m_szName;
	}

	_float4x4	Get_CombinedTransformationMatrix() const {
		return m_CombinedTransformationMatrix;
	}

	_float4x4	Get_OffsetMatrix() const {
		return m_OffsetMatrix;
	}

	void Set_OffsetMatrix(const _float4x4& OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

public:
	HRESULT Initialize(const NODE* pNode, CBone* pParent, _uint iIndex);
	void	Invalidate_CombinedTransformationMatrix(const CModel::BONES& Bones);

private:
	char m_szName[MAX_PATH] = "";
	_float4x4	m_TransformationMatrix;
	_float4x4	m_CombinedTransformationMatrix;
	_float4x4	m_OffsetMatrix;
	_int		m_iParentIndex = { -1 };
	_uint		m_iIndex = { 0 };

public:
	static CBone* Create(const NODE* pNode, CBone* pParent, _uint iIndex);
	virtual void Free(void) override;
	CBone* Clone();
};

END