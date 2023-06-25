#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImMode final : public CImWindow
{
protected:
    explicit CImMode(ImGuiIO* pIO);
    virtual ~CImMode() = default;

public:
    IMWIN_MODE Get_Mode() { return m_eMode; }
    IMWIN_MODE Get_PreMode() { return m_ePreMode; }

    void Set_Mode(IMWIN_MODE eMode);
    void Set_PreMode(IMWIN_MODE ePreMode) { m_ePreMode = ePreMode; }


public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

private:
    IMWIN_MODE    m_ePreMode = { MODE_END };
    IMWIN_MODE    m_eMode = { MODE_END };

public:
    static  CImMode* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
