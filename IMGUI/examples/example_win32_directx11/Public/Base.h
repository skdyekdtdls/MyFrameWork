#pragma once

#include "Imgui_defines.h"

class CBase abstract
{
protected:
    CBase() = default;
    virtual ~CBase() = default;

public:
    _ulong AddRef();
    _ulong Release();

private:
    _ulong m_dwRefCnt = { 0 };
public:
    virtual void Free() = 0;
};
