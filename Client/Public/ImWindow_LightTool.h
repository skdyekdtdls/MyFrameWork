
#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImWindow_LightTool final : public CImWindow
{
protected:
    explicit CImWindow_LightTool(ImGuiIO* pIO);
    virtual ~CImWindow_LightTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_LightTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
