#pragma once

#include "Imgui_Defines.h"
#include "Level.h"

BEGIN(Imgui)
class CLevel_Tool final : public CLevel
{
private:
    explicit CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CLevel_Tool(const CLevel_Tool& rhs);
    virtual ~CLevel_Tool() = default;

public:
    virtual HRESULT Initialize();
    virtual void Tick(_double TimeDelta);
    virtual void Late_Tick(_double TimeDelta);
    virtual HRESULT Render();

private:
    HRESULT Ready_Prototype_Component();
    HRESULT Ready_Prototype_GameObject(const _tchar* pLayerTag);

public:
    static CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free(void) override;
};

END
