#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"
BEGIN(Engine)
class CAnimation;
END
BEGIN(Client)
class CImWindow_AnimationTool final : public CImWindow
{
protected:
    explicit CImWindow_AnimationTool(ImGuiIO* pIO);
    virtual ~CImWindow_AnimationTool() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();
    virtual void LateTick() override;
    _bool ShowDummyObject();
    void ChangeDummyObject(CGameObject* pGameObject);
protected:
    _int Animation_item_current = -1;
    vector<std::string> Animation_items;
    CAnimation* m_pAnimation = { nullptr };

    _bool m_isDummy = { false };
    CGameObject* m_pDummyObject = { nullptr };
public:
    static  CImWindow_AnimationTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif