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
	// �ε���[0, 2]�� �븻 ���͸� ��ȯ
	_float3 Get_Normal(NEIGHBOR eNeighbor) {
		return m_vNormals[eNeighbor];
	}

	// �ε���[0, 2]�� ����ġ�� ��ȯ.
	_vector Get_Point(POINT ePoint) {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	// NEIGHBOR��° �ε����� Cell�� �ε����� ä���.
	void SetUp_Neighbor(NEIGHBOR eNeighbor, CCell* pNeighbor) {
		m_iNeighborIndices[eNeighbor] = pNeighbor->m_iIndex;
	}

	// �� ��ġ, ��������, VIBuffer, �ݶ��̴� ���� ����
	void Set_Point(POINT ePoint, const _float3* vPos);

	// �ﰢ���� ������ ���� ���Ͽ� �������͸� �����.
	void CalcNormal();

public:
	// ���� ��ġ�� �ε����� �ʱ�ȭ�Ѵ�.(DEBUG���� ���ۿ� �ݶ��̴��� �ʱ�ȭ)
	HRESULT Initialize(const _float3* pPoints, _int iIndex);

	// �� ���� �����ִ��� �˻��Ѵ�.
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);

	// �ﰢ���� �� �� �ȿ� ���� ����ִ��� �˻��ϰ�, ������� ������ ��� ������ �������� NEIGHBOR������ �˷��ش�.
	_bool is_In(_fvector vPosition, _int* pNeighborIndex, NEIGHBOR& eNeighbor);

#ifdef _DEBUG
public:
	
	HRESULT Render_ColliderSphere();
	HRESULT Render_VIBuffer();

	// ���� ���� 1���� ���ؼ� �ݶ��̴� ��ġ ������Ʈ
	void	UpdateColliderForVertex(POINT ePoint);

	// ���� ���� 3���� ���ؼ� �ݶ��̴� ��ġ ������Ʈ 
	void	UpdateColliderForVertices();

	_bool IsCellVertexPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay);
	_bool IsCellPicked(CELL_PICK_DESC& tPickDesc, const RAY& tRay);
	
	// �ݽð� ������ ������ �ð� ���⿬��� �ٲٴ� �Լ�.
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