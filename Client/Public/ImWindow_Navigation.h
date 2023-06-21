#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_Navigation final : public CImWindow
{
public:
    enum MODE { CREATE_MODE, SELECT_POINT_MODE, SELECT_CELL_MODE, MODE_END };
protected:
    explicit CImWindow_Navigation(ImGuiIO* pIO);
    virtual ~CImWindow_Navigation() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    MODE m_eMode = { MODE_END };

private:
    int item_current = 0;
    vector<std::string> items{ "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };

private:
    static bool VectorGetter(void* data, int idx, const char** out_str);


public:
    static  CImWindow_Navigation* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
