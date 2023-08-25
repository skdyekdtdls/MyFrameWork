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

class SmokeParticle final : public CGameObject
{
public:
    typedef struct tagSmokeParticleDesc : public tagCGameObjectDesc
    {
        tagSmokeParticleDesc() : tagCGameObjectDesc() {}
        _uint iNumParticles;
        _float fScale = { 0.2f };
    }SMOKE_PARTICLE_DESC;

private:
    typedef struct tagSmokeParticle
    {
        _bool        isAlive;
        _float4     vVelocity;
        _float4x4    WorldMatrix;
        _double     dAge;
        _double     dLifeTime;
        _double     dGenTime;
    }SMOKEPARTICLE;

private:
    explicit SmokeParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit SmokeParticle(const SmokeParticle& rhs);
    virtual ~SmokeParticle() = default;

public:
    virtual HRESULT Initialize_Prototype() override { return S_OK; }
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta) override;
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render();

public:
    void Reset_Effects();
    void Reset_Particle(SMOKEPARTICLE& Particle);
    void Render_Effect(_fvector vEffectPos);

private:
    vector<SMOKEPARTICLE>   m_Particles;
    _uint                 m_iNumParticles;
    _float                  m_fScale = { 0.2f };
private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CTexture* m_pTextureGradient = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT SetUp_ShaderResources();

public:
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_SmokeParticle"; }
    static SmokeParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual SmokeParticle* Clone(void* pArg) override;
    virtual void Free() override;
};

END
