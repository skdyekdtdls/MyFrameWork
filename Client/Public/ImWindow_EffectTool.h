#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_EffectTool final : public CImWindow
{
protected:
    explicit CImWindow_EffectTool(ImGuiIO* pIO);
    virtual ~CImWindow_EffectTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_EffectTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif