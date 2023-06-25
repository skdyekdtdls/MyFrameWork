
#ifdef _DEBUG
#pragma once
#include "ImWindow.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CImWindow_ObjectTool final : public CImWindow
{
protected:
    explicit CImWindow_ObjectTool(ImGuiIO* pIO);
    virtual ~CImWindow_ObjectTool() = default;

private:
    virtual HRESULT Initialize(void* pArg = nullptr);
    
public:
    virtual void Tick();
   
    void Set_GameObject(CGameObject* pGameObject);
    void ShowFileDialog();

private: /* Staitc_Mesh */
    CTerrain* m_pCurTerrain = { nullptr };
    int Staitc_Mesh_item_current = 0;
    vector<std::string> Static_Mesh_items;

private: /* Skeletal_Mesh */
    int Skeletal_Mesh_item_current = 0;
    vector<std::string> Skeletal_Mesh_items;

    std::string filePathName;
    std::string filePath;

private: /* Transform */
    
    string  strName = "dummy";
    _float3 vPos = { 0.f, 0.f, 0.f };
    _float3 vRot = { 0.f, 0.f, 0.f };
    _float3 vScale = { 0.f, 0.f, 0.f };

private:
    CGameObject* m_pGameObject = { nullptr };
     
private:
    void VecInfo(const char* text, _float3* vec3, int iSize);

public:
    static  CImWindow_ObjectTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
