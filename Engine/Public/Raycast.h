#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL Raycast final : public CCollider
{
public:
	typedef struct tagRaycastDesc : public tagCColliderDesc
	{
		tagRaycastDesc() : tagCColliderDesc() {}
		_float fLength;
	}RAYCAST_DESC;

private:
	explicit Raycast(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit Raycast(const Raycast& rhs);
	virtual ~Raycast() = default;

public: // Getter
	_vector GetRayOrigin() const { return XMLoadFloat4(&m_tRay.vRayOrigin); }
	_vector GetRayDir() const { return XMVector3Normalize(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m_tRay.vRayDir))); }
	_float	GetLength() const { return m_fLength; }

public: // Setter
	
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_fvector vRayOrigin, _fvector vRayDir);

	virtual _bool Intersect(CCollider * pOtherCollider) override;

#ifdef _DEBUG
	virtual HRESULT Render() override;
#endif

private:
	_float3 m_vCenter;
	_float	m_fLength = { 1.f };
	RAY		m_tRay = { _float4(), _float4(0.f, 0.f, 1.f, 1.f) };

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CColliderRay"; }
	static Raycast* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END