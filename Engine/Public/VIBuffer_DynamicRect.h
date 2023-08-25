#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_DynamicRect final : public CVIBuffer
{
public:
	typedef struct tagCVIBuffer_DynamicRectDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_DynamicRectDesc() : tagCVIBufferDesc() {};
	};
protected:
	CVIBuffer_DynamicRect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_DynamicRect(const CVIBuffer_DynamicRect& rhs);
	virtual ~CVIBuffer_DynamicRect() = default;

public:
	HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void SetVTXPOS(VTXPOSTEX* pVertices);

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CVIBuffer_DynamicRect"; }
	static CVIBuffer_DynamicRect* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END