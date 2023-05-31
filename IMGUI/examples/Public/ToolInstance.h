#pragma once
#pragma once

#include "Imgui_Defines.h"
#include "Base.h"

BEGIN(Imgui)

class CToolInstance final : public CBase
{
    DECLARE_SINGLETON(CToolInstance);
public:
    explicit CToolInstance() = default;
    ~CToolInstance() = default;

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Update(_double TimeDelta);
    void Late_Update(_double TimeDelta);
    void Render();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

public:
    virtual void Free(void) override;
};

END
