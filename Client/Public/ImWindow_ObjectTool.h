
#ifdef _DEBUG
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

private: /* Static_Mesh */
    int Static_Mesh_item_current = 0;
    vector<std::string> Static_Mesh_items;

private: /* Skeletal_Mesh*/
    int Skeletal_Mesh_item_current = 0;
    vector<std::string> Skeletal_Mesh_items;
    char buf[32] = "";

private: /* Hierachy */
    int Hierachy_item_current = 0;
    vector<std::string> Hierachy_items;

};

END
#endif
