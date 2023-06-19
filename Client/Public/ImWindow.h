#ifdef _USE_IMGUI
#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CImWindow abstract : public CBase
{
public:
    typedef struct Desc
    {
        int a;
    };

protected:
    explicit CImWindow(ImGuiIO* pIO);
    virtual ~CImWindow() = default;

public:
    _bool   m_IsShow;
    _bool   m_bStart = { true };
public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick() = 0;

protected:
    ImGuiIO* m_pIO = { nullptr };

public:
    virtual void Free(void) override;
};
END
#endif