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

class HitParticle final : public CGameObject
{
public:
    typedef struct tagHitParticleDesc : public tagCGameObjectDesc
    {
        tagHitParticleDesc() : tagCGameObjectDesc() {}
        _uint iNumParticles;
    }HIT_PARTICLE_DESC;

private:
    typedef struct tagHitParticle
    {
        _bool        isAlive;
        _float4     vAccel;
        _float4     vVelocity;
        _float4x4    WorldMatrix;
        _double     dAge;
        _double     dLifeTime;
    }HITPARTICLE;

private:
    explicit HitParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit HitParticle(const HitParticle& rhs);
    virtual ~HitParticle() = default;

public:
    virtual HRESULT Initialize_Prototype() override { return S_OK; }
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta) override;
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render();

public:
    void Reset_Effects();
    void Reset_Particle(HITPARTICLE& Particle);
    void Render_Effect(_fvector vEffectPos);

private:
    vector<HITPARTICLE>   m_Particles;
    _uint                 m_iNumParticles;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT SetUp_ShaderResources();

public:
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_HitParticle"; }
    static HitParticle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual HitParticle* Clone(void* pArg) override;
    virtual void Free() override;
};

END
