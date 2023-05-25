#pragma once

#include "Base.h"

class CImWindow;

class CImWindow_Manager final : public CBase
{
    DECLARE_SINGLETON(CImWindow_Manager);
public:
    CImWindow_Manager();
    ~CImWindow_Manager() = default;

public:
    HRESULT Add_ImWindow(wstring tag, CImWindow* pImWindow);
    void Update();

private:
    unordered_map<const _tchar*, CImWindow*> m_ImWindows;
public:
    virtual void Free(void) override;
};
