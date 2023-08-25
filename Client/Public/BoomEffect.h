#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CVIBuffer_Point_Instance;
class CCollider;
END

BEGIN(Client)

class BoomEffect final : public CGameObject
{
public:
    typedef struct tagBoomEffectDesc : public tagCGameObjectDesc
    {
        tagBoomEffectDesc() : tagCGameObjectDesc() {}
        _uint iNumParticles;
        const _tchar* pTextureProtoTag;
    }BOOM_EFFECT_DESC;

private:
    typedef struct tagBoomParticle
    {
        _bool        isAlive;
        _float4     vAccel;
        _float4     vVelocity;
        _float4x4    WorldMatrix;
        _double     dAge;
        _double     dLifeTime;
    }BOOMPARTICLE;

private:
    explicit BoomEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit BoomEffect(const BoomEffect& rhs);
    virtual ~BoomEffect() = default;

public:
    virtual HRESULT Initialize_Prototype() override { return S_OK; }
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta) override;
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render();

public:
    void Reset_Effects();
    void Reset_Particle(BOOMPARTICLE& Particle);
    void Render_Effect(_fvector vEffectPos);
    void SetPos(_fvector vPos) {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
    }

private:
    vector<BOOMPARTICLE>   m_Particles;
    _uint                 m_iNumParticles;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CTexture* m_pTextureGraCom = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };

private:
    HRESULT Add_Components(const _tchar* TextureProtoTag);
    HRESULT SetUp_ShaderResources();

public:
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_BoomEffect"; }
    static BoomEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual BoomEffect* Clone(void* pArg) override;
    virtual void Free() override;
};

END
