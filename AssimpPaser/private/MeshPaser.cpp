#include "..\public\MeshPaser.h"
#include "BonePaser.h"

HRESULT CMeshPaser::Pasing(HANDLE hFile, DWORD& dwByte, const aiScene* pAIScene, ANIM_TYPE eAnimType, const BONES& BonesName)
{
	WriteUINT(pAIScene->mNumMeshes);

	for (_uint i = 0; i < pAIScene->mNumMeshes; ++i)
	{
		Pasing_SubMesh(hFile, dwByte, pAIScene->mMeshes[i], eAnimType, BonesName);
	}

	return S_OK;
}

void CMeshPaser::Pasing_SubMesh(HANDLE hFile, DWORD& dwByte, const aiMesh* pAIMesh, ANIM_TYPE eAnimType, const BONES& BonesName)
{
	// m_iMaterialIndex
	WriteUINT(pAIMesh->mMaterialIndex);
	
	// m_szName
	WriteCHAR(pAIMesh->mName.data);
	
	// m_iNumVertices
	WriteUINT(pAIMesh->mNumVertices);
	
	// m_iNumIndices
	WriteUINT(pAIMesh->mNumFaces);
	
	if (ANIM_TYPE::NONANIM == eAnimType)
		Pasing_Vertex_NonAnim(hFile, pAIMesh, dwByte);
	else
		Pasing_Vertex_Anim(hFile, pAIMesh, dwByte, BonesName);

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		WriteUINT(pAIMesh->mFaces[i].mIndices[0]);
		WriteUINT(pAIMesh->mFaces[i].mIndices[1]);
		WriteUINT(pAIMesh->mFaces[i].mIndices[2]);
	}
}

void CMeshPaser::Pasing_Vertex_NonAnim(HANDLE hFile, const aiMesh* pAIMesh, DWORD& dwByte)
{
	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
	{
		WriteAiVector3D(pAIMesh->mVertices[i]);
		WriteAiVector3D(pAIMesh->mNormals[i]);
		WriteAiVector3D(pAIMesh->mTextureCoords[0][i]);
		WriteAiVector3D(pAIMesh->mTangents[i]);
	}
}

void CMeshPaser::Pasing_Vertex_Anim(HANDLE hFile, const aiMesh* pAIMesh, DWORD& dwByte, const BONES& BonesName)
{
	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
	{
		WriteAiVector3D(pAIMesh->mVertices[i]);
		WriteAiVector3D(pAIMesh->mNormals[i]);
		WriteAiVector3D(pAIMesh->mTextureCoords[0][i]);
		WriteAiVector3D(pAIMesh->mTangents[i]);
	}

	WriteUINT(pAIMesh->mNumBones);

	for (size_t i = 0; i < pAIMesh->mNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		_uint		iBoneIndex = 0;

		auto	iter = find_if(BonesName.begin(), BonesName.end(), [&](const char* pValue)
			{
				if (0 != strcmp(pAIBone->mName.data, pValue))
				{
					++iBoneIndex;
					return false;
				}
				else
					return true;
			});

		WriteAiMatrix4x4(pAIBone->mOffsetMatrix.Transpose());
		WriteUINT(iBoneIndex);

		// °¡ÁßÄ¡
		WriteUINT(pAIBone->mNumWeights);
		for (size_t j = 0; j < pAIBone->mNumWeights; ++j)
		{
			aiVertexWeight	AIVertexWeight = pAIBone->mWeights[j];

			WriteUINT(AIVertexWeight.mVertexId);
			WriteFloat(AIVertexWeight.mWeight);
		}
	}
}