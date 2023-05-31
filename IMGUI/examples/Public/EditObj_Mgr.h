#pragma once

#include "Imgui_Defines.h"
#include "EditObject.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
END

BEGIN(Imgui)

class CEditObj_Mgr final : public CBase
{
    DECLARE_SINGLETON(CEditObj_Mgr);
public:
    explicit CEditObj_Mgr() = default;
    ~CEditObj_Mgr() = default;

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    HRESULT Add_EditObj(string tag, CEditObject* pEditObj);
    void Update(_double TimeDelta);
    void Late_Update(_double TimeDelta);
    void Render();

private:
    CEditObject* Find_EditObject(string tag);

private:
    CRenderer*                  m_pRenderer = { nullptr };
    map<string, CEditObject*>   m_pEditObjects;
    int                         m_iID = { 0 };
    ID3D11Device*               m_pDevice = { nullptr };
    ID3D11DeviceContext*        m_pContext = { nullptr };
public:
    virtual void Free(void) override;
};

END
