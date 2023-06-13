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

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const _tchar* pModelFilePath, _fmatrix PivotMatrix);
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

private:
	_uint	m_iCurrentAnimIndex = { 0 };
	_uint	m_iNumAnimations = { 0 };
	vector<class CAnimation*>	m_Animations;

private:
	HRESULT Ready_Meshes(HANDLE hFile, DWORD& dwByte, TYPE eType, _fmatrix PivotMatrix);
	HRESULT Ready_Materials(HANDLE hFile, DWORD& dwByte);
	HRESULT Ready_Bones(HANDLE hFile, DWORD& dwByte, CBone* pParent);
	HRESULT Ready_Animations(HANDLE hFile, DWORD& dwByte);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

