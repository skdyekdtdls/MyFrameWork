#pragma once

#include "Imgui_Defines.h"
#include "GameInstance.h"
#include "ImWindow_Manager.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Imgui)
class CMainTool final : public CBase
{
private:
    CMainTool();
    ~CMainTool() = default;
    NO_COPY(CMainTool);

public:
    HRESULT Initialize();
    
    void Tick(_double TimeDelta);
    HRESULT Render();

    //HRESULT Ready_Prototype_Component();
    //HRESULT Ready_Prototype_GameObject();

private:
    void Update_Demo();

private: /* For Prototype */
    CRenderer* m_pRenderer = { nullptr };

private:
    CGameInstance*      m_pGameInstance = { nullptr };
    CImWindow_Manager* m_pImWindow_Manager = { nullptr };

private:
    ID3D11Device*           m_pDevice = { nullptr };
    ID3D11DeviceContext*    m_pContext = { nullptr };
    ImGuiIO*                m_pIO = { nullptr };

public:
    static CMainTool* Create();
    void Free() override;
};

END
