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

class CStone_Effect final : public CGameObject
{
public:
    typedef struct tagCStone_EffectDesc : public tagCGameObjectDesc
    {
        tagCStone_EffectDesc() : tagCGameObjectDesc() {}
        _uint iNumParticles;
    }STONE_EFFECT_DESC;

private:
    typedef struct tagStoneParticle
    {
        _bool        isAlive;
        _float4     vAccel;
        _float4     vVelocity;
        _float4x4    WorldMatrix;
        _double     dAge;
        _double     dLifeTime;
    }STONEPARTICLE;

private:
    explicit CStone_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    explicit CStone_Effect(const CStone_Effect& rhs);
    virtual ~CStone_Effect() = default;

public:
    virtual HRESULT Initialize_Prototype() override { return S_OK; }
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(_double TimeDelta) override;
    virtual void Late_Tick(_double TimeDelta) override;
    virtual HRESULT Render();

public:
    void Reset_Effects();
    void Reset_Particle(STONEPARTICLE& Particle);
    void Render_Effect(_fvector vEffectPos);

private:
    vector<STONEPARTICLE>   m_Particles;
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
    static const _tchar* ProtoTag() { return L"Prototype_GameObject_CStone_Effect"; }
    static CStone_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CStone_Effect* Clone(void* pArg) override;
    virtual void Free() override;
};

END
