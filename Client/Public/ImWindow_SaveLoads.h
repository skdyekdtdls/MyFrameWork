#ifdef _USE_IMGUI
#pragma once
#include "ImWindow.h"

BEGIN(Client)
class CImWindow_SaveLoads final : public CImWindow
{
protected:
    explicit CImWindow_SaveLoads(ImGuiIO* pIO);
    virtual ~CImWindow_SaveLoads() = default;

public:
    virtual HRESULT Initialize(void* pArg = nullptr);
    virtual void Tick();

private:
    void Serialization(const _tchar* pSaveFileName, _uint iLevelIndex);
    void Deserialization(const _tchar* pSaveFileName, _uint iLevelIndex);

public:
    static  CImWindow_SaveLoads* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif