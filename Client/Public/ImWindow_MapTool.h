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
    enum NAVI_MODE { CREATE_MODE, VERTEX_EDIT_MODE, SELECT_CELL_MODE, MODE_END };

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
    void SaveNaviMesh(string filePathName);
private: /* VERTEX_EDIT_MODE */
    void VecInfo(const char* text, _float3* vec3, int iSize);
    void Vertex_Edit();

private:
    void CreateTriangleStrip();

public:
    CImWindow_Manager* m_pImMgr = { nullptr };
    NAVI_MODE m_eNaviMode = { MODE_END };
    
private: /* CREATE_MODE */
    _float3 m_vClickPoint[CCell::POINT_END];
    int m_iClickCount = { 0 };
    
private:  /* VERTEX_EDIT_MODE */
    int Cell_Index_item_current = 0;
    vector<std::string> Cell_Index_items;
    _float3 vPos;
    vector<CELL_PICK_DESC> tCellPickDesces; // 버텍스 정보도 들어옴

private:
    _bool m_bStart = { true };

    // 저장

public:
    static  CImWindow_MapTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
