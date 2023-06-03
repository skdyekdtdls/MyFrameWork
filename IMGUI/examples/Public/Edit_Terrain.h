#pragma once


#pragma once

#include "Imgui_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Terrain;
END

BEGIN(Imgui)
class CEdit_Terrain final : public CGameObject
{
private:
    CEdit_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEdit_Terrain(const CEdit_Terrain& rhs);
    virtual ~CEdit_Terrain() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta) override;
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render() override;

private: /* For. Component */
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT SetUp_ShaderResources();

public:
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_CEdit_Terrain"; }
    static CEdit_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free(void) override;
};
END


