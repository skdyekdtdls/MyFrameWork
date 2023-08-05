#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
public:
	typedef struct tagCVIBuffer_InstancingDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_InstancingDesc() : tagCVIBufferDesc() {};
		_uint		iNumInstance;
	}CVIBUFFER_INSTANCE_DESC;

protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	
public:
	virtual void Tick(list<_float4x4>& InstMatrices, _double TimeDelta);
	virtual HRESULT Render();

protected:
	ID3D11Buffer*				m_pVBInstance = { nullptr }; // RULT
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };

	CVIBUFFER_INSTANCE_DESC				m_InstanceDesc;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END