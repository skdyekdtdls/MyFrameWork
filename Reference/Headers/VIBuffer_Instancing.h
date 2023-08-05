#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_float		fHeight; // y축 레인지
		_float3		vRange; // x, z축 레인지
		_uint2		vSpeed; // m_pInstanceSpeed
		_float		fLifeTime; // m)LiftTime
		_uint		iNumInstance; // 인스턴싱 수
	}INSTANCEDESC;

	typedef struct tagCVIBuffer_InstancingDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_InstancingDesc() : tagCVIBufferDesc() {};
	}CVIBUFFER_INSTANCE_DESC;

protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCEDESC* pInstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;
	
public:
	virtual void Tick(_double TimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Buffer*				m_pVBInstance = { nullptr }; // RULT
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };

	INSTANCEDESC				m_InstanceDesc;
	_float*						m_pInstanceSpeed = { nullptr };
	_float4*					m_pInstancePos = { nullptr };
	_double*					m_pLifeTime = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END