
#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_ObjectTool final : public CImWindow
{
protected:
    explicit CImWindow_ObjectTool(ImGuiIO* pIO);
    virtual ~CImWindow_ObjectTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

public:
    static  CImWindow_ObjectTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;

private: /* Static_ */
    int Prototype_item_current = 0;
    vector<std::string> Prototype_items;

    int Hierachy_item_current = 0;
    vector<std::string> Hierachy_itesm;
private: /* */


};

END
#endif
