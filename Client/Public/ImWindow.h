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
protected:
    explicit CImWindow(ImGuiIO* pIO);
    virtual ~CImWindow() = default;

public:
    _bool   m_IsShow;

public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick() = 0;
    virtual void LateTick();
    virtual void Set_GameObject(CGameObject* pGameObject);

protected:
    ImGuiIO* m_pIO = { nullptr };
    class CGameObject* m_pGameObject = { nullptr };

protected: /* Static_Mesh */
    class CTerrain* m_pCurTerrain = { nullptr };
    _int Static_Mesh_item_current = -1;
    vector<std::string> Static_Mesh_items;

protected: /* Skeletal_Mesh */
    _int Skeletal_Mesh_item_current = -1;
    vector<std::string> Skeletal_Mesh_items;

protected: /* File Dialog*/
    std::string filePathName;
    std::string filePath;

protected: /* Transform */
    string  strName = "dummy";
    _float3 vPos = { 0.f, 0.f, 0.f };
    _float3 vRot = { 0.f, 0.f, 0.f };
    _float3 vScale = { 0.f, 0.f, 0.f };
protected:
    void VecInfo(const char* text, _float3* vec3, int iSize);
public:
    virtual void Free(void) override;
};
END
#endif