#pragma once
#include "Component.h"
#include "ISerializable.h"
BEGIN(Engine)

class ENGINE_DLL CNavigation : public CComponent
{
public:
	typedef struct tagCNavigation : public tagComponentDesc
	{
		tagCNavigation() : tagComponentDesc(), iCurrentIndex(-1) {}
		explicit tagCNavigation(_int _iCurrentIndex) : iCurrentIndex{ _iCurrentIndex } {}

		_int iCurrentIndex = { -1 };
	}CNAVIGATION_DESC;

private:
	explicit CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNavigation(const CNavigation& rhs);
	CNavigation& operator=(const CNavigation& rhs) = delete;
	virtual ~CNavigation() = default;

public:
	virtual void Save(HANDLE hFile, DWORD& dwByte);

public:
	// 네비게이션 .dat파일을 읽어서 셀을 추가한다.
	// 셀을 추가한 후 이웃 정보를 갱신해준다.
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFiles);
	
	// NAVIGATIONDESC구조체를 이용하여 현재 인덱스를 초기화한다.
	// 기본이 -1이며 초기화를 안하면 배열 out of range에러발생.
	virtual HRESULT Initialize(void* pArg) override;

	// 셀의 Size 반환
	_uint GetCellNum() {
		return m_Cells.size();
	}

	// 셀을 생성 후 m_Cells컨테이너에 추가한다.
	void AddCell(const _float3* vPoints);
	const _float3& ContactNormal() const {
		return m_vContactNormal;
	}

	// 이동 방향과 접촉한 선분의 노말벡터를 받아서 슬라이딩 벡터를 반환한다.
	_vector GetSlidingVector(_fvector vLook, _fvector vContactNormal);

	// 다음 위치로 움직일 수 있는지 검사. 이 함수가 호출되면 ContactNormal의 정보가 갱신된다.
	_bool is_Move(_fvector vPosition);

	// 들어온 점에 대해서 어떤 셀의 인덱스에 있는지 찾아준다.
	_uint FindIndex(_fvector vPosition);

	// 객체가 들어있는 인덱스를 설정해준다.
	void SetCellCurIndex(_uint iIndex) { m_tNaviDesc.iCurrentIndex = iIndex; }

	_uint GetCellCurIndex() { return m_tNaviDesc.iCurrentIndex; }

	// 셀의 랜덤 포지션을 반환한다.
	_float4 RandomPosInCell(_uint iIndex);

#ifdef _DEBUG

	_bool* isRenderPtr() {
		return &m_bRender;
	}


	// 셀을 지우고 셀의 이웃을 처음부터 다시 연결한다.
	void DeleteCellByIndex(_uint iIndex);

	// _DEBUG 셰이더에 콜라이더 렌더 정보를 바인딩한다.(WVP, Color)
	void Set_ShaderResources();

	// _DEBUG 모든셀의 콜라이더 위치를 갱신한다.
	void UpdateCellsCollider();

	// _DEBUG 셀의 콜라이더 위치를 갱신한다.
	void UpdateCellCollider(_uint iIndex);

	//_DEBUG VIBuffer와 ColliderSphere를 렌더하는 함수.
	virtual HRESULT Render() override;

	//_DEBUG 피킹된 모든 정점들을 vector컨테이너에 채워준다.
	_bool IsCellVertexPicked(vector<CELL_PICK_DESC>& tPickDesc, const RAY& tRay);
	
	//_DEBUG 피킹된 셀의 정보를 구조체에 채워준다.
	_bool IsCellPicked(CELL_PICK_DESC& tCellPickDesces, const RAY& tRay);
#endif

private:
	_float3 m_vContactNormal;
	CNAVIGATION_DESC	m_tNaviDesc;
	vector<class CCell*>	m_Cells;

#ifdef _DEBUG
private:
	class CShader*  m_pShader = { nullptr };
	_bool			m_bRender = { false };
#endif // _DEBUG

private:
	// 셀들을 순회하면서 셀들의 이웃인덱스 정보를 갱신한다. O(N^2)
	void SetUp_Neighbors();

public:
	void Release_Cells();
	static const _tchar* ProtoTag() { return L"Prototype_Component_CNavigation"; }
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END