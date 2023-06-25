#pragma once

#include "GameObject.h"
#include "ISerializable.h"
BEGIN(Engine)

class ENGINE_DLL CCell final : public CGameObject, public ISerializable
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCell(const CCell& rhs) = delete;
	CCell& operator=(const CCell& rhs) = delete;
	virtual ~CCell() = default;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte) override;
	virtual void Load(HANDLE hFile, DWORD& dwByte, _uint iLevelIndex) override;
public:
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	void SetUp_Neighbor(NEIGHBOR eNeighbor, CCell* pNeighbor) {
		m_iNeighborIndices[eNeighbor] = pNeighbor->m_iIndex;
	}
public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_bool is_In(_fvector vPosition, _int* pNeighborIndex);

#ifdef _DEBUG
public:
	HRESULT Render();
	_bool PickedCell(const RAY& tRay);
#endif

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
private:
	_int				m_iIndex = { 0 };
	_float3				m_vPoints[POINT_END];
	_float3				m_vNormals[NEIGHBOR_END];
	_int				m_iNeighborIndices[NEIGHBOR_END] = { -1, -1, -1 };

#ifdef _DEBUG
private:
	class CVIBuffer_Cell* m_pVIBuffer = { nullptr };
	vector<BoundingSphere>	m_BoundingSphereSpheres;
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free();
};

END