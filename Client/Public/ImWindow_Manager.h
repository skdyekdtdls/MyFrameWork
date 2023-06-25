#ifdef _DEBUG
#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CImWindow;
class CEditCamera;

class CImWindow_Manager final : public CBase
{
    DECLARE_SINGLETON(CImWindow_Manager);
private:
    explicit CImWindow_Manager();
    ~CImWindow_Manager() = default;

public:
    HRESULT Initialize(__inout ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick();
    void LateTick();
    void Render();

    PICK_DESC GetMinDistPickDesc();
    PICK_DESC GetTerrainPickDesc();
    const _bool& IsPicking();

    void Set_Window(CImWindow* pWindow) {
        m_pCurrentImWindow = pWindow;
    }


public: /* CImWindow_ObjectTool */

public: /* CImWindow_MapTool */
    void AddItems(const char* strItem);
    void AddCell(const _float3* vPoints);
    _uint GetCellNum();
public: /* CImWindow_CameraTool */

public: /* CImWindow_AnimationTool */

public: /* CImWindow_UITool */

public: /* CImWindow_LightTool */

public: /* CImWindow_Demo */

public: /* CImWindow_SaveLoads */

public: /* CImMode */
    IMWIN_MODE Get_Mode();
    IMWIN_MODE Get_PreMode();
    void Set_Mode(IMWIN_MODE eMode);
    void Set_PreMode(IMWIN_MODE ePreMode);

private:
    void Initialize_Imgui(ImGuiIO** pIO, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

private:
    ImGuiIO* m_pIO = { nullptr };

public:
    virtual void Free(void) override;

private:
    _bool m_bStart = { true };
    
private:
    CEditCamera* Get_EditCamera();

private:
    class CImWindow* m_pCurrentImWindow = { nullptr };

    class CImWindow_ObjectTool* m_pObjectTool = { nullptr };
    class CImWindow_MapTool* m_pMapTool = { nullptr };
    class CImWindow_CameraTool* m_pCameraTool = { nullptr };
    class CImWindow_AnimationTool* m_pAnimationTool = { nullptr };
    class CImWindow_EffectTool* m_pEffectTool = { nullptr };
    class CImWindow_UITool* m_pUITool = { nullptr };
    class CImWindow_LightTool* m_pLightTool = { nullptr };

    class CImWindow_Demo* m_pDemo = { nullptr };
    class CImWindow_SaveLoads* m_pSaveLoads = { nullptr };
    class CImMode* m_pImMode = { nullptr };
};

END
#endif