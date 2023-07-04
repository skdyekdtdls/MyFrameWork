#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_Demo final : public CImWindow
{
public:
    _float4 clear_color = { 0.45f, 0.55f, 0.60f, 1.00f };

protected:
    explicit CImWindow_Demo(ImGuiIO* pIO);
    virtual ~CImWindow_Demo() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick() ;

public:
    static  CImWindow_Demo* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif