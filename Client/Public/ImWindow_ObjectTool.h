
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
   
    // 피킹한 객체를 저장하는 로직이다.
    void Set_GameObject(CGameObject* pGameObject);

    // 안씀
    void ShowFileDialog();

private: /* Static_Mesh */
    CTerrain* m_pCurTerrain = { nullptr };
    _int Static_Mesh_item_current = -1;
    vector<std::string> Static_Mesh_items;

private: /* Skeletal_Mesh */
    _int Skeletal_Mesh_item_current = -1;
    vector<std::string> Skeletal_Mesh_items;

    std::string filePathName;
    std::string filePath;

private: /* Obejct_Place */
    _bool   m_bCheck = { false };
    _int    m_bRadioButton = -1;
    char    m_szCurItemLabel[MAX_PATH] = "";
    bool    open_popup = false;

private: /* Transform */
    string  strName = "dummy";
    _float3 vPos = { 0.f, 0.f, 0.f };
    _float3 vRot = { 0.f, 0.f, 0.f };
    _float3 vScale = { 0.f, 0.f, 0.f };

private:
    CGameObject* m_pGameObject = { nullptr };
     
private:
    void VecInfo(const char* text, _float3* vec3, int iSize);
    void ObjectPlace();
public:
    static  CImWindow_ObjectTool* Create(ImGuiIO* pIO);
    virtual void Free(void) override;
};

END
#endif
