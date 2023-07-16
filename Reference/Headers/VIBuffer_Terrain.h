#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
	enum POINT { LT, RT, RB, LB, POINT_END };
public:
	typedef struct tagCVIBuffer_TerrainDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_TerrainDesc() : tagCVIBufferDesc() {};
	}CVIBUFFER_TERRAIN_DESC;
protected:
	CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar * pDDSFile);
	virtual HRESULT Initialize(void* pArg) override;
	_bool	IsPicked(FXMVECTOR vRayOrigin, FXMVECTOR vRayDir, _float& fMinDist);

private:
	void CheckPoint(_float3 vPoints[POINT_END], _fvector RayOrigin, _fvector RayDir, _float& fDist);
	_bool isInFourPoint(_fvector LT, _fvector RT, _fvector RB, _gvector LB, _hvector RayOrigin, _hvector RayDir, _float& fDist);

private:
	_uint			m_iNumVerticesX = { 0 };
	_uint			m_iNumVerticesZ = { 0 };
	VTXPOSNORTEX*	m_pVertices = { nullptr };

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_VIBuffer_Terrain"; }
	static CVIBuffer_Terrain* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pHeightMap);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END