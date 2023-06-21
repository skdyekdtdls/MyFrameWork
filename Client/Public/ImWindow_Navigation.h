#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CTerrain;
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

    void Set_Terrain(CTerrain* pTerrain);
    void AddItems(const char* strItem);

public:
    MODE m_eMode = { MODE_END };

private:
    CTerrain* m_pCurTerrain = { nullptr };
    int item_current = 0;
    vector<std::string> items;

private:
    static bool VectorGetter(void* data, int idx, const char** out_str);

public:
    static  CImWindow_Navigation* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
