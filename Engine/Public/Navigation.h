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
	// �׺���̼� .dat������ �о ���� �߰��Ѵ�.
	// ���� �߰��� �� �̿� ������ �������ش�.
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFiles);
	
	// NAVIGATIONDESC����ü�� �̿��Ͽ� ���� �ε����� �ʱ�ȭ�Ѵ�.
	// �⺻�� -1�̸� �ʱ�ȭ�� ���ϸ� �迭 out of range�����߻�.
	virtual HRESULT Initialize(void* pArg) override;

	// ���� Size ��ȯ
	_uint GetCellNum() {
		return m_Cells.size();
	}

	// ���� ���� �� m_Cells�����̳ʿ� �߰��Ѵ�.
	void AddCell(const _float3* vPoints);
	const _float3& ContactNormal() const {
		return m_vContactNormal;
	}

	// �̵� ����� ������ ������ �븻���͸� �޾Ƽ� �����̵� ���͸� ��ȯ�Ѵ�.
	_vector GetSlidingVector(_fvector vLook, _fvector vContactNormal);

	// ���� ��ġ�� ������ �� �ִ��� �˻�. �� �Լ��� ȣ��Ǹ� ContactNormal�� ������ ���ŵȴ�.
	_bool is_Move(_fvector vPosition);
#ifdef _DEBUG
	// _DEBUG ���̴��� �ݶ��̴� ���� ������ ���ε��Ѵ�.(WVP, Color)
	void Set_ShaderResources();

	// _DEBUG ��缿�� �ݶ��̴� ��ġ�� �����Ѵ�.
	void UpdateCellsCollider();

	// _DEBUG ���� �ݶ��̴� ��ġ�� �����Ѵ�.
	void UpdateCellCollider(_uint iIndex);

	//_DEBUG VIBuffer�� ColliderSphere�� �����ϴ� �Լ�.
	HRESULT Render_Navigation();

	//_DEBUG ��ŷ�� ��� �������� vector�����̳ʿ� ä���ش�.
	_bool IsCellVertexPicked(vector<CELL_PICK_DESC>& tPickDesc, const RAY& tRay);
	
	//_DEBUG ��ŷ�� ���� ������ ����ü�� ä���ش�.
	_bool IsCellPicked(CELL_PICK_DESC& tCellPickDesces, const RAY& tRay);
#endif

private:
	_float3 m_vContactNormal;
	CNAVIGATION_DESC	m_tNaviDesc;
	vector<class CCell*>	m_Cells;

#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
#endif // _DEBUG

private:
	// ������ ��ȸ�ϸ鼭 ������ �̿��ε��� ������ �����Ѵ�. O(N^2)
	void SetUp_Neighbors();

public:
	void Release_Cells();
	static const _tchar* ProtoTag() { return L"Prototype_Component_CNavigation"; }
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END