#include "Assimp_Struct.h"
#include "Engine_Defines.h"
#include <unordered_set>
META_DATA_ENTRY::~META_DATA_ENTRY()
{

}

void META_DATA_ENTRY::Serialization(aiMetadataEntry* pAIMetadataEntry, HANDLE hFile, DWORD& dwByte)
{
	if (nullptr == pAIMetadataEntry)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);	

	WriteVoid(&pAIMetadataEntry->mType, sizeof(pAIMetadataEntry->mType));

	unsigned int iSize = 0;
	switch (pAIMetadataEntry->mType)
	{
	case AI_BOOL:
		iSize = sizeof(_bool);
		break;
	case AI_INT32:
		iSize = sizeof(int);
		break;
	case AI_UINT64:
		iSize = sizeof(unsigned long long);
		break;
	case AI_FLOAT:
		iSize = sizeof(_float);
		break;
	case AI_DOUBLE:
		iSize = sizeof(_double);
		break;
	case AI_AISTRING:
		iSize = sizeof(char) * 1024Ui64;
		break;
	case AI_AIVECTOR3D:
		iSize = sizeof(_float3);
		break;
	case AI_AIMETADATA:
		iSize = sizeof(META_DATA);
		break;
	case AI_INT64:
		iSize = sizeof(long long);
		break;
	case AI_UINT32:
		iSize = sizeof(unsigned int);
		break;
	case AI_META_MAX:
		iSize = 0;
		break;
	}
	WriteVoid(&iSize, sizeof(iSize));
	WriteVoid(&pAIMetadataEntry->mData, iSize);
}

void META_DATA_ENTRY::Deserialization(META_DATA_ENTRY* tMetaDataEntry, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMetaDataEntry->m_eType, sizeof(tMetaDataEntry->m_eType));
	unsigned int iSize = 0;

	ReadVoid(&iSize, sizeof(iSize));
	ReadVoid(&tMetaDataEntry->m_Data, iSize);
}

META_DATA::~META_DATA()
{
	Safe_Delete_Array(m_Keys);
	Safe_Delete_Array(m_Values);
}

void META_DATA::Serialization(aiMetadata* pAIMetadata, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMetadata == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);
	WriteVoid(&pAIMetadata->mNumProperties, sizeof(pAIMetadata->mNumProperties));

	for (size_t i = 0; i < pAIMetadata->mNumProperties; ++i)
		AI_STRING::Serialization(&pAIMetadata->mKeys[i], hFile, dwByte);

	for (size_t i = 0; i < pAIMetadata->mNumProperties; ++i)
		META_DATA_ENTRY::Serialization(&pAIMetadata->mValues[i], hFile, dwByte);
}

void META_DATA::Deserialization(META_DATA* tMetaData, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMetaData->m_NumProperties, sizeof(tMetaData->m_NumProperties));

	tMetaData->m_Keys = new AI_STRING[tMetaData->m_NumProperties];
	for (size_t i = 0; i < tMetaData->m_NumProperties; ++i)
		AI_STRING::Deserialization(&tMetaData->m_Keys[i], hFile, dwByte);
	
	tMetaData->m_Values = new META_DATA_ENTRY[tMetaData->m_NumProperties];
	for (size_t i = 0; i < tMetaData->m_NumProperties; ++i)
		META_DATA_ENTRY::Deserialization(&tMetaData->m_Values[i], hFile, dwByte);
}

NODE::~NODE()
{
	Safe_Delete(m_MetaData);
	Safe_Delete_Array(m_Meshes);
	Safe_Delete_Array(m_Children);
	for (size_t i = 0; i < m_NumChildren; ++i)
	{
		Safe_Delete(m_Children[i]);
	}
	//Safe_Delete(m_Parent);
}

void NODE::Serialization(aiNode* pAINode, HANDLE hFile, DWORD& dwByte)
{
	if (pAINode == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAINode->mName, hFile, dwByte);
	WriteVoid(&pAINode->mTransformation.Transpose(), sizeof(pAINode->mTransformation.Transpose()));

	NODE::Serialization(pAINode->mParent, hFile, dwByte);

	WriteVoid(&pAINode->mNumChildren, sizeof(pAINode->mNumChildren));
	for (size_t i = 0; i < pAINode->mNumChildren; ++i)
	{
		NODE::Serialization(pAINode->mChildren[i], hFile, dwByte);
	}
	for (size_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Serialization(pAINode->mChildren[i], hFile, dwByte);
	}

	WriteVoid(&pAINode->mNumMeshes, sizeof(pAINode->mNumMeshes));
	for (size_t i = 0; i < pAINode->mNumMeshes; ++i)
		WriteVoid(&pAINode->mMeshes[i], sizeof(pAINode->mMeshes[i]));

	META_DATA::Serialization(pAINode->mMetaData, hFile, dwByte);
}

void NODE::Deserialization(NODE* tNode, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tNode->m_Name, hFile, dwByte);

	ReadVoid(&tNode->m_Transformation, sizeof(tNode->m_Transformation));

	//tNode->m_Parent = new NODE;
	//NODE::Deserialization(tNode->m_Parent, hFile, dwByte);

	ReadVoid(&tNode->m_NumChildren, sizeof(tNode->m_NumChildren));
	tNode->m_Children = new NODE*[tNode->m_NumChildren];
	for (size_t i = 0; i < tNode->m_NumChildren; ++i)
	{
		tNode->m_Children[i] = new NODE;
		NODE::Deserialization(tNode->m_Children[i], hFile, dwByte);
	}

	ReadVoid(&tNode->m_NumMeshes, sizeof(tNode->m_NumMeshes));
	tNode->m_Meshes = new unsigned int[tNode->m_NumMeshes];
	for (size_t i = 0; i < tNode->m_NumMeshes; ++i)
	{
		ReadVoid(&tNode->m_Meshes[i], sizeof(tNode->m_Meshes[i]));
	}

	tNode->m_MetaData = new META_DATA;
	META_DATA::Deserialization(tNode->m_MetaData, hFile, dwByte);
}

FACE::~FACE()
{
	Safe_Delete_Array(m_Indices);
}

void FACE::Serialization(aiFace* pAIFace, HANDLE hFile, DWORD& dwByte)
{
	if (pAIFace == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIFace->mNumIndices, sizeof(pAIFace->mNumIndices));
	for(size_t i = 0; i < pAIFace->mNumIndices; ++i)
		WriteVoid(&pAIFace->mIndices[i], sizeof(pAIFace->mIndices[i]));
}

void FACE::Deserialization(FACE* tFace, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	WriteVoid(&tFace->m_NumIndices, sizeof(tFace->m_NumIndices));
	tFace->m_Indices = new unsigned int[tFace->m_NumIndices];
	for (size_t i = 0; i < tFace->m_NumIndices; ++i)
		WriteVoid(&tFace->m_Indices[i], sizeof(tFace->m_Indices[i]));
}

AABB::~AABB()
{
}

void AABB::Serialization(aiAABB* pAIAABB, HANDLE hFile, DWORD& dwByte)
{
	if (pAIAABB == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIAABB->mMin, sizeof(pAIAABB->mMin));
	WriteVoid(&pAIAABB->mMax, sizeof(pAIAABB->mMax));
}

void AABB::Deserialization(AABB* tAABB, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tAABB->m_Min, sizeof(tAABB->m_Min));
	ReadVoid(&tAABB->m_Max, sizeof(tAABB->m_Max));
}

MESH::~MESH()
{
	Safe_Delete_Array(m_TextureCoordsNames);
	for (size_t i = 0; i < m_NumAnimMeshes; ++i)
		Safe_Delete(m_AnimMeshes[i]);
	Safe_Delete_Array(m_AnimMeshes);
	for (size_t i = 0; i < m_NumBones; ++i)
		Safe_Delete(m_Bones[i]);
	Safe_Delete_Array(m_Bones);
	Safe_Delete_Array(m_Faces);
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		Safe_Delete_Array(m_TextureCoords[i]);
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
		Safe_Delete_Array(m_Colors[i]);
	Safe_Delete_Array(m_Bitangents);
	Safe_Delete_Array(m_Tangents);
	Safe_Delete_Array(m_Normals);
	Safe_Delete_Array(m_Vertices);
}

void MESH::Serialization(aiMesh* pAIMesh, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMesh == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIMesh->mPrimitiveTypes, sizeof(pAIMesh->mPrimitiveTypes));
	WriteVoid(&pAIMesh->mNumVertices, sizeof(pAIMesh->mNumVertices));
	WriteVoid(&pAIMesh->mNumFaces, sizeof(pAIMesh->mNumFaces));
	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteVoid(&pAIMesh->mVertices[i], sizeof(pAIMesh->mVertices[i]));

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteVoid(&pAIMesh->mNormals[i], sizeof(pAIMesh->mNormals[i]));

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteVoid(&pAIMesh->mTangents[i], sizeof(pAIMesh->mTangents[i]));

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteVoid(&pAIMesh->mBitangents[i], sizeof(pAIMesh->mBitangents[i]));


	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIMesh->mColors[i][j], sizeof(pAIMesh->mColors[i][j]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIMesh->mTextureCoords[i], sizeof(pAIMesh->mTextureCoords[i]));
		}
	}
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		WriteVoid(&pAIMesh->mNumUVComponents[i], sizeof(pAIMesh->mNumUVComponents[i]));
	}
	
	for (size_t i = 0; i < pAIMesh->mNumFaces; ++i)
	{
		FACE::Serialization(&pAIMesh->mFaces[i], hFile, dwByte);
	}

	WriteVoid(&pAIMesh->mNumBones, sizeof(pAIMesh->mNumBones));
	for (size_t i = 0; i < pAIMesh->mNumBones; ++i)
	{
		BONE::Serialization(pAIMesh->mBones[i], hFile, dwByte);
	}

	WriteVoid(&pAIMesh->mMaterialIndex, sizeof(pAIMesh->mMaterialIndex));
	AI_STRING::Serialization(&pAIMesh->mName, hFile, dwByte);

	WriteVoid(&pAIMesh->mNumAnimMeshes, sizeof(pAIMesh->mNumAnimMeshes));
	for (size_t i = 0; i < pAIMesh->mNumAnimMeshes; ++i)
	{
		ANIM_MESH::Serialization(pAIMesh->mAnimMeshes[i], hFile, dwByte);
	}

	WriteVoid(&pAIMesh->mMethod, sizeof(pAIMesh->mMethod));
	AABB::Serialization(&pAIMesh->mAABB, hFile, dwByte);
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		AI_STRING::Serialization(pAIMesh->mTextureCoordsNames[i], hFile, dwByte);
}

void MESH::Deserialization(MESH* tMesh, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMesh->m_PrimitiveTypes, sizeof(tMesh->m_PrimitiveTypes));
	ReadVoid(&tMesh->m_NumVertices, sizeof(tMesh->m_NumVertices));
	ReadVoid(&tMesh->m_NumFaces, sizeof(tMesh->m_NumFaces));

	
	tMesh->m_Vertices = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadVoid(&tMesh->m_Vertices[i], sizeof(tMesh->m_Vertices[i]));
	}
	
	tMesh->m_Normals = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadVoid(&tMesh->m_Normals[i], sizeof(tMesh->m_Normals[i]));
	}

	tMesh->m_Tangents = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadVoid(&tMesh->m_Tangents[i], sizeof(tMesh->m_Tangents[i]));
	}

	tMesh->m_Bitangents = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadVoid(&tMesh->m_Bitangents[i], sizeof(tMesh->m_Bitangents[i]));
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		tMesh->m_Colors[i] = new _float4[tMesh->m_NumVertices];
		for (size_t j = 0; j < tMesh->m_NumVertices; ++j)
		{
			ReadVoid(&tMesh->m_Colors[i][j], sizeof(tMesh->m_Colors[i][j]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		tMesh->m_TextureCoords[i] = new _float3[tMesh->m_NumVertices];
		for (size_t j = 0; j < tMesh->m_NumVertices; ++j)
		{
			ReadVoid(&tMesh->m_TextureCoords[i][j], sizeof(tMesh->m_TextureCoords[i][j]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		ReadVoid(&tMesh->m_NumUVComponents[i], sizeof(tMesh->m_NumUVComponents[i]));
	}

	tMesh->m_Faces = new FACE[tMesh->m_NumFaces];
	for (size_t i = 0; i < tMesh->m_NumFaces; ++i)
	{
		FACE::Deserialization(tMesh->m_Faces, hFile, dwByte);
	}

	ReadVoid(&tMesh->m_NumBones, sizeof(tMesh->m_NumBones));
	tMesh->m_Bones = new BONE*[tMesh->m_NumBones];
	for (size_t i = 0; i < tMesh->m_NumBones; ++i)
	{
		tMesh->m_Bones[i] = new BONE;
		BONE::Deserialization(tMesh->m_Bones[i], hFile, dwByte);
	}

	ReadVoid(&tMesh->m_MaterialIndex, sizeof(tMesh->m_MaterialIndex));
	AI_STRING::Deserialization(&tMesh->m_Name, hFile, dwByte);
	ReadVoid(&tMesh->m_NumAnimMeshes, sizeof(tMesh->m_NumAnimMeshes));

	tMesh->m_AnimMeshes = new ANIM_MESH*[tMesh->m_NumAnimMeshes];
	for (size_t i = 0; i < tMesh->m_NumAnimMeshes; ++i)
	{
		tMesh->m_AnimMeshes[i] = new ANIM_MESH;
		ANIM_MESH::Deserialization(tMesh->m_AnimMeshes[i], hFile, dwByte);
	}

	ReadVoid(&tMesh->m_eMethod, sizeof(tMesh->m_eMethod));
	AABB::Deserialization(&tMesh->m_AABB, hFile, dwByte);

	tMesh->m_TextureCoordsNames = new AI_STRING [AI_MAX_NUMBER_OF_TEXTURECOORDS];
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		ReadVoid(&tMesh->m_TextureCoordsNames[i], sizeof(tMesh->m_TextureCoordsNames[i]));
	}
}

MATERIAL::~MATERIAL()
{
	for (size_t i = 0; i < m_NumProperties; ++i)
		Safe_Delete(m_Properties[i]);
	Safe_Delete_Array(m_Properties);
}

void MATERIAL::Serialization(aiMaterial* pAIMaterial, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMaterial == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIMaterial->mNumProperties, sizeof(pAIMaterial->mNumProperties));
	WriteVoid(&pAIMaterial->mNumAllocated, sizeof(pAIMaterial->mNumAllocated));

	for(size_t i = 0; i < pAIMaterial->mNumProperties; ++i)
		MATERIAL_PROPERTY::Serialization(pAIMaterial->mProperties[i], hFile, dwByte);
}

void MATERIAL::Deserialization(MATERIAL* tMaterial, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMaterial->m_NumProperties, sizeof(tMaterial->m_NumProperties));
	ReadVoid(&tMaterial->m_NumAllocated, sizeof(tMaterial->m_NumAllocated));

	tMaterial->m_Properties = new MATERIAL_PROPERTY * [tMaterial->m_NumProperties];
	for (size_t i = 0; i < tMaterial->m_NumProperties; ++i)
	{
		tMaterial->m_Properties[i] = new MATERIAL_PROPERTY;
		MATERIAL_PROPERTY::Deserialization(tMaterial->m_Properties[i], hFile, dwByte);
	}
}

MATERIAL_PROPERTY::~MATERIAL_PROPERTY()
{
}

void MATERIAL_PROPERTY::Serialization(aiMaterialProperty* pAIMaterialProperty, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMaterialProperty == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIMaterialProperty->mKey, hFile, dwByte);
	WriteVoid(&pAIMaterialProperty->mSemantic, sizeof(pAIMaterialProperty->mSemantic));
	WriteVoid(&pAIMaterialProperty->mIndex, sizeof(pAIMaterialProperty->mIndex));
	WriteVoid(&pAIMaterialProperty->mDataLength, sizeof(pAIMaterialProperty->mDataLength));
	WriteVoid(&pAIMaterialProperty->mType, sizeof(pAIMaterialProperty->mType));
	WriteVoid(pAIMaterialProperty->mData, pAIMaterialProperty->mDataLength);
}

void MATERIAL_PROPERTY::Deserialization(MATERIAL_PROPERTY* tMaterialProperty, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tMaterialProperty->m_Key, hFile, dwByte);
	ReadVoid(&tMaterialProperty->m_Semantic, sizeof(tMaterialProperty->m_Semantic));
	ReadVoid(&tMaterialProperty->m_Index, sizeof(tMaterialProperty->m_Index));
	ReadVoid(&tMaterialProperty->m_DataLength, sizeof(tMaterialProperty->m_DataLength));
	ReadVoid(&tMaterialProperty->m_eType, sizeof(tMaterialProperty->m_eType));
	ReadVoid(tMaterialProperty->m_Data, tMaterialProperty->m_DataLength);
}

// 애니메이션 부터 하면됨.

ANIMATION::~ANIMATION()
{
	for (size_t i = 0; i < m_NumMorphMeshChannels; ++i)
		Safe_Delete(m_MorphMeshChannels[i]);
	Safe_Delete_Array(m_MorphMeshChannels);

	for (size_t i = 0; i < m_NumMeshChannels; ++i)
		Safe_Delete(m_MeshChannels[i]);
	Safe_Delete_Array(m_MeshChannels);

	for (size_t i = 0; i < m_NumChannels; ++i)
		Safe_Delete(m_Channels[i]);
	Safe_Delete_Array(m_Channels);
}

void ANIMATION::Serialization(aiAnimation* pAIAnimation, HANDLE hFile, DWORD& dwByte)
{
	if (pAIAnimation == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIAnimation->mName, hFile, dwByte);
	WriteVoid(&pAIAnimation->mDuration, sizeof(pAIAnimation->mDuration));
	WriteVoid(&pAIAnimation->mTicksPerSecond, sizeof(pAIAnimation->mTicksPerSecond));
	WriteVoid(&pAIAnimation->mNumChannels, sizeof(pAIAnimation->mNumChannels));
	for(size_t i = 0; i < pAIAnimation->mNumChannels; ++i)
		NODE_ANIM::Serialization(pAIAnimation->mChannels[i], hFile, dwByte);
	WriteVoid(&pAIAnimation->mNumMeshChannels, sizeof(pAIAnimation->mNumMeshChannels));
	for (size_t i = 0; i < pAIAnimation->mNumMeshChannels; ++i)
		MESH_ANIM::Serialization(pAIAnimation->mMeshChannels[i], hFile, dwByte);
	WriteVoid(&pAIAnimation->mNumMorphMeshChannels, sizeof(pAIAnimation->mNumMorphMeshChannels));
	for (size_t i = 0; i < pAIAnimation->mNumMorphMeshChannels; ++i)
		MESH_MORPH_ANIM::Serialization(pAIAnimation->mMorphMeshChannels[i], hFile, dwByte);
}

void ANIMATION::Deserialization(ANIMATION* tAnimation, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tAnimation->m_Name, hFile, dwByte);
	WriteVoid(&tAnimation->m_Duration, sizeof(tAnimation->m_Duration));
	WriteVoid(&tAnimation->m_TicksPerSecond, sizeof(tAnimation->m_TicksPerSecond));
	WriteVoid(&tAnimation->m_NumChannels, sizeof(tAnimation->m_NumChannels));

	tAnimation->m_Channels = new NODE_ANIM * [tAnimation->m_NumChannels];
	for (size_t i = 0; i < tAnimation->m_NumChannels; ++i)
	{
		tAnimation->m_Channels[i] = new NODE_ANIM[tAnimation->m_NumChannels];
		NODE_ANIM::Deserialization(tAnimation->m_Channels[i], hFile, dwByte);
	}

	WriteVoid(&tAnimation->m_NumMeshChannels, sizeof(tAnimation->m_NumMeshChannels));
	tAnimation->m_MeshChannels = new MESH_ANIM * [tAnimation->m_NumMeshChannels];
	for (size_t i = 0; i < tAnimation->m_NumMeshChannels; ++i)
	{
		tAnimation->m_MeshChannels[i] = new MESH_ANIM;
		MESH_ANIM::Deserialization(tAnimation->m_MeshChannels[i], hFile, dwByte);
	}

	WriteVoid(&tAnimation->m_NumMorphMeshChannels, sizeof(tAnimation->m_NumMorphMeshChannels));
	tAnimation->m_MorphMeshChannels = new MESH_MORPH_ANIM * [tAnimation->m_NumMorphMeshChannels];
	for (size_t i = 0; i < tAnimation->m_NumMorphMeshChannels; ++i)
	{
		tAnimation->m_MorphMeshChannels[i] = new MESH_MORPH_ANIM[tAnimation->m_NumMorphMeshChannels];
		MESH_MORPH_ANIM::Deserialization(tAnimation->m_MorphMeshChannels[i], hFile, dwByte);
	}
}

NODE_ANIM::~NODE_ANIM()
{
	Safe_Delete_Array(m_ScalingKeys);
	Safe_Delete_Array(m_RotationKeys);
	Safe_Delete_Array(m_PositionKeys);
}

void NODE_ANIM::Serialization(aiNodeAnim* pAINodeAnim, HANDLE hFile, DWORD& dwByte)
{
	if (pAINodeAnim == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAINodeAnim->mNodeName, hFile, dwByte);
	WriteVoid(&pAINodeAnim->mNumPositionKeys, sizeof(pAINodeAnim->mNumPositionKeys));
	VECTOR_KEY::Serialization(pAINodeAnim->mPositionKeys, hFile, dwByte);
	WriteVoid(&pAINodeAnim->mNumRotationKeys, sizeof(pAINodeAnim->mNumRotationKeys));
	QUAT_KEY::Serialization(pAINodeAnim->mRotationKeys, hFile, dwByte);
	WriteVoid(&pAINodeAnim->mNumScalingKeys, sizeof(pAINodeAnim->mNumScalingKeys));
	VECTOR_KEY::Serialization(pAINodeAnim->mScalingKeys, hFile, dwByte);
	WriteVoid(&pAINodeAnim->mPreState, sizeof(pAINodeAnim->mPreState));
	WriteVoid(&pAINodeAnim->mPostState, sizeof(pAINodeAnim->mPostState));
}

void NODE_ANIM::Deserialization(NODE_ANIM* tNodeAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tNodeAnim->m_NodeName, hFile, dwByte);

	ReadVoid(&tNodeAnim->m_NumPositionKeys, sizeof(tNodeAnim->m_NumPositionKeys));
	tNodeAnim->m_PositionKeys = new VECTOR_KEY[tNodeAnim->m_NumPositionKeys];
	for(size_t i = 0; i < tNodeAnim->m_NumPositionKeys; ++i)
		VECTOR_KEY::Deserialization(&tNodeAnim->m_PositionKeys[i], hFile, dwByte);

	ReadVoid(&tNodeAnim->m_NumRotationKeys, sizeof(tNodeAnim->m_NumRotationKeys));
	tNodeAnim->m_RotationKeys = new QUAT_KEY[tNodeAnim->m_NumRotationKeys];
	for (size_t i = 0; i < tNodeAnim->m_NumRotationKeys; ++i)
		QUAT_KEY::Deserialization(&tNodeAnim->m_RotationKeys[i], hFile, dwByte);

	ReadVoid(&tNodeAnim->m_NumScalingKeys, sizeof(tNodeAnim->m_NumScalingKeys));
	tNodeAnim->m_ScalingKeys = new VECTOR_KEY[tNodeAnim->m_NumScalingKeys];
	for (size_t i = 0; i < tNodeAnim->m_NumScalingKeys; ++i)
		VECTOR_KEY::Deserialization(&tNodeAnim->m_ScalingKeys[i], hFile, dwByte);
	ReadVoid(&tNodeAnim->m_ePreState, sizeof(tNodeAnim->m_ePreState));
	ReadVoid(&tNodeAnim->m_ePostState, sizeof(tNodeAnim->m_ePostState));
}

VECTOR_KEY::~VECTOR_KEY()
{
}

void VECTOR_KEY::Serialization(aiVectorKey* pAIVectorKey, HANDLE hFile, DWORD& dwByte)
{
	if (pAIVectorKey == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIVectorKey->mTime, sizeof(pAIVectorKey->mTime));
	WriteVoid(&pAIVectorKey->mValue, sizeof(pAIVectorKey->mValue));
}

void VECTOR_KEY::Deserialization(VECTOR_KEY* tVectorKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tVectorKey->m_Time, sizeof(tVectorKey->m_Time));
	ReadVoid(&tVectorKey->m_Value, sizeof(tVectorKey->m_Value));
}

QUAT_KEY::~QUAT_KEY()
{
}

void QUAT_KEY::Serialization(aiQuatKey* pAIQuatKey, HANDLE hFile, DWORD& dwByte)
{
	if (pAIQuatKey == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIQuatKey->mTime, sizeof(pAIQuatKey->mTime));
	WriteVoid(&pAIQuatKey->mValue, sizeof(pAIQuatKey->mValue));
}

void QUAT_KEY::Deserialization(QUAT_KEY* tQuatKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tQuatKey->m_Time, sizeof(tQuatKey->m_Time));
	ReadVoid(&tQuatKey->m_Value, sizeof(tQuatKey->m_Value));
}

MESH_ANIM::~MESH_ANIM()
{
	Safe_Delete_Array(m_Keys);
}

void MESH_ANIM::Serialization(aiMeshAnim* pAIMeshAnim, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMeshAnim == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIMeshAnim->mName, hFile, dwByte);
	WriteVoid(&pAIMeshAnim->mNumKeys, sizeof(pAIMeshAnim->mNumKeys));

	for(size_t i = 0; i < pAIMeshAnim->mNumKeys; ++i)
		MESH_KEY::Serialization(&pAIMeshAnim->mKeys[i], hFile, dwByte);
}

void MESH_ANIM::Deserialization(MESH_ANIM* tMeshAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tMeshAnim->m_Name, hFile, dwByte);
	ReadVoid(&tMeshAnim->m_NumKeys, sizeof(tMeshAnim->m_NumKeys));
	
	tMeshAnim->m_Keys = new MESH_KEY[tMeshAnim->m_NumKeys];
	for (size_t i = 0; i < tMeshAnim->m_NumKeys; ++i)
		MESH_KEY::Deserialization(&tMeshAnim->m_Keys[i], hFile, dwByte);
}

MESH_KEY::~MESH_KEY()
{
}

void MESH_KEY::Serialization(aiMeshKey* pAIMeshKey, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMeshKey == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIMeshKey->mTime, sizeof(pAIMeshKey->mTime));
	WriteVoid(&pAIMeshKey->mValue, sizeof(pAIMeshKey->mValue));
}

void MESH_KEY::Deserialization(MESH_KEY* tMeshKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMeshKey->m_Time, sizeof(tMeshKey->m_Time));
	ReadVoid(&tMeshKey->m_Value, sizeof(tMeshKey->m_Value));
}

MESH_MORPH_ANIM::~MESH_MORPH_ANIM()
{
	Safe_Delete_Array(m_Keys);
}

void MESH_MORPH_ANIM::Serialization(aiMeshMorphAnim* pAIMeshMorphAnim, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMeshMorphAnim == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIMeshMorphAnim->mName, hFile, dwByte);
	WriteVoid(&pAIMeshMorphAnim->mNumKeys, sizeof(pAIMeshMorphAnim->mNumKeys));
	for (size_t i = 0; i < pAIMeshMorphAnim->mNumKeys; ++i)
		MESH_MORPH_KEY::Serialization(&pAIMeshMorphAnim->mKeys[i], hFile, dwByte);
}

void MESH_MORPH_ANIM::Deserialization(MESH_MORPH_ANIM* tMeshMorphAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tMeshMorphAnim->m_Name, hFile, dwByte);
	ReadVoid(&tMeshMorphAnim->m_NumKeys, sizeof(tMeshMorphAnim->m_NumKeys));
	
	tMeshMorphAnim->m_Keys = new MESH_MORPH_KEY[tMeshMorphAnim->m_NumKeys];
	for (size_t i = 0; i < tMeshMorphAnim->m_NumKeys; ++i)
		MESH_MORPH_KEY::Deserialization(&tMeshMorphAnim->m_Keys[i], hFile, dwByte);
}

BONE::~BONE()
{
	Safe_Delete_Array(m_Weights);
}

void BONE::Serialization(aiBone* pAIBone, HANDLE hFile, DWORD& dwByte)
{
	if (pAIBone == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIBone->mName, hFile, dwByte);
	WriteVoid(&pAIBone->mNumWeights, sizeof(pAIBone->mNumWeights));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Serialization(pAIBone->mArmature, hFile, dwByte);
	NODE::Serialization(pAIBone->mNode, hFile, dwByte);
#endif
	for (size_t i = 0; i < pAIBone->mNumWeights; ++i)
		VERTEX_WEIGHT::Serialization(&pAIBone->mWeights[i], hFile, dwByte);
	WriteVoid(&pAIBone->mOffsetMatrix, sizeof(pAIBone->mOffsetMatrix));
}

void BONE::Deserialization(BONE* tBone, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tBone->m_Name, hFile, dwByte);
	ReadVoid(&tBone->m_NumWeights, sizeof(tBone->m_NumWeights));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Deserialization(tBone->m_Armature, hFile, dwByte);
	NODE::Deserialization(tBone->m_Node, hFile, dwByte);
#endif
	
	tBone->m_Weights = new VERTEX_WEIGHT[tBone->m_NumWeights];
	for (size_t i = 0; i < tBone->m_NumWeights; ++i)
		VERTEX_WEIGHT::Deserialization(&tBone->m_Weights[i], hFile, dwByte);
	ReadVoid(&tBone->m_OffsetMatrix, sizeof(tBone->m_OffsetMatrix));
}

VERTEX_WEIGHT::~VERTEX_WEIGHT()
{
}

void VERTEX_WEIGHT::Serialization(aiVertexWeight* pAIVertexWeight, HANDLE hFile, DWORD& dwByte)
{
	if (pAIVertexWeight == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIVertexWeight->mVertexId, sizeof(pAIVertexWeight->mVertexId));
	WriteVoid(&pAIVertexWeight->mWeight, sizeof(pAIVertexWeight->mWeight));

}

void VERTEX_WEIGHT::Deserialization(VERTEX_WEIGHT* tVertexWeight, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tVertexWeight->m_VertexId, sizeof(tVertexWeight->m_VertexId));
	ReadVoid(&tVertexWeight->m_Weight, sizeof(tVertexWeight->m_Weight));
}

ANIM_MESH::~ANIM_MESH()
{
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		Safe_Delete_Array(m_TextureCoords[i]);
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
		Safe_Delete_Array(m_Colors[i]);
	Safe_Delete_Array(m_Bitangents);
	Safe_Delete_Array(m_Tangents);
	Safe_Delete_Array(m_Normals);
	Safe_Delete_Array(m_Vertices);
}

void ANIM_MESH::Serialization(aiAnimMesh* pAIAnimMesh, HANDLE hFile, DWORD& dwByte)
{
	if (pAIAnimMesh == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAIAnimMesh->mName, hFile, dwByte);
	WriteVoid(&pAIAnimMesh->mNumVertices, sizeof(pAIAnimMesh->mNumVertices));

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteVoid(&pAIAnimMesh->mVertices[i], sizeof(pAIAnimMesh->mVertices[i]));

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteVoid(&pAIAnimMesh->mNormals[i], sizeof(pAIAnimMesh->mNormals[i]));

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteVoid(&pAIAnimMesh->mTangents[i], sizeof(pAIAnimMesh->mTangents[i]));

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteVoid(&pAIAnimMesh->mBitangents[i], sizeof(pAIAnimMesh->mBitangents[i]));

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIAnimMesh->mColors[i], sizeof(pAIAnimMesh->mColors[i]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIAnimMesh->mTextureCoords[i], sizeof(pAIAnimMesh->mTextureCoords[i]));
		}
	}

	WriteVoid(&pAIAnimMesh->mWeight, sizeof(pAIAnimMesh->mWeight));
}

void ANIM_MESH::Deserialization(ANIM_MESH* tAnimMesh, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tAnimMesh->m_Name, hFile, dwByte);
	ReadVoid(&tAnimMesh->m_NumVertices, sizeof(tAnimMesh->m_NumVertices));

	tAnimMesh->m_Vertices = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadVoid(&tAnimMesh->m_Vertices[i], sizeof(tAnimMesh->m_Vertices[i]));

	tAnimMesh->m_Normals = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadVoid(&tAnimMesh->m_Normals[i], sizeof(tAnimMesh->m_Normals[i]));

	tAnimMesh->m_Tangents = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadVoid(&tAnimMesh->m_Tangents[i], sizeof(tAnimMesh->m_Tangents[i]));

	tAnimMesh->m_Bitangents = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadVoid(&tAnimMesh->m_Bitangents[i], sizeof(tAnimMesh->m_Bitangents[i]));
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		tAnimMesh->m_Colors[i] = new XMFLOAT4[tAnimMesh->m_NumVertices];
		for (size_t j = 0; j < tAnimMesh->m_NumVertices; ++j)
		{
			ReadVoid(&tAnimMesh->m_Colors[i][j], sizeof(tAnimMesh->m_Colors[i][j]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		tAnimMesh->m_TextureCoords[i] = new XMFLOAT3[tAnimMesh->m_NumVertices];
		for (size_t j = 0; j < tAnimMesh->m_NumVertices; ++j)
		{
			ReadVoid(&tAnimMesh->m_TextureCoords[i][j], sizeof(tAnimMesh->m_TextureCoords[i][j]));
		}
	}

	ReadVoid(&tAnimMesh->m_Weight, sizeof(tAnimMesh->m_Weight));
}

TEXTURE::~TEXTURE()
{
	Safe_Delete_Array(m_pcData);
}

void TEXTURE::Serialization(aiTexture* pAITexture, HANDLE hFile, DWORD& dwByte)
{
	if (pAITexture == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAITexture->mWidth, sizeof(pAITexture->mWidth));
	WriteVoid(&pAITexture->mHeight, sizeof(pAITexture->mHeight));
	WriteVoid(&pAITexture->achFormatHint, sizeof(char) * HINTMAXTEXTURELEN);
	for(size_t i = 0; i < pAITexture->mWidth * pAITexture->mHeight; ++i)
		TEXEL::Serialization(&pAITexture->pcData[i], hFile, dwByte);
	AI_STRING::Serialization(&pAITexture->mFilename, hFile, dwByte);
}

void TEXTURE::Deserialization(TEXTURE* tTexture, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tTexture->m_Width, sizeof(tTexture->m_Width));
	ReadVoid(&tTexture->m_Height, sizeof(tTexture->m_Height));
	ReadVoid(&tTexture->m_achFormatHint, sizeof(char) * HINTMAXTEXTURELEN);
	tTexture->m_pcData = new TEXEL[tTexture->m_Width * tTexture->m_Height];
	for (size_t i = 0; i < tTexture->m_Width * tTexture->m_Height; ++i)
		TEXEL::Deserialization(&tTexture->m_pcData[i], hFile, dwByte);
	AI_STRING::Deserialization(&tTexture->m_Filename, hFile, dwByte);
}

TEXEL::~TEXEL()
{
}

void TEXEL::Serialization(aiTexel* pAITexel, HANDLE hFile, DWORD& dwByte)
{
	if (pAITexel == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);
	
	WriteVoid(&pAITexel->b, sizeof(char) * 4);
}

void TEXEL::Deserialization(TEXEL* tTexel, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tTexel->b, sizeof(char) * 4);
}

LIGHT::~LIGHT()
{
}

void LIGHT::Serialization(aiLight* pAILight, HANDLE hFile, DWORD& dwByte)
{
	if (pAILight == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(pAILight, sizeof(aiLight));
}

void LIGHT::Deserialization(LIGHT* tLight, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(tLight, sizeof(LIGHT));
}

CAMERA::~CAMERA()
{
}

void CAMERA::Serialization(aiCamera* pAICamera, HANDLE hFile, DWORD& dwByte)
{
	if (pAICamera == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(pAICamera, sizeof(aiCamera));
}

void CAMERA::Deserialization(CAMERA* tCamera, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(tCamera, sizeof(CAMERA));
}

SKELETON::~SKELETON()
{
	for (size_t i = 0; i < m_NumBones; i++)
		Safe_Delete(m_Bones[i]);
	Safe_Delete_Array(m_Bones);
}

void SKELETON::Serialization(aiSkeleton* pAISkeleton, HANDLE hFile, DWORD& dwByte)
{
	if (pAISkeleton == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	AI_STRING::Serialization(&pAISkeleton->mName, hFile, dwByte);
	WriteVoid(&pAISkeleton->mNumBones, sizeof(pAISkeleton->mNumBones));

	for (unsigned int i = 0; i < pAISkeleton->mNumBones; i++)
		SKELETON_BONE::Serialization(pAISkeleton->mBones[i], hFile, dwByte);
}

void SKELETON::Deserialization(SKELETON* tSkeleton, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	AI_STRING::Deserialization(&tSkeleton->m_Name, hFile, dwByte);
	ReadVoid(&tSkeleton->m_NumBones, sizeof(tSkeleton->m_NumBones));

	tSkeleton->m_Bones = new SKELETON_BONE * [tSkeleton->m_NumBones];
	for (size_t i = 0; i < tSkeleton->m_NumBones; i++) {
		tSkeleton->m_Bones[i] = new SKELETON_BONE;
		SKELETON_BONE::Deserialization(tSkeleton->m_Bones[i], hFile, dwByte);
	}
}

SKELETON_BONE::~SKELETON_BONE()
{
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	Safe_Delete(m_Armature);
	Safe_Delete(m_Node);
#endif
	Safe_Delete(m_MeshId);
	Safe_Delete_Array(m_Weights);
}

void SKELETON_BONE::Serialization(aiSkeletonBone* pAISkeletonBone, HANDLE hFile, DWORD& dwByte)
{
	if (pAISkeletonBone == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAISkeletonBone->mParent, sizeof(pAISkeletonBone->mParent));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Serialization(pAISkeletonBone->mArmature, hFile, dwByte);
	NODE::Serialization(pAISkeletonBone->mNode, hFile, dwByte);
#endif
	WriteVoid(&pAISkeletonBone->mNumnWeights, sizeof(pAISkeletonBone->mNumnWeights));
	MESH::Serialization(pAISkeletonBone->mMeshId, hFile, dwByte);
	for (size_t i = 0; i < pAISkeletonBone->mNumnWeights; ++i)
		VERTEX_WEIGHT::Serialization(&pAISkeletonBone->mWeights[i], hFile, dwByte);
	WriteVoid(&pAISkeletonBone->mOffsetMatrix, sizeof(pAISkeletonBone->mOffsetMatrix));
	WriteVoid(&pAISkeletonBone->mLocalMatrix, sizeof(pAISkeletonBone->mLocalMatrix));
}

void SKELETON_BONE::Deserialization(SKELETON_BONE* tSkeletonBone, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tSkeletonBone->m_Parent, sizeof(tSkeletonBone->m_Parent));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	tSkeletonBone->m_Armature = new NODE;
	NODE::Deserialization(tSkeletonBone->m_Armature, hFile, dwByte);
	tSkeletonBone->m_Node = new NODE;
	NODE::Deserialization(tSkeletonBone->m_Node, hFile, dwByte);
#endif
	ReadVoid(&tSkeletonBone->m_NumnWeights, sizeof(tSkeletonBone->m_NumnWeights));
	
	tSkeletonBone->m_MeshId = new MESH;
	MESH::Deserialization(tSkeletonBone->m_MeshId, hFile, dwByte);
	
	tSkeletonBone->m_Weights = new VERTEX_WEIGHT[tSkeletonBone->m_NumnWeights];
	for (size_t i = 0; i < tSkeletonBone->m_NumnWeights; ++i)
		VERTEX_WEIGHT::Deserialization(&tSkeletonBone->m_Weights[i], hFile, dwByte);

	ReadVoid(&tSkeletonBone->m_OffsetMatrix, sizeof(tSkeletonBone->m_OffsetMatrix));
	ReadVoid(&tSkeletonBone->m_LocalMatrix, sizeof(tSkeletonBone->m_LocalMatrix));
}

SCENE::~SCENE()
{
	// Skeletons 메모리 해제
	for (size_t i = 0; i < m_NumSkeletons; ++i)
	{
		Safe_Delete_Array(m_Skeletons[i]);
	}
	Safe_Delete_Array(m_Skeletons);

	// m_MetaData 메모리 해제
	Safe_Delete(m_MetaData);

	// Cameras 메모리 해제
	for (size_t i = 0; i < m_NumCameras; ++i)
	{
		Safe_Delete_Array(m_Cameras[i]);
	}
	Safe_Delete_Array(m_Cameras);

	// Lights 메모리 해제
	for (size_t i = 0; i < m_NumLights; ++i)
	{
		Safe_Delete_Array(m_Lights[i]);
	}
	Safe_Delete_Array(m_Lights);

	// Textures 메모리 해제
	for (size_t i = 0; i < m_NumTextures; ++i)
	{
		Safe_Delete_Array(m_Textures[i]);
	}
	Safe_Delete_Array(m_Textures);

	// Animations 메모리 해제
	for (size_t i = 0; i < m_NumAnimations; ++i)
	{
		Safe_Delete_Array(m_Animations[i]);
	}
	Safe_Delete_Array(m_Animations);

	// Materials 메모리 해제
	for (size_t i = 0; i < m_NumMaterials; ++i)
	{
		Safe_Delete_Array(m_Materials[i]);
	}
	Safe_Delete_Array(m_Materials);

	// Meshes 메모리 해제
	for (size_t i = 0; i < m_NumMeshes; ++i)
	{
		Safe_Delete_Array(m_Meshes[i]);
	}
	Safe_Delete_Array(m_Meshes);

	// RootNode 메모리 해제
	Safe_Delete(m_RootNode);

}

void SCENE::Serialization(const aiScene* pAIScene, HANDLE hFile, DWORD& dwByte)
{
	if (pAIScene == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIScene->mFlags, sizeof(pAIScene->mFlags));
	NODE::Serialization(pAIScene->mRootNode, hFile, dwByte);

	WriteVoid(&pAIScene->mNumMeshes, sizeof(pAIScene->mNumMeshes));
	for (size_t i = 0; i < pAIScene->mNumMeshes; ++i)
		MESH::Serialization(pAIScene->mMeshes[i], hFile, dwByte);

	WriteVoid(&pAIScene->mNumMaterials, sizeof(pAIScene->mNumMaterials));
	for (size_t i = 0; i < pAIScene->mNumMaterials; ++i)
		MATERIAL::Serialization(pAIScene->mMaterials[i], hFile, dwByte);

	WriteVoid(&pAIScene->mNumAnimations, sizeof(pAIScene->mNumAnimations));
	for (size_t i = 0; i < pAIScene->mNumMaterials; ++i)
		ANIMATION::Serialization(pAIScene->mAnimations[i], hFile, dwByte);

	WriteVoid(&pAIScene->mNumTextures, sizeof(pAIScene->mNumTextures));
	for (size_t i = 0; i < pAIScene->mNumTextures; ++i)
		TEXTURE::Serialization(pAIScene->mTextures[i], hFile, dwByte);

	WriteVoid(&pAIScene->mNumLights, sizeof(pAIScene->mNumLights));
	for (size_t i = 0; i < pAIScene->mNumLights; ++i)
		LIGHT::Serialization(pAIScene->mLights[i], hFile, dwByte);

	WriteVoid(&pAIScene->mNumCameras, sizeof(pAIScene->mNumCameras));
	for (size_t i = 0; i < pAIScene->mNumCameras; ++i)
		CAMERA::Serialization(pAIScene->mCameras[i], hFile, dwByte);

	META_DATA::Serialization(pAIScene->mMetaData, hFile, dwByte);

	AI_STRING::Serialization(&pAIScene->mName, hFile, dwByte);

	WriteVoid(&pAIScene->mSkeletons, sizeof(pAIScene->mSkeletons));
	for (size_t i = 0; i < pAIScene->mNumCameras; ++i)
		SKELETON::Serialization(pAIScene->mSkeletons[i], hFile, dwByte);
}

void SCENE::Deserialization(SCENE* tScene, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;
	
	ReadVoid(&tScene->m_Flags, sizeof(tScene->m_Flags));
	tScene->m_RootNode = new NODE;
	NODE::Deserialization(tScene->m_RootNode, hFile, dwByte);

	ReadVoid(&tScene->m_NumMeshes, sizeof(tScene->m_NumMeshes));
	
	for (size_t i = 0; i < tScene->m_NumMeshes; ++i)
	{
		tScene->m_Meshes[i] = new MESH[tScene->m_NumMeshes];
		MESH::Deserialization(tScene->m_Meshes[i], hFile, dwByte);
	}

	ReadVoid(&tScene->m_NumMaterials, sizeof(tScene->m_NumMaterials));
	for (size_t i = 0; i < tScene->m_NumMaterials; ++i)
	{
		tScene->m_Materials[i] = new MATERIAL[tScene->m_NumMaterials];
		MATERIAL::Deserialization(tScene->m_Materials[i], hFile, dwByte);
	}

	ReadVoid(&tScene->m_NumAnimations, sizeof(tScene->m_NumAnimations));
	for (size_t i = 0; i < tScene->m_NumAnimations; ++i)
	{
		tScene->m_Animations[i] = new ANIMATION[tScene->m_NumAnimations];
		ANIMATION::Deserialization(tScene->m_Animations[i], hFile, dwByte);
	}

	ReadVoid(&tScene->m_NumTextures, sizeof(tScene->m_NumTextures));
	for (size_t i = 0; i < tScene->m_NumTextures; ++i)
	{
		tScene->m_Textures[i] = new TEXTURE[tScene->m_NumTextures];
		TEXTURE::Deserialization(tScene->m_Textures[i], hFile, dwByte);
	}

	ReadVoid(&tScene->m_NumLights, sizeof(tScene->m_NumLights));
	for (size_t i = 0; i < tScene->m_NumLights; ++i)
	{
		tScene->m_Lights[i] = new LIGHT[tScene->m_NumLights];
		LIGHT::Deserialization(tScene->m_Lights[i], hFile, dwByte);
	}

	ReadVoid(&tScene->m_NumCameras, sizeof(tScene->m_NumCameras));
	for (size_t i = 0; i < tScene->m_NumCameras; ++i)
	{
		tScene->m_Cameras[i] = new CAMERA[tScene->m_NumCameras];
		CAMERA::Deserialization(tScene->m_Cameras[i], hFile, dwByte);
	}

	tScene->m_MetaData = new META_DATA;
	META_DATA::Deserialization(tScene->m_MetaData, hFile, dwByte);

	AI_STRING::Deserialization(&tScene->m_Name, hFile, dwByte);

	ReadVoid(&tScene->m_NumSkeletons, sizeof(tScene->m_NumSkeletons));
	for (size_t i = 0; i < tScene->m_NumSkeletons; ++i)
	{
		tScene->m_Skeletons[i] = new SKELETON[tScene->m_NumSkeletons];
		SKELETON::Deserialization(tScene->m_Skeletons[i], hFile, dwByte);
	}
}

AI_STRING::~AI_STRING()
{
}

void AI_STRING::Serialization(const aiString* pAIString, HANDLE hFile, DWORD& dwByte)
{
	if (pAIString == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(pAIString->data, sizeof(pAIString->data));
}

void AI_STRING::Deserialization(AI_STRING* tAIString, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(tAIString->m_data, sizeof(tAIString->m_data));
}

MESH_MORPH_KEY::~MESH_MORPH_KEY()
{
	Safe_Delete_Array(m_Weights);
	Safe_Delete_Array(m_Values);
}

void MESH_MORPH_KEY::Serialization(aiMeshMorphKey* pAIMeshMorphKey, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMeshMorphKey == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIMeshMorphKey->mTime, sizeof(pAIMeshMorphKey->mTime));
	WriteVoid(&pAIMeshMorphKey->mNumValuesAndWeights, sizeof(pAIMeshMorphKey->mNumValuesAndWeights));
	for (size_t i = 0; i < pAIMeshMorphKey->mNumValuesAndWeights; ++i)
		WriteVoid(&pAIMeshMorphKey->mValues[i], sizeof(pAIMeshMorphKey->mValues[i]));
	for (size_t i = 0; i < pAIMeshMorphKey->mNumValuesAndWeights; ++i)
		WriteVoid(&pAIMeshMorphKey->mWeights[i], sizeof(pAIMeshMorphKey->mWeights[i]));

}

void MESH_MORPH_KEY::Deserialization(MESH_MORPH_KEY* tMeshMorphKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadVoid(&tMeshMorphKey->m_Time, sizeof(tMeshMorphKey->m_Time));
	ReadVoid(&tMeshMorphKey->m_NumValuesAndWeights, sizeof(tMeshMorphKey->m_NumValuesAndWeights));

	tMeshMorphKey->m_Values = new unsigned int[tMeshMorphKey->m_NumValuesAndWeights];
	for (size_t i = 0; i < tMeshMorphKey->m_NumValuesAndWeights; ++i)
		ReadVoid(&tMeshMorphKey->m_Values[i], sizeof(tMeshMorphKey->m_Values[i]));

	tMeshMorphKey->m_Weights = new double[tMeshMorphKey->m_NumValuesAndWeights];
	for (size_t i = 0; i < tMeshMorphKey->m_NumValuesAndWeights; ++i)
		ReadVoid(&tMeshMorphKey->m_Weights[i], sizeof(tMeshMorphKey->m_Weights[i]));
}
