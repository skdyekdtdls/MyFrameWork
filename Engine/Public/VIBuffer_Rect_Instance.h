#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instance final : public CVIBuffer_Instancing
{
public:
	typedef struct tagCVIBuffer_Rect_InstanceDesc : public tagCVIBuffer_InstancingDesc
	{
		tagCVIBuffer_Rect_InstanceDesc() : tagCVIBuffer_InstancingDesc() {};
	}CVIBUFFER_RECT_INSTANCE_DESC;

private:
	CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance& rhs);
	virtual ~CVIBuffer_Rect_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const CVIBuffer_Instancing::INSTANCEDESC* InstanceDesc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_double TimeDelta) override;

public:
	static const _tchar* ProtoTag() { return L"Prototype_Component_CVIBuffer_Rect_Instance"; }

public:
	static CVIBuffer_Rect_Instance* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const CVIBuffer_Instancing::INSTANCEDESC* pInstanceDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END