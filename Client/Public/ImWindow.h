#ifdef _DEBUG
#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
static bool VectorGetter(void* data, int idx, const char** out_str)
{
    auto& vector = *static_cast<std::vector<std::string>*>(data);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_str = vector.at(idx).c_str();
    return true;
}

class CImWindow abstract : public CBase
{
public:
    typedef struct Desc
    {
        int a;
    };

protected:
    explicit CImWindow(ImGuiIO* pIO);
    virtual ~CImWindow() = default;

public:
    _bool   m_IsShow;
    _bool   m_bStart = { true };
public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick() = 0;

protected:
    ImGuiIO* m_pIO = { nullptr };

public:
    virtual void Free(void) override;
};
END
#endif