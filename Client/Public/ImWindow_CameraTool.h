
#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_CameraTool final : public CImWindow
{
protected:
    explicit CImWindow_CameraTool(ImGuiIO* pIO);
    virtual ~CImWindow_CameraTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_CameraTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
