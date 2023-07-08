#include "ClintModel.h"
#include "GameInstance.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Animation.h"
#include "GameObject.h"
#include "Clint.h"

ClintModel::ClintModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CModel(pDevice, pContext)
{
	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_iCurrentAnimIndex[i] = { 64 };

	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_iPrevAnimIndex[i] = { 0 };

	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_InterTimeAcc[i] = { 0.0 };
}

ClintModel::ClintModel(const ClintModel& rhs)
	: CModel(rhs)
{
	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_iCurrentAnimIndex[i] = rhs.m_iCurrentAnimIndex[i];

	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_iPrevAnimIndex[i] = rhs.m_iPrevAnimIndex[i];

	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
		m_InterTimeAcc[i] = rhs.m_InterTimeAcc[i];



	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
	{
		for (auto& pOriginalAnimation : m_Animations)
			m_Animationss[i].push_back(pOriginalAnimation->Clone());
	}
}

void ClintModel::GroupingBones()
{
	// 본인을 포함하여 ik_hand_root(7)의 자식들은 전부 상체로간다, spine_01(12)자식들은 상체
	// 본인을 포함하여 ik_foot_root(4)의 자식들은 전부 하체로간다, thigh_l(88)자식들은 하체, thigh_r(94)자식들은 하체

	for (auto& Bone : m_Bones)
	{
		CBone* pBone = Bone;
		_uint iBoneIndex = pBone->GetIndex();

		// 뼈가 ik_hand_root(7)이거나 spine_01(12)이면 상체
		if (7 == iBoneIndex || 11 == iBoneIndex || 12 == iBoneIndex)
		{
			Bone->Set_Body(UPPER);
			continue;
		}

		// 뼈가 ik_foot_root(4)이거나 thigh_l(88) 이거나 thigh_r(94)이면 하체
		if (3 == iBoneIndex || 4 == iBoneIndex || 88 == iBoneIndex || 94 == iBoneIndex)
		{
			Bone->Set_Body(LOWER);
			continue;
		}

		while (pBone->HasParent()) // 부모 뼈가 있으면 루프
		{
			_uint iParentIndex = pBone->GetParentIndex();

			// 부모가 ik_hand_root(7)이거나 spine_01(12)이면 상체
			if (7 == iParentIndex || 12 == iParentIndex)
			{
				Bone->Set_Body(UPPER);
				break;
			}

			// 부모가 ik_foot_root(4)이거나 thigh_l(88) 이거나 thigh_r(94)이면 하체
			if (4 == iParentIndex || 88 == iParentIndex || 94 == iParentIndex)
			{
				Bone->Set_Body(LOWER);
				break;
			}

			pBone = m_Bones[iParentIndex];
		}
	}
}

HRESULT ClintModel::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	Add_TimeLineEvent("Clint_basic_shoot", L"Test", TIMELINE_EVENT(30.0, [this]() {
		m_Animationss[UPPER][5]->Reset();
		}));

	return S_OK;
}

void ClintModel::Set_AnimByIndex(_uint iAnimIndex, BODY eBody)
{
	if (iAnimIndex >= m_iNumAnimations)
		return;

	if (m_iCurrentAnimIndex[(_int)eBody] == iAnimIndex)
		return;

	if (eBody >= BODY_END)
	{
		Set_AnimByIndex(iAnimIndex, UPPER);
		Set_AnimByIndex(iAnimIndex, LOWER);
		return;
	}

	m_Animationss[(_uint)eBody][iAnimIndex]->Reset();
	m_InterTimeAcc[eBody] = 0.0;
	m_iCurrentAnimIndex[(_int)eBody] = iAnimIndex;
}

void ClintModel::ResetAnimation(_int iIndex, BODY eBody)
{
	if (iIndex < 0)
	{
		m_Animationss[eBody][m_iCurrentAnimIndex[eBody]]->Reset();
		return;
	}
	m_Animationss[eBody][iIndex]->Reset();
}

void ClintModel::Play_Animation(_double TimeDelta)
{
	if (m_eAnimType == TYPE_NONANIM)
		return;

	for (_uint i = 0; i < BODY_END; ++i)
	{
		if (m_iPrevAnimIndex[i] != m_iCurrentAnimIndex[i])
		{
			m_InterTimeAcc[i] += TimeDelta;

			if (m_InterTimeAcc[i] > 0.2)
			{
				m_iPrevAnimIndex[i] = m_iCurrentAnimIndex[i];
				m_Animationss[i][m_iCurrentAnimIndex[i]]->Reset();
			}
			else
			{
				m_Animationss[i][m_iCurrentAnimIndex[i]]->InterAnimation_TransfomationMatrix(m_Bones, m_InterTimeAcc[i], (BODY)i);
			}
		}
		else
		{
			m_Animationss[i][m_iCurrentAnimIndex[i]]->Invalidate_TransformationMatrix(m_Bones, TimeDelta, (BODY)i);
		}
	}

	m_RootMoveDistance = m_Bones[m_RootIndex]->GetTransformationMatrix_43();
	//m_Bones[12]->SetPosition(m_Bones[m_RootIndex]->GetPosition());
	m_Bones[m_RootIndex]->FixBone();

	for (auto& Bone : m_Bones)
	{
		Bone->Invalidate_CombinedTransformationMatrix(m_Bones);
	}
}

_bool ClintModel::IsAnimationFinished(BODY eBody)
{
	if (m_Animationss[eBody][m_iCurrentAnimIndex[eBody]]->IsFinished())
	{
		m_PrevMoveDistance = { 0.f };
		m_RootMoveDistance = { 0.f };
		return true;
	}

	return false;
}

ClintModel* ClintModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _fmatrix PivotMatrix, _int RootIndex)
{
	ClintModel* pModel = new ClintModel(pDevice, pContext);

	pModel->Set_PivotMatrix(PivotMatrix);
	pModel->Set_RootNode(RootIndex);

	return pModel;
}

CComponent* ClintModel::Clone(void* pArg)
{
	ClintModel* pInstance = new ClintModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned ClintModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void ClintModel::Free()
{
	__super::Free();

	for (size_t i = 0; i < (size_t)BODY::BODY_END; ++i)
	{
		for (auto& pOriginalAnimation : m_Animationss[i])
			Safe_Release(pOriginalAnimation);
		m_Animationss[i].clear();
	}
}