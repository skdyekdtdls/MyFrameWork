#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const	{
		return m_iMaterialIndex;
	}

	void Get_Matrices(CModel::BONES Bones, _float4x4* pMatrices, _fmatrix PivotMatrix);

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eType, const CModel::BONES& Bones, const MESH* pMesh, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg) override;

private:
	char	m_szName[MAX_PATH] = "";
	_uint	m_iMaterialIndex = { 0 };

private:
	_uint	m_iNumBones = { 0 };
	vector<_uint> m_BoneIndices; /* ∏µ®ø° ∑ŒµÂ«ÿ≥ı¿∫ ¿¸√ºª¿¿Œµ¶Ω∫ */

private:
	HRESULT Ready_VertexBuffer_NonAnim(const MESH* pMesh, _matrix PivotMatrix);
	HRESULT Ready_VertexBuffer_Anim(const MESH* pMesh, const CModel::BONES& Bones);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, const CModel::BONES& Bones, const MESH* pMesh, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END