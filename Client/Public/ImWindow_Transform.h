
#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImWindow_Transform final : public CImWindow
{
protected:
    explicit CImWindow_Transform(ImGuiIO* pIO);
    virtual ~CImWindow_Transform() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();
   
    void Set_GameObject(CGameObject* pGameObject);

private:
    string  strName = "dummy";
    _float3 vPos = { 0.f, 0.f, 0.f };
    _float3 vRot = { 0.f, 0.f, 0.f };
    _float3 vScale = { 0.f, 0.f, 0.f };

private:
    CGameObject* m_pGameObject = { nullptr };

private:
    void VecInfo(const char* text, _float3* vec3, int iSize);

public:
    static  CImWindow_Transform* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
