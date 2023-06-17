#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel :
	public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint	Get_NumMeshes() const	{
		return m_iNumMeshes;
	}

	void Set_AnimIndex(_uint iAnimIndex) {
		if (iAnimIndex >= m_iNumAnimations)
			return;
		m_iCurrentAnimIndex = iAnimIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Render(_uint iMeshIndex);

public:
	void Play_Animation(_double TimeDelta);

public:
	HRESULT Bind_Material(class CShader* pShader, const char* pConstantName, _uint iMeshIndex, TextureType MaterialType);
	HRESULT Bind_BoneMatrices(class CShaer* pShader, const char* pConstantName, _uint iMeshIndex);

private: /* For Meshes */
	_uint m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;

private: /* For. Materials */
	_uint	m_iNumMaterials = { 0 };
	vector<MESHMATERIAL>	m_Materials;

private: /* For. Bones */
	vector<class CBone*>	m_Bones;
public:
	typedef vector<class CBone*> BONES;

private: /* For. Animaitons*/
	TYPE	m_eAnimType = { TYPE_END };
	_uint	m_iCurrentAnimIndex = { 0 };
	_uint	m_iNumAnimations = { 0 };
	vector<class CAnimation*>	m_Animations;

private:
	_float4x4	m_PivotMatrix;

private:
	HRESULT LoadModel(const _tchar* pModelFilePath, _Inout_ SCENE& tScene);
	HRESULT Ready_Meshes(const SCENE& tScene, _fmatrix PivotMatrix);
	HRESULT Ready_Materials(const SCENE& tScene);
	HRESULT Ready_Bones(const NODE* pNode, CBone* pParent);
	HRESULT Ready_Animations(const SCENE& tScene);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pModelFilePath, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

