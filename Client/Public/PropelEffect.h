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

class PropelEffect final : public CGameObject
{
public:
    typedef struct tagPropelEffectDesc : public tagCGameObjectDesc
    {
        tagPropelEffectDesc() : tagCGameObjectDesc() {}
        _uint iNumParticles;
        const _tchar* pTextureProtoTag;
    }PROPEL_EFFECT_DESC;

private:
    typedef struct tagPropleParticle
    {
        _float4x4   WorldMatrix;
        _float4     vVelocity;
        _double     dAge;
        _double     dLifeTime;
        _double     dGenTime;
        _float      fSpeed;
        _float4     vDirection;
        _bool       bStart;
    }PROPELPARTICLE;

private:
    explicit PropelEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit PropelEffect(const PropelEffect& rhs);
    virtual ~PropelEffect() = default;

public:
    virtual HRESULT Initialize_Prototype() override { return S_OK; }
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta);
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render();

public:
    void Reset_Effects();
    void Reset_Particle(PROPELPARTICLE& Particle);

    void Render_Effect(_fvector vEffectPos);
    void SetPos(_fvector vPos) {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
    }

private:
    vector<PROPELPARTICLE>  m_Particles;
    _uint                   m_iNumParticles;
    CTransform*             m_pOwnerTransform = { nullptr };

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CRenderer* m_pRendererCom = { nullptr };
    CTransform* m_pTransformCom = { nullptr };
    CVIBuffer_Point_Instance* m_pBufferCom = { nullptr };

private:
    HRESULT Add_Components(const _tchar* TextureProtoTag);
    HRESULT SetUp_ShaderResources();

public:
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_PropelEffect"; }
    static PropelEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual PropelEffect* Clone(void* pArg) override;
    virtual void Free() override;
};

END
