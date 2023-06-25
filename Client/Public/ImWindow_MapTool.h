#ifdef _DEBUG
#pragma once
#include "ImWindow.h"
#include "Cell.h"

BEGIN(Client)
class CTerrain;
class CImWindow_Manager;
class CImWindow_MapTool final : public CImWindow
{
public:
    enum NAVI_MODE { CREATE_MODE, SELECT_POINT_MODE, SELECT_CELL_MODE, MODE_END };

protected:
    explicit CImWindow_MapTool(ImGuiIO* pIO);
    virtual ~CImWindow_MapTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();
    virtual void LateTick();

    void ResetClickCout() {
        m_iClickCount = 0;
    }

    void AddItems(const char* strItem);

private:
    void Object_Place();
    void Edit_Navigation_Mesh();
    void CreateTriangleStrip();

public:
    CImWindow_Manager* m_pImMgr = { nullptr };
    NAVI_MODE m_eNaviMode = { MODE_END };
    
private: /* Create_Mode */
    _float3 m_vClickPoint[CCell::POINT_END];
    int m_iClickCount = { 0 };

private:
    int Cell_Index_item_current = 0;
    vector<std::string> Cell_Index_items;
    
private:
    _bool m_bStart = { true };

public:
    static  CImWindow_MapTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
