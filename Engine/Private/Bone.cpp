#include "..\Public\Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const NODE* pNode, CBone* pParent, _uint iIndex)
{
	strcpy_s(m_szName, pNode->m_Name.m_data);
	memcpy(&m_TransformationMatrix, &pNode->m_Transformation, sizeof(_float4x4));
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	m_OffsetMatrix = m_CombinedTransformationMatrix;
	m_iParentIndex = (nullptr == pParent) ? -1 : pParent->m_iIndex;
	m_iIndex = iIndex;

	return S_OK;
}

void CBone::Invalidate_CombinedTransformationMatrix(const CModel::BONES& Bones)
{
	if (-1 == m_iParentIndex)
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix)* XMLoadFloat4x4(&Bones[m_iParentIndex]->m_CombinedTransformationMatrix));
	}	
}

CBone* CBone::Create(const NODE* pNode, CBone* pParent, _uint iIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pNode, pParent, iIndex)))
	{
		MSG_BOX("Failed to Created CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free(void)
{
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}
