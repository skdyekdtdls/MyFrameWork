#ifdef _USE_IMGUI
#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "ImWindow_Manager.h"

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

public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick() = 0;

protected:
    ImGuiIO* m_pIO = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };
    CImWindow_Manager* m_pImWindow_Manager = { nullptr };
public:
    virtual void Free(void) override;
};
END
#endif