#pragma once

#include "Imgui_Defines.h"
#include "Composite.h"

BEGIN(Imgui)
class CEditObject abstract : public CComposite
{
protected:
    CEditObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CEditObject() = default;

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Update(_double TimeDelta);
    virtual void Late_Update(_double TimeDelta);
    virtual HRESULT Render() override;

public:
    virtual void Free(void) override;
};
END


