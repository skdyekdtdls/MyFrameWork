#pragma once

#include "Base.h"
#include "ISerializable.h"
BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
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
	void Save(HANDLE hFile, DWORD& dwByte);

public:
	// 인덱스[0, 2]의 노말 벡터를 반환
	_float3 Get_Normal(NEIGHBOR eNeighbor) {
		return m_vNormals[eNeighbor];
	}

	// 인덱스[0, 2]의 점위치를 반환.
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	// NEIGHBOR번째 인덱스에 Cell의 인덱스를 채운다.
	void SetUp_Neighbor(NEIGHBOR eNeighbor, CCell* pNeighbor) {
		m_iNeighborIndices[eNeighbor] = pNeighbor->m_iIndex;
	}

	// 점 위치, 법선벡터, VIBuffer, 콜라이더 정보 갱신
	void Set_Point(POINT ePoint, const _float3* vPos);

	// 삼각형의 각각의 변에 대하여 법선벡터를 만든다.
	void CalcNormal();

public:
	// 셀의 위치와 인덱스를 초기화한다.(DEBUG에선 버퍼와 콜라이더도 초기화)
	HRESULT Initialize(const _float3* pPoints, _int iIndex);

	// 두 점이 겹쳐있는지 검사한다.
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);

	// 삼각형의 세 변 안에 점이 들어있는지 검사하고, 들어있지 않으면 어느 변으로 나갔는지 NEIGHBOR변수로 알려준다.
	_bool is_In(_fvector vPosition, _int* pNeighborIndex, NEIGHBOR& eNeighbor);

#ifdef _DEBUG
public:
	
	HRESULT Render_ColliderSphere();
	HRESULT Render_VIBuffer();

	// 셀의 정점 1개에 대해서 콜라이더 위치 업데이트
	void	UpdateColliderForVertex(POINT ePoint);

	// 셀의 정점 3개에 대해서 콜라이더 위치 업데이트 
	void	UpdateColliderForVertices();

	_bool IsCellVertexPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay);
	_bool IsCellPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay);
	
	// 반시계 방향의 연결을 시계 방향연결로 바꾸는 함수.
	void ClockWiseSort();
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
	void Release_Debug();
private:
	class vector<class CColliderSphere*> m_pColliderSpheres;
	class CVIBuffer_Cell* m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void Free();
};

END