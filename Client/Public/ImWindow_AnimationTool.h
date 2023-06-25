#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_AnimationTool final : public CImWindow
{
protected:
    explicit CImWindow_AnimationTool(ImGuiIO* pIO);
    virtual ~CImWindow_AnimationTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_AnimationTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif