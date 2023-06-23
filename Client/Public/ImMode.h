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
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    MODE    m_eMode = { MODE_END };

public:
    static  CImMode* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
