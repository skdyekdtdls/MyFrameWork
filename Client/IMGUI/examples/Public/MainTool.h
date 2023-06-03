#pragma once

#include "Imgui_Defines.h"
#include "ToolInstance.h"

BEGIN(Imgui)
class CMainTool final : public CBase
{
private:
    CMainTool();
    ~CMainTool() = default;
    NO_COPY(CMainTool);

public:
    HRESULT Initialize();
    void Tick(_double TimeDelta);
    HRESULT Render();

public:
    static CMainTool* Create();
    void Free() override;
};

END
