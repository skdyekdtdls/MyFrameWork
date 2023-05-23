#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CVIBuffer_Grid final : public CVIBuffer
{
private:
	CVIBuffer_Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Grid(const CVIBuffer_Grid& rhs);
	~CVIBuffer_Grid() = default;

public:
	HRESULT Initialize_Prototype(_float fWidth, _float fDepth, _uint iXNum, _uint iZNum);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Grid* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext
		, _float fWidth, _float fDepth, _uint iXNum, _uint iZNum);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void) override;
};

END