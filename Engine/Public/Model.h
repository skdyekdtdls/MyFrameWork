#pragma once

#include "Component.h"
#include "ISerializable.h"
BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}
	class CAnimation* Get_Animation(int iIndex) {
		if (iIndex >= m_iNumAnimations)
			return nullptr;

		return m_Animations[iIndex];
	}
	CAnimation* Get_AnimationByName(string strName);
#ifdef _DEBUG
	void CoutRootNodePos();
#endif

	_float3 Get_RootTranslation();
	_uint Get_NumAnimation() {
		return m_iNumAnimations;
	}

	_uint Get_NumBones() {
		return m_iNumBones;
	}

	// 시간복잡도 O(1)
	class CBone* GetBoneByIndex(_uint iIndex) {
		return m_Bones[iIndex];
	}

	// 시간복잡도 O(n)
	class CBone* GetBoneByName(string strName);
	class CBone* GetRootNode() {
		if (-1 == m_RootIndex) return nullptr;
		return m_Bones[m_RootIndex];
	}
public:
	void Set_AnimByIndex(_uint iAnimIndex);
	void Set_AnimByName(const char* pName);

	// 객체 프로토타입 만들 때 최초에 1번만 부른다.
	void Set_RootNode(_uint iBoneIndex);
	void Set_PivotMatrix(_fmatrix PivotMatrix) {
		XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
	}

	void SaveAssimp(HANDLE hFile, DWORD & dwByte);
	void LoadAssimp(const char* pFileName);

public:
	virtual HRESULT Initialize_Prototype(const aiScene * pAIScene, TYPE eType, fs::path pModelFilePath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	virtual HRESULT Render(_uint iMeshIndex);

public:
	void Play_Animation(_double TimeDelta);
	_bool IsAnimationFinished();

	// 인자로 아무것도 들어오지 않거나 음수가 들어오면 현재 애니메이션 리셋
	void ResetAnimation(_int iIndex = -1);

public:
	HRESULT Bind_Material(class CShader* pShader, const char* pConstantName, _uint iMeshIndex, TextureType MaterialType);
	HRESULT Bind_BoneMatrices(class CShader* pShader, const char* pConstantName, _uint iMeshIndex);

public: /* For.Bones*/
	_uint						m_iNumBones = { 0 };
	vector<class CBone*>		m_Bones;
	_int						m_RootIndex = { -1 };

public:
	typedef vector<class CBone*>	BONES;

private: /* For.Meshes  */
	_uint					m_iNumMeshes = { 0 };
	vector<class CMesh*>	m_Meshes;

private: /* For.Materials */
	_uint					m_iNumMaterials = { 0 };
	vector<MESHMATERIAL>	m_Materials;

private:
	_uint						m_iPrevAnimIndex = { 0 };
	_uint						m_iCurrentAnimIndex = { 0 };
	_uint						m_iNumAnimations = { 0 };
	vector<class CAnimation*>	m_Animations;

public:
	_float4x4		m_PivotMatrix;
	TYPE			m_eAnimType = { TYPE_END };
	_double			m_InterTimeAcc = { 0.0 };
private:
	HRESULT LoadModel(const _tchar* pModelFilePath, _Inout_ SCENE& tScene, _Out_ fs::path& ModelFilePath, _fmatrix PivotMatrix);
	fs::path FindModelDirecotyPath(fs::path ModelPath, fs::path Stem);
	fs::path FindDATFile(fs::path ModelPath);

private: /*For Assimp*/
	HRESULT Ready_Bones(aiNode* pNode, class CBone* pParent);
	HRESULT Ready_Meshes(const aiScene* pAIScene, TYPE eType, _fmatrix PivotMatrix);
	HRESULT Ready_Materials(const aiScene* pAIScene, fs::path pModelFilePath);
	HRESULT Ready_Animations(const aiScene* pAIScene);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix, _int RootIndex = -1);
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiScene* pAIScene, TYPE eType, const _tchar* pModelFilePath, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END