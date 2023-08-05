#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Instance final : public CVIBuffer_Instancing
{
public:
	typedef struct tagCVIBuffer_Point_InstanceDesc : public tagCVIBuffer_InstancingDesc
	{
		tagCVIBuffer_Point_InstanceDesc() : tagCVIBuffer_InstancingDesc() {};
	}CVIBUFFER_POINT_INSTANCE_DESC;
private:
	CVIBuffer_Point_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const CVIBuffer_Instancing::CVIBUFFER_INSTANCE_DESC* InstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CVIBuffer_Point_Instance"; }
	static CVIBuffer_Point_Instance* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const CVIBuffer_Instancing::CVIBUFFER_INSTANCE_DESC* pInstanceDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END