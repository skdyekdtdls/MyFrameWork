#pragma once

#include "Base.h"

class CImWindow abstract : public CBase
{
protected:
    CImWindow();
    virtual ~CImWindow() = default;

public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Update() = 0;

public:
    virtual void Free(void) override;
};
