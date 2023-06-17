#pragma once

#include "Base.h"

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

public:
	HRESULT Initialize(const SCENE& tScene, CBone* pParent);
	void	Invalidate_CombinedTransformationMatrix();

private:
	char m_szName[MAX_PATH] = "";
	_float4x4	m_TransformationMatrix;
	_float4x4	m_CombinedTransformationMatrix;
	_float4x4	m_OffsetMatrix;
	CBone*		m_pParent = { nullptr };

public:
	static CBone* Create(const SCENE& tScene, CBone* pParent);
	virtual void Free(void) override;
};

END