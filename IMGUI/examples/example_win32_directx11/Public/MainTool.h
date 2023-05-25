#pragma once

#include "Base.h"

class CMainTool final : public CBase
{
private:
    CMainTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    ~CMainTool() = default;

public:
    void Tick(_double TimeDelta);

private:
    HRESULT Initialize();
    void Update(_double TimeDelta);
    void LateUpdate(_double TimeDelta);
    void Render();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

public:
    static CMainTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};
