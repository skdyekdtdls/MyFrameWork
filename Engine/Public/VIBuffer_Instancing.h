#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct tagInstanceDesc
	{
		_float		fHeight;
		_float3		vRange;
		_uint2		vSpeed;
		_float		fLifeTime;
		_uint		iNumInstance;
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
	ID3D11Buffer*				m_pVBInstance = { nullptr };
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