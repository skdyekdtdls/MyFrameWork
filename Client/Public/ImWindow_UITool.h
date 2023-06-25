
#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImWindow_UITool final : public CImWindow
{
protected:
    explicit CImWindow_UITool(ImGuiIO* pIO);
    virtual ~CImWindow_UITool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_UITool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
