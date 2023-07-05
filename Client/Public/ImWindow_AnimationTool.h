#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"
BEGIN(Engine)
class CAnimation;
class CGameObject;
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

private:
    // 애니메이션 리스트 박스
    _int Animation_item_current = -1;
    vector<std::string> Animation_items;
    CAnimation* m_pAnimation = { nullptr };

    // 뼈 리스트박스
    _int Bone_item_current = -1;
    vector<std::string> Bone_items;

    // 더미 설정
    _bool m_isDummy = { false };
    CGameObject* m_pDummyObject = { nullptr };

private:


private:
    void SaveAnimationData();
    void LoadAnimationData();
public:
    static  CImWindow_AnimationTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif