#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Sphere final : public CVIBuffer
{
public:
	typedef struct tagCVIBuffer_SphereDesc : public tagCVIBufferDesc
	{
		tagCVIBuffer_SphereDesc() : tagCVIBufferDesc() {};
	}CVIBUFFER_SPHERE_DESC;
protected:
	CVIBuffer_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Sphere(const CVIBuffer_Sphere& rhs);
	virtual ~CVIBuffer_Sphere() = default;

public:
	HRESULT Initialize_Prototype(_uint iNumSegements, _uint iNumRings, _float fRadius);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CVIBuffer_Sphere"; }
	static CVIBuffer_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumSegements, _uint iNumRings, _float fRadius);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free(void) override;
};

END