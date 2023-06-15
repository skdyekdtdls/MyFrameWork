#include "Assimp_Struct.h"
#include "Engine_Defines.h"

META_DATA_ENTRY::~META_DATA_ENTRY()
{
	if (AI_AISTRING == m_eType)
		Safe_Delete_Array(m_Data);
	else
		Safe_Delete(m_Data);
}

void META_DATA_ENTRY::Serialization(aiMetadataEntry* pAIMetadataEntry, HANDLE hFile, DWORD& dwByte)
{
	if (nullptr == pAIMetadataEntry || pAIMetadataEntry->mData)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);	

	WriteUINT(pAIMetadataEntry->mType);

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
		iSize = sizeof(char) * MAX_PATH;
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
		iSize = sizeof(META_DATA);
		break;
	}

	WriteFile(hFile, pAIMetadataEntry->mData, iSize, &dwByte, nullptr);
}

void META_DATA_ENTRY::Deserialization(META_DATA_ENTRY* tMetaDataEntry, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(*((_uint*)&tMetaDataEntry->m_eType));

	unsigned int iSize = 0;

	void* pData = new int;
	switch (tMetaDataEntry->m_eType)
	{
	case AI_BOOL:
		iSize = sizeof(_bool);
		pData = new bool;
		break;
	case AI_INT32:
		iSize = sizeof(int);
		pData = new int;
		break;
	case AI_UINT64:
		iSize = sizeof(unsigned long long);
		pData = new unsigned long long;
		break;
	case AI_FLOAT:
		iSize = sizeof(_float);
		pData = new _float;
		break;
	case AI_DOUBLE:
		iSize = sizeof(_double);
		pData = new _double;
		break;
	case AI_AISTRING:
		iSize = sizeof(char) * MAX_PATH;
		pData = new char[MAX_PATH];
		break;
	case AI_AIVECTOR3D:
		iSize = sizeof(_float3);
		pData = new _float3;
		break;
	case AI_AIMETADATA:
		iSize = sizeof(META_DATA);
		pData = new META_DATA;
		break;
	case AI_INT64:
		iSize = sizeof(long long);
		pData = new long long;
		break;
	case AI_UINT32:
		iSize = sizeof(unsigned int);
		pData = new unsigned int;
		break;
	case AI_META_MAX:
		iSize = sizeof(META_DATA);
		pData = new META_DATA;
		break;
	}

	ReadFile(hFile, pData, iSize, &dwByte, nullptr);
	tMetaDataEntry->m_Data = pData;
}

META_DATA::~META_DATA()
{
	Safe_Delete_Array(m_szKeys);
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

	WriteUINT(pAIMetadata->mNumProperties);

	for (size_t i = 0; i < pAIMetadata->mNumProperties; ++i)
		WriteCHAR(pAIMetadata->mKeys[i].data);

	for (size_t i = 0; i < pAIMetadata->mNumProperties; ++i)
		META_DATA_ENTRY::Serialization(&pAIMetadata->mValues[i], hFile, dwByte);
}

void META_DATA::Deserialization(META_DATA* tMetaData, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tMetaData->m_NumProperties);

	tMetaData->m_szKeys = new char* [tMetaData->m_NumProperties];
	for (size_t i = 0; i < tMetaData->m_NumProperties; ++i)
	{
		tMetaData->m_szKeys[i] = new char[MAX_PATH];
		ReadCHAR(tMetaData->m_szKeys[i]);
	}
	
	tMetaData->m_Values = new META_DATA_ENTRY[tMetaData->m_NumProperties];
	for (size_t i = 0; i < tMetaData->m_NumProperties; ++i)
	{
		META_DATA_ENTRY::Deserialization(&tMetaData->m_Values[i], hFile, dwByte);
	}
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
	Safe_Delete(m_Parent);
}

void NODE::Serialization(aiNode* pAINode, HANDLE hFile, DWORD& dwByte)
{
	if (pAINode == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteCHAR(pAINode->mName.data);
	WriteFloat4x4(*((_float4x4*)&pAINode->mTransformation.Transpose()));
	NODE::Serialization(pAINode->mParent, hFile, dwByte);
	WriteUINT(pAINode->mNumChildren);
	for (size_t i = 0; i < pAINode->mNumChildren; ++i)
		NODE::Serialization(pAINode->mChildren[i], hFile, dwByte);
	WriteUINT(pAINode->mNumMeshes);
	for (size_t i = 0; i < pAINode->mNumMeshes; ++i)
		WriteUINT(pAINode->mMeshes[i]);
	META_DATA::Serialization(pAINode->mMetaData, hFile, dwByte);
}

void NODE::Deserialization(NODE* tNode, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tNode->m_Name);
	ReadFloat4x4(tNode->m_Transformation);

	tNode->m_Parent = new NODE;
	NODE::Deserialization(tNode->m_Parent, hFile, dwByte);

	ReadUINT(tNode->m_NumChildren);
	tNode->m_Children = new NODE*[tNode->m_NumChildren];
	for (size_t i = 0; i < tNode->m_NumChildren; ++i)
	{
		tNode->m_Children[i] = new NODE;
		NODE::Deserialization(tNode->m_Children[i], hFile, dwByte);
	}

	ReadUINT(tNode->m_NumMeshes);
	tNode->m_Meshes = new unsigned int[tNode->m_NumMeshes];
	for (size_t i = 0; i < tNode->m_NumMeshes; ++i)
	{
		ReadUINT(tNode->m_Meshes[i]);
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

	WriteUINT(pAIFace->mNumIndices);
	for(size_t i = 0; i < pAIFace->mNumIndices; ++i)
		WriteUINT(pAIFace->mIndices[i]);
}

void FACE::Deserialization(FACE* tFace, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tFace->m_NumIndices);
	tFace->m_Indices = new unsigned int[tFace->m_NumIndices];
	for (size_t i = 0; i < tFace->m_NumIndices; ++i)
		ReadUINT(tFace->m_Indices[i]);
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

	WriteFloat3(*((_float3*)&pAIAABB->mMin));
	WriteFloat3(*((_float3*)&pAIAABB->mMax));
}

void AABB::Deserialization(AABB* tAABB, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadFloat3(tAABB->m_Min);
	ReadFloat3(tAABB->m_Max);
}

MESH::~MESH()
{
}

void MESH::Serialization(aiMesh* pAIMesh, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMesh == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteUINT(pAIMesh->mPrimitiveTypes);
	WriteUINT(pAIMesh->mNumVertices);
	WriteUINT(pAIMesh->mNumFaces);
	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIMesh->mVertices[i]);

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIMesh->mNormals[i]);

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIMesh->mTangents[i]);

	for (size_t i = 0; i < pAIMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIMesh->mBitangents[i]);


	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			WriteFloat4(*(_float4*)&pAIMesh->mColors[i][j]);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			WriteFloat3(*(_float3*)&pAIMesh->mTextureCoords[i]);
		}
	}
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		WriteUINT(pAIMesh->mNumUVComponents[i]);
	}
	
	for (size_t i = 0; i < pAIMesh->mNumFaces; ++i)
	{
		FACE::Serialization(&pAIMesh->mFaces[i], hFile, dwByte);
	}

	WriteUINT(pAIMesh->mNumBones);
	for (size_t i = 0; i < pAIMesh->mNumBones; ++i)
	{
		BONE::Serialization(pAIMesh->mBones[i], hFile, dwByte);
	}

	WriteUINT(pAIMesh->mMaterialIndex);
	WriteCHAR(pAIMesh->mName.data);

	WriteUINT(pAIMesh->mNumAnimMeshes);
	for (size_t i = 0; i < pAIMesh->mNumAnimMeshes; ++i)
	{
		ANIM_MESH::Serialization(pAIMesh->mAnimMeshes[i], hFile, dwByte);
	}

	WriteUINT(pAIMesh->mMethod);
	AABB::Serialization(&pAIMesh->mAABB, hFile, dwByte);
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		WriteCHAR(pAIMesh->mTextureCoordsNames[i]->data);
}

void MESH::Deserialization(MESH* tMesh, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tMesh->m_PrimitiveTypes);
	ReadUINT(tMesh->m_NumVertices);
	ReadUINT(tMesh->m_NumFaces);

	tMesh->m_Vertices = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadFloat3(tMesh->m_Vertices[i]);
	}

	tMesh->m_Normals = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadFloat3(tMesh->m_Normals[i]);
	}

	tMesh->m_Tangents = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadFloat3(tMesh->m_Tangents[i]);
	}

	tMesh->m_Bitangents = new _float3[tMesh->m_NumVertices];
	for (size_t i = 0; i < tMesh->m_NumVertices; ++i)
	{
		ReadFloat3(tMesh->m_Bitangents[i]);
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		tMesh->m_Colors[i] = new _float4[tMesh->m_NumVertices];
		for (size_t j = 0; j < tMesh->m_NumVertices; ++j)
		{
			ReadFloat4(tMesh->m_Colors[i][j]);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		tMesh->m_TextureCoords[i] = new _float3[tMesh->m_NumVertices];
		for (size_t j = 0; j < tMesh->m_NumVertices; ++j)
		{
			ReadFloat3(tMesh->m_TextureCoords[i][j]);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		ReadUINT(tMesh->m_NumUVComponents[i]);
	}

	tMesh->m_Faces = new FACE[tMesh->m_NumFaces];
	for (size_t i = 0; i < tMesh->m_NumFaces; ++i)
	{
		FACE::Deserialization(tMesh->m_Faces, hFile, dwByte);
	}

	ReadUINT(tMesh->m_NumBones);
	tMesh->m_Bones = new BONE*[tMesh->m_NumBones];
	for (size_t i = 0; i < tMesh->m_NumBones; ++i)
	{
		tMesh->m_Bones[i] = new BONE;
		BONE::Deserialization(tMesh->m_Bones[i], hFile, dwByte);
	}

	ReadUINT(tMesh->m_MaterialIndex);
	ReadCHAR(tMesh->m_szName);
	ReadUINT(tMesh->m_NumAnimMeshes);

	tMesh->m_AnimMeshes = new ANIM_MESH*[tMesh->m_NumAnimMeshes];
	for (size_t i = 0; i < tMesh->m_NumAnimMeshes; ++i)
	{
		tMesh->m_AnimMeshes[i] = new ANIM_MESH;
		ANIM_MESH::Deserialization(tMesh->m_AnimMeshes[i], hFile, dwByte);
	}

	ReadUINT((unsigned int&)tMesh->m_eMethod);
	AABB::Deserialization(&tMesh->m_AABB, hFile, dwByte);

	tMesh->m_TextureCoordsNames = new char* [AI_MAX_NUMBER_OF_TEXTURECOORDS];
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		tMesh->m_TextureCoordsNames[i] = new char[MAX_PATH];
		WriteCHAR(tMesh->m_TextureCoordsNames[i]);
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

	WriteUINT(pAIMaterial->mNumProperties);
	WriteUINT(pAIMaterial->mNumAllocated);

	for(size_t i = 0; i < pAIMaterial->mNumProperties; ++i)
		MATERIAL_PROPERTY::Serialization(pAIMaterial->mProperties[i], hFile, dwByte);
}

void MATERIAL::Deserialization(MATERIAL* tMaterial, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tMaterial->m_NumProperties);
	ReadUINT(tMaterial->m_NumAllocated);

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

	WriteCHAR(pAIMaterialProperty->mKey.data);
	WriteUINT(pAIMaterialProperty->mSemantic);
	WriteUINT(pAIMaterialProperty->mIndex);
	WriteUINT(pAIMaterialProperty->mDataLength);
	WriteUINT(pAIMaterialProperty->mType);
	WriteFile(hFile, pAIMaterialProperty->mData, pAIMaterialProperty->mDataLength, &dwByte, nullptr);
}

void MATERIAL_PROPERTY::Deserialization(MATERIAL_PROPERTY* tMaterialProperty, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tMaterialProperty->m_szKey);
	ReadUINT(tMaterialProperty->m_Semantic);
	ReadUINT(tMaterialProperty->m_Index);
	ReadUINT(tMaterialProperty->m_DataLength);
	ReadUINT((unsigned int&)tMaterialProperty->m_eType);
	ReadFile(hFile, tMaterialProperty->m_Data, tMaterialProperty->m_DataLength, &dwByte, nullptr);
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

	WriteCHAR(pAIAnimation->mName.data);
	WriteDouble(pAIAnimation->mDuration);
	WriteDouble(pAIAnimation->mTicksPerSecond);
	WriteUINT(pAIAnimation->mNumChannels);
	for(size_t i = 0; i < pAIAnimation->mNumChannels; ++i)
		NODE_ANIM::Serialization(pAIAnimation->mChannels[i], hFile, dwByte);
	WriteUINT(pAIAnimation->mNumMeshChannels);
	for (size_t i = 0; i < pAIAnimation->mNumMeshChannels; ++i)
		MESH_ANIM::Serialization(pAIAnimation->mMeshChannels[i], hFile, dwByte);
	WriteUINT(pAIAnimation->mNumMorphMeshChannels);
	for (size_t i = 0; i < pAIAnimation->mNumMorphMeshChannels; ++i)
		MESH_MORPH_ANIM::Serialization(pAIAnimation->mMorphMeshChannels[i], hFile, dwByte);
}

void ANIMATION::Deserialization(ANIMATION* tAnimation, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	WriteCHAR(tAnimation->m_szName);
	WriteDouble(tAnimation->m_Duration);
	WriteDouble(tAnimation->m_TicksPerSecond);
	WriteUINT(tAnimation->m_NumChannels);

	tAnimation->m_Channels = new NODE_ANIM * [tAnimation->m_NumChannels];
	for (size_t i = 0; i < tAnimation->m_NumChannels; ++i)
	{
		tAnimation->m_Channels[i] = new NODE_ANIM[tAnimation->m_NumChannels];
		NODE_ANIM::Deserialization(tAnimation->m_Channels[i], hFile, dwByte);
	}

	WriteUINT(tAnimation->m_NumMeshChannels);
	tAnimation->m_MeshChannels = new MESH_ANIM * [tAnimation->m_NumMeshChannels];
	for (size_t i = 0; i < tAnimation->m_NumMeshChannels; ++i)
	{
		tAnimation->m_MeshChannels[i] = new MESH_ANIM;
		MESH_ANIM::Deserialization(tAnimation->m_MeshChannels[i], hFile, dwByte);
	}

	WriteUINT(tAnimation->m_NumMorphMeshChannels);
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

	WriteCHAR(pAINodeAnim->mNodeName.data);
	WriteUINT(pAINodeAnim->mNumPositionKeys);
	VECTOR_KEY::Serialization(pAINodeAnim->mPositionKeys, hFile, dwByte);
	WriteUINT(pAINodeAnim->mNumRotationKeys);
	QUAT_KEY::Serialization(pAINodeAnim->mRotationKeys, hFile, dwByte);
	WriteUINT(pAINodeAnim->mNumScalingKeys);
	VECTOR_KEY::Serialization(pAINodeAnim->mScalingKeys, hFile, dwByte);
	WriteUINT(pAINodeAnim->mPreState);
	WriteUINT(pAINodeAnim->mPostState);
}

void NODE_ANIM::Deserialization(NODE_ANIM* tNodeAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tNodeAnim->m_szNodeName);

	ReadUINT(tNodeAnim->m_NumPositionKeys);
	tNodeAnim->m_PositionKeys = new VECTOR_KEY[tNodeAnim->m_NumPositionKeys];
	for(size_t i = 0; i < tNodeAnim->m_NumPositionKeys; ++i)
		VECTOR_KEY::Deserialization(&tNodeAnim->m_PositionKeys[i], hFile, dwByte);

	ReadUINT(tNodeAnim->m_NumRotationKeys);
	tNodeAnim->m_RotationKeys = new QUAT_KEY[tNodeAnim->m_NumRotationKeys];
	for (size_t i = 0; i < tNodeAnim->m_NumRotationKeys; ++i)
		QUAT_KEY::Deserialization(&tNodeAnim->m_RotationKeys[i], hFile, dwByte);

	ReadUINT(tNodeAnim->m_NumScalingKeys);
	tNodeAnim->m_ScalingKeys = new VECTOR_KEY[tNodeAnim->m_NumScalingKeys];
	for (size_t i = 0; i < tNodeAnim->m_NumScalingKeys; ++i)
		VECTOR_KEY::Deserialization(&tNodeAnim->m_ScalingKeys[i], hFile, dwByte);
	ReadUINT((unsigned int&)tNodeAnim->m_ePreState);
	ReadUINT((unsigned int&)tNodeAnim->m_ePostState);
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

	WriteDouble(pAIVectorKey->mTime);
	WriteFloat3(*(_float3*)&pAIVectorKey->mValue);
}

void VECTOR_KEY::Deserialization(VECTOR_KEY* tVectorKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadDouble(tVectorKey->m_Time);
	ReadFloat3(tVectorKey->m_Value);
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

	WriteDouble(pAIQuatKey->mTime);
	WriteFloat3(*(_float3*)&pAIQuatKey->mValue);
}

void QUAT_KEY::Deserialization(QUAT_KEY* tQuatKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

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

	WriteCHAR(pAIMeshAnim->mName.data);
	WriteUINT(pAIMeshAnim->mNumKeys);

	for(size_t i = 0; i < pAIMeshAnim->mNumKeys; ++i)
		MESH_KEY::Serialization(&pAIMeshAnim->mKeys[i], hFile, dwByte);
}

void MESH_ANIM::Deserialization(MESH_ANIM* tMeshAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tMeshAnim->m_szName);
	ReadUINT(tMeshAnim->m_NumKeys);
	
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

	WriteDouble(pAIMeshKey->mTime);
	WriteUINT(pAIMeshKey->mValue);
}

void MESH_KEY::Deserialization(MESH_KEY* tMeshKey, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadDouble(tMeshKey->m_Time);
	ReadUINT(tMeshKey->m_Value);
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

	WriteCHAR(pAIMeshMorphAnim->mName.data);
	WriteUINT(pAIMeshMorphAnim->mNumKeys);
	for (size_t i = 0; i < pAIMeshMorphAnim->mNumKeys; ++i)
		MESH_MORPH_KEY::Serialization(&pAIMeshMorphAnim->mKeys[i], hFile, dwByte);
}

void MESH_MORPH_ANIM::Deserialization(MESH_MORPH_ANIM* tMeshMorphAnim, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tMeshMorphAnim->m_szName);
	ReadUINT(tMeshMorphAnim->m_NumKeys);
	
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

	WriteCHAR(pAIBone->mName.data);
	WriteUINT(pAIBone->mNumWeights);
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Serialization(pAIBone->mArmature, hFile, dwByte);
	NODE::Serialization(pAIBone->mNode, hFile, dwByte);
#endif
	for (size_t i = 0; i < pAIBone->mNumWeights; ++i)
		VERTEX_WEIGHT::Serialization(&pAIBone->mWeights[i], hFile, dwByte);
	WriteFloat4x4(*(_float4x4*)&pAIBone->mOffsetMatrix);
}

void BONE::Deserialization(BONE* tBone, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadCHAR(tBone->m_szName);
	ReadUINT(tBone->m_NumWeights);
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Deserialization(tBone->m_Armature, hFile, dwByte);
	NODE::Deserialization(tBone->m_Node, hFile, dwByte);
#endif
	
	tBone->m_Weights = new VERTEX_WEIGHT[tBone->m_NumWeights];
	for (size_t i = 0; i < tBone->m_NumWeights; ++i)
		VERTEX_WEIGHT::Deserialization(&tBone->m_Weights[i], hFile, dwByte);
	ReadFloat4x4(*(_float4x4*)&tBone->m_OffsetMatrix);
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

	WriteUINT(pAIVertexWeight->mVertexId);
	WriteFloat(pAIVertexWeight->mWeight);

}

void VERTEX_WEIGHT::Deserialization(VERTEX_WEIGHT* tVertexWeight, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tVertexWeight->m_VertexId);
	ReadFloat(tVertexWeight->m_Weight);
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

	WriteUINT(pAIAnimMesh->mNumVertices, hFile, dwByte);
	WriteCHAR(pAIAnimMesh->mName.data);

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIAnimMesh->mVertices[i]);

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIAnimMesh->mNormals[i]);

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIAnimMesh->mTangents[i]);

	for (size_t i = 0; i < pAIAnimMesh->mNumVertices; ++i)
		WriteFloat3(*(_float3*)&pAIAnimMesh->mBitangents[i]);

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteFloat4(*(_float4*)&pAIAnimMesh->mColors[i]);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteFloat3(*(_float3*)&pAIAnimMesh->mTextureCoords[i]);
		}
	}

	WriteFloat(pAIAnimMesh->mWeight);
}

void ANIM_MESH::Deserialization(ANIM_MESH* tAnimMesh, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tAnimMesh->m_NumVertices, hFile, dwByte);
	ReadCHAR(tAnimMesh->m_szName);

	tAnimMesh->m_Vertices = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadFloat3(tAnimMesh->m_Vertices[i]);

	tAnimMesh->m_Normals = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadFloat3(tAnimMesh->m_Normals[i]);

	tAnimMesh->m_Tangents = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadFloat3(tAnimMesh->m_Tangents[i]);

	tAnimMesh->m_Bitangents = new XMFLOAT3[tAnimMesh->m_NumVertices];
	for (size_t i = 0; i < tAnimMesh->m_NumVertices; ++i)
		ReadFloat3(tAnimMesh->m_Bitangents[i]);
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		tAnimMesh->m_Colors[i] = new XMFLOAT4[tAnimMesh->m_NumVertices];
		for (size_t j = 0; j < tAnimMesh->m_NumVertices; ++j)
		{
			ReadFloat4(tAnimMesh->m_Colors[i][j]);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		tAnimMesh->m_TextureCoords[i] = new XMFLOAT3[tAnimMesh->m_NumVertices];
		for (size_t j = 0; j < tAnimMesh->m_NumVertices; ++j)
		{
			ReadFloat3(tAnimMesh->m_TextureCoords[i][j]);
		}
	}

	ReadFloat(tAnimMesh->m_Weight);
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

	WriteUINT(pAITexture->mWidth);
	WriteUINT(pAITexture->mHeight);	
	WriteFile(hFile, pAITexture->achFormatHint, sizeof(char) * HINTMAXTEXTURELEN, &dwByte, nullptr);
	for(size_t i = 0; i < pAITexture->mWidth * pAITexture->mHeight; ++i)
		TEXEL::Serialization(&pAITexture->pcData[i], hFile, dwByte);
	WriteCHAR(pAITexture->mFilename.data);

}

void TEXTURE::Deserialization(TEXTURE* tTexture, HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return;

	ReadUINT(tTexture->m_Width);
	ReadUINT(tTexture->m_Height);
	ReadFile(hFile, tTexture->m_achFormatHint, sizeof(char) * HINTMAXTEXTURELEN, &dwByte, nullptr);
	tTexture->m_pcData = new TEXEL[tTexture->m_Width * tTexture->m_Height];
	for (size_t i = 0; i < tTexture->m_Width * tTexture->m_Height; ++i)
		TEXEL::Deserialization(&tTexture->m_pcData[i], hFile, dwByte);
	ReadCHAR(tTexture->m_szFilename);
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
	ReadFile(hFile, &tTexel->b, sizeof(char) * 4, &dwByte, nullptr);
}

LIGHT::~LIGHT()
{
}

void LIGHT::Serialization(aiLight* pAILight, HANDLE hFile, DWORD& dwByte)
{

}

void LIGHT::Deserialization(LIGHT* tLight, HANDLE hFile, DWORD& dwByte)
{
}
