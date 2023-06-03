#pragma once

#include "Base.h"
#include "Imgui_Defines.h"
#include "GameInstance.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Imgui)

class CToolInstance final : public CBase
{
    DECLARE_SINGLETON(CToolInstance);
private:
    explicit CToolInstance();
    ~CToolInstance() = default;

public: /* For Tool*/
    HRESULT Initialize();
    void Tick(_double TimeDelta);
    HRESULT Render();

    
public: /* For. ImWindow_Manager */
    class CImWindow* Get_ImWindow(const _tchar* tag);
    HRESULT Add_Window(const _tchar* tag, CImWindow* pWindow);

public: /* GameInstance */
    HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
    HRESULT Add_Prototype(const _tchar* pPrototypeTag, CGameObject* pPrototype);
    HRESULT Add_GameObject(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    ImGuiIO* m_pIO = { nullptr };

private:
    class CRenderer* m_pRenderer = { nullptr };
    class CImWindow_Manager* m_pImWindow_Manager = { nullptr };
    class CGameInstance*    m_pGameInstace = {nullptr};

private:
    HRESULT Ready_Prototype_Component();

public:
    static void Release_Tool();
    virtual void Free(void) override;
};

END
