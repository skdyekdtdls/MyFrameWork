#ifdef _DEBUG
#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_Imgui final : public CLevel
{
private:
    explicit CLevel_Imgui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CLevel_Imgui(const CLevel_Imgui& rhs);
    virtual ~CLevel_Imgui() = default;

public:
    virtual HRESULT Initialize();
    virtual void Tick(_double TimeDelta);
    virtual void Late_Tick(_double TimeDelta);
    virtual HRESULT Render();

public:
    void Ready_Lights();
    void Ready_Layer_BackGround(const _tchar* pLayerTag);
    void Ready_Layer_Camera(const _tchar* pLayerTag);
    void Ready_Layer_Monster(const _tchar* pLayerTag);
    void Ready_Layer_Player(const _tchar* pLayerTag);
    void Ready_Layer_Effect(const _tchar* pLayerTag);

public:
    static CLevel_Imgui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free(void) override;
};

END

#endif