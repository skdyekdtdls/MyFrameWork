#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Ray final : public CVIBuffer
{
public:
	typedef struct tagCVIBuffer_RayDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_RayDesc() : tagCVIBufferDesc() {};
	}CVIBUFFER_RAY_DESC;

protected:
	CVIBuffer_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Ray(const CVIBuffer_Ray& rhs);
	virtual ~CVIBuffer_Ray() = default;

public:
	HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_VIBuffer_Ray"; }
	static CVIBuffer_Ray* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;

};

END