
#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImWindow_ObjectTool final : public CImWindow
{
protected:
    explicit CImWindow_ObjectTool(ImGuiIO* pIO);
    virtual ~CImWindow_ObjectTool() = default;

private:
    virtual HRESULT Initialize(void* pArg = nullptr);
    
public:
    virtual void Tick();
    virtual void Set_GameObject(CGameObject* pGameObject) override;

private: /* Obejct_Place */
    _bool   m_bCheck = { false };
    _int    m_bRadioButton = -1;
    char    m_szCurItemLabel[MAX_PATH] = "";
    bool    open_popup = false;
     
private:
    void ObjectPlace();

public:
    static  CImWindow_ObjectTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif