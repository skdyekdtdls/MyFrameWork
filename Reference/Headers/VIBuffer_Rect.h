#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
protected:
	CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_Rect() = default;

public:
	HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_VIBuffer_Rect"; }
	static CVIBuffer_Rect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END