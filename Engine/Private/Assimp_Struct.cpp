#include "Assimp_Struct.h"

META_DATA_ENTRY::META_DATA_ENTRY()
	: m_eType(_META_MAX)
{
	
}
META_DATA_ENTRY::~META_DATA_ENTRY()
{
	Safe_Delete(m_Data);
}

void META_DATA_ENTRY::Serialization(aiMetadataEntry* pAIMetadataEntry, HANDLE hFile, DWORD& dwByte)
{
	if (nullptr == pAIMetadataEntry)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);	

	WriteVoid(&pAIMetadataEntry->mType, sizeof(E_META_DATA_TYPE));

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
		iSize = sizeof(AI_STRING);
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

bool META_DATA_ENTRY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_eType, sizeof(E_META_DATA_TYPE));
	unsigned int iSize = 0;

	switch (m_eType)
	{
	case _BOOL:
		m_Data = new _bool;
		break;
	case _INT32:
		m_Data = new int;
		break;
	case _UINT64:
		m_Data = new unsigned long long;
		break;
	case _FLOAT:
		m_Data = new _float;
		break;
	case _DOUBLE:
		m_Data = new _double;
		break;
	case _AISTRING:
		m_Data = new AI_STRING;
		break;
	case _AIVECTOR3D:
		m_Data = new _float3;
		break;
	case _AIMETADATA:
		m_Data = new META_DATA;
		break;
	case _INT64:
		m_Data = new long long;
		break;
	case _UINT32:
		m_Data = new unsigned int;
		break;
	case _META_MAX:
		m_Data = nullptr;
		break;
	}

	
	ReadVoid(&iSize, sizeof(iSize));
	ReadVoid(m_Data, iSize);
	return true;
}

META_DATA::META_DATA()
	: m_NumProperties(0)
{
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
	unsigned int size = pAIMetadata->mNumProperties;

	if (0 == size)
		return;

	for (size_t i = 0; i < size; ++i)
		AI_STRING::Serialization(&pAIMetadata->mKeys[i], hFile, dwByte);

	for (size_t i = 0; i < size; ++i)
		META_DATA_ENTRY::Serialization(&pAIMetadata->mValues[i], hFile, dwByte);
}

bool META_DATA::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_NumProperties, sizeof(m_NumProperties));
	unsigned int size = m_NumProperties;

	if (0 == size)
		return false;

	m_Keys = new AI_STRING[size];
	for (size_t i = 0; i < size; ++i)
	{
		m_Keys[i].Deserialization(hFile, dwByte);
	}
	
	m_Values = new META_DATA_ENTRY[size];
	for (size_t i = 0; i < size; ++i)
		m_Values[i].Deserialization(hFile, dwByte);

	return true;
}

NODE::NODE()
	: m_Name()
{
	ZeroMemory(&m_Transformation, sizeof(XMFLOAT4X4));
}

NODE::~NODE()
{
	Safe_Delete_Array(m_Children);
	Safe_Delete(m_MetaData);
	Safe_Delete_Array(m_Meshes);
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

	WriteVoid(&pAINode->mTransformation, sizeof(pAINode->mTransformation));

	WriteVoid(&pAINode->mNumMeshes, sizeof(pAINode->mNumMeshes));
	for (size_t i = 0; i < pAINode->mNumMeshes; ++i)
	{
		WriteVoid(&pAINode->mMeshes[i], sizeof(pAINode->mMeshes[i]));
	}

	META_DATA::Serialization(pAINode->mMetaData, hFile, dwByte);

	WriteVoid(&pAINode->mNumChildren, sizeof(pAINode->mNumChildren));
	for (size_t i = 0; i < pAINode->mNumChildren; ++i)
	{
		NODE::Serialization(pAINode->mChildren[i], hFile, dwByte);
	}
}

bool NODE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);

	ReadVoid(&m_Transformation, sizeof(m_Transformation));

	ReadVoid(&m_NumMeshes, sizeof(m_NumMeshes));

	if (0 != m_NumMeshes)
	{
		m_Meshes = new unsigned int[m_NumMeshes];
		for (size_t i = 0; i < m_NumMeshes; ++i)
		{
			ReadVoid(&m_Meshes[i], sizeof(m_Meshes[i]));
		}
	}


	m_MetaData = new META_DATA;
	if (!m_MetaData->Deserialization(hFile, dwByte))
		Safe_Delete(m_MetaData);

	ReadVoid(&m_NumChildren, sizeof(m_NumChildren));

	if (0 == m_NumChildren)
		return true;

	m_Children = new NODE[m_NumChildren];
	for (size_t i = 0; i < m_NumChildren; ++i)
	{
		m_Children[i].Deserialization(hFile, dwByte);
	}

	return true;
}

FACE::FACE()
{
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

	WriteVoid(&pAIFace->mNumIndices, sizeof(unsigned int));
	for(size_t i = 0; i < pAIFace->mNumIndices; ++i)
		WriteVoid(&pAIFace->mIndices[i], sizeof(unsigned int));
}

bool FACE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_NumIndices, sizeof(m_NumIndices));

	if (0 != m_NumIndices)
	{
		m_Indices = new unsigned int[m_NumIndices];
		for (size_t i = 0; i < m_NumIndices; ++i)
			ReadVoid(&m_Indices[i], sizeof(unsigned int));
	}

	return true;
}

AABB::AABB()
{
	ZeroMemory(&m_Min, sizeof(XMFLOAT3));
	ZeroMemory(&m_Max, sizeof(XMFLOAT3));
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

bool AABB::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Min, sizeof(m_Min));
	ReadVoid(&m_Max, sizeof(m_Max));

	return true;
}

MESH::MESH()
	: m_Colors{}
	, m_TextureCoords{}
	, m_NumUVComponents{}
	, m_Name{}
	, m_eMethod(MorphingMethod_UNKNOWN)
	, m_AABB{}
{
	ZeroMemory(&m_TextureCoordsNames[0], sizeof(void*) * AI_MAX_NUMBER_OF_TEXTURECOORDS);
}

MESH::~MESH()
{
	for(size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		Safe_Delete(m_TextureCoordsNames[i]);
	Safe_Delete_Array(m_AnimMeshes);
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
		if (nullptr == pAIMesh->mColors[i])
		{
			WriteEnable(false);
			continue;
		}
		WriteEnable(true);

		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			COLOR_RGBA_FLOAT::Serialization(pAIMesh->mColors[i], hFile, dwByte);
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (!pAIMesh->HasTextureCoords(i))
		{
			WriteEnable(false);
			continue;
		}
		WriteEnable(true);

		for (size_t j = 0; j < pAIMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIMesh->mTextureCoords[i][j], sizeof(pAIMesh->mTextureCoords[i][j]));
		}
	}
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (!pAIMesh->HasTextureCoords(i))
		{
			WriteEnable(false);
			continue;
		}
		WriteEnable(true);

		WriteVoid(&pAIMesh->mNumUVComponents[i], sizeof(pAIMesh->mNumUVComponents[i]));
	}
	// 여기차례임
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
	{
		if(!pAIMesh->HasTextureCoordsName(i))
		{
			WriteEnable(false);
			continue;
		}
		WriteEnable(true);

		AI_STRING::Serialization(pAIMesh->mTextureCoordsNames[i], hFile, dwByte);
	}		
}

bool MESH::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
	{
		return false;
	}

	ReadVoid(&m_PrimitiveTypes, sizeof(m_PrimitiveTypes));
	ReadVoid(&m_NumVertices, sizeof(m_NumVertices));
	ReadVoid(&m_NumFaces, sizeof(m_NumFaces));

	if (0 != m_NumVertices)
	{
		m_Vertices = new _float3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
		{
			ReadVoid(&m_Vertices[i], sizeof(m_Vertices[i]));
		}

		m_Normals = new _float3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
		{
			ReadVoid(&m_Normals[i], sizeof(m_Normals[i]));
		}

		m_Tangents = new _float3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
		{
			ReadVoid(&m_Tangents[i], sizeof(m_Tangents[i]));
		}

		m_Bitangents = new _float3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
		{
			ReadVoid(&m_Bitangents[i], sizeof(m_Bitangents[i]));
		}
	}
	
	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		if (false == ReadEnable())
			continue;
		
		if (0 != m_NumVertices)
		{
			m_Colors[i] = new COLOR_RGBA_FLOAT[m_NumVertices];
			for (size_t j = 0; j < m_NumVertices; ++j)
			{
				m_Colors[i]->Deserialization(hFile, dwByte);
			}
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (false == ReadEnable())
			continue;
		if (0 != m_NumVertices)
		{
			m_TextureCoords[i] = new XMFLOAT3[m_NumVertices];
			for (size_t j = 0; j < m_NumVertices; ++j)
			{
				ReadVoid(&m_TextureCoords[i][j], sizeof(m_TextureCoords[i][j]));
			}
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (false == ReadEnable())
			continue;

		ReadVoid(&m_NumUVComponents[i], sizeof(m_NumUVComponents[i]));
	}

	if (0 != m_NumFaces)
	{
		m_Faces = new FACE[m_NumFaces];
		for (size_t i = 0; i < m_NumFaces; ++i)
		{
			m_Faces[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumBones, sizeof(m_NumBones));
	if (0 != m_NumBones)
	{
		m_Bones = new BONE[m_NumBones];
		for (size_t i = 0; i < m_NumBones; ++i)
		{
			m_Bones[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_MaterialIndex, sizeof(m_MaterialIndex));
	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumAnimMeshes, sizeof(m_NumAnimMeshes));

	if (0 != m_NumAnimMeshes)
	{
		m_AnimMeshes = new ANIM_MESH[m_NumAnimMeshes];
		for (size_t i = 0; i < m_NumAnimMeshes; ++i)
		{
			m_AnimMeshes[i].Deserialization(hFile, dwByte);
		}
	}


	ReadVoid(&m_eMethod, sizeof(m_eMethod));
	m_AABB.Deserialization(hFile, dwByte);

	
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (false == ReadEnable())
			continue;

		m_TextureCoordsNames[i] = new AI_STRING;
		if (!m_TextureCoordsNames[i]->Deserialization(hFile, dwByte))
			Safe_Delete(m_TextureCoordsNames[i]);
	}

	return true;
}

MATERIAL::MATERIAL()
{
	ZeroMemory(&m_TexturePath[0], sizeof(AI_STRING*) * TextureType_TRANSMISSION);
}

MATERIAL::~MATERIAL()
{
	Safe_Delete_Array(m_Properties);
	for (size_t i = 0; i < TextureType_TRANSMISSION; ++i)
	{
		Safe_Delete(m_TexturePath[i]);
	}
}

bool MATERIAL::GetTexture(TextureType type, unsigned int index, AI_STRING* path)
{
	if(nullptr == m_TexturePath[type])
		return false;

	strcpy_s(path->m_data, m_TexturePath[type]->m_data);

	return true;
}

void MATERIAL::Serialization(aiMaterial* pAIMaterial, HANDLE hFile, DWORD& dwByte)
{
	if (pAIMaterial == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	for (size_t i = 0; i < aiTextureType_TRANSMISSION; ++i)
	{
		aiString path;
		if (AI_SUCCESS == pAIMaterial->GetTexture(aiTextureType(i), 0, &path))
		{
			WriteEnable(true);
			AI_STRING::Serialization(&path, hFile, dwByte);
		}
		else
			WriteEnable(false);
	}
	
	WriteVoid(&pAIMaterial->mNumProperties, sizeof(pAIMaterial->mNumProperties));
	WriteVoid(&pAIMaterial->mNumAllocated, sizeof(pAIMaterial->mNumAllocated));

	for(size_t i = 0; i < pAIMaterial->mNumProperties; ++i)
		MATERIAL_PROPERTY::Serialization(pAIMaterial->mProperties[i], hFile, dwByte);
}

bool MATERIAL::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	for (size_t i = 0; i < TextureType_TRANSMISSION; ++i)
	{
		m_TexturePath[i] = nullptr;
		if (true == ReadEnable())
		{
			m_TexturePath[i] = new AI_STRING;
			m_TexturePath[i]->Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumProperties, sizeof(m_NumProperties));
	ReadVoid(&m_NumAllocated, sizeof(m_NumAllocated));

	if (0 != m_NumProperties)
	{
		m_Properties = new MATERIAL_PROPERTY[m_NumProperties];
		for (size_t i = 0; i < m_NumProperties; ++i)
		{
			m_Properties[i].Deserialization(hFile, dwByte);
		}
	}

	return true;
}

MATERIAL_PROPERTY::MATERIAL_PROPERTY()
	: m_Key{}
	, m_eType{ _PTI_Force32Bit }
{
}

MATERIAL_PROPERTY::~MATERIAL_PROPERTY()
{
	Safe_Delete(m_Data);
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
	if (0 != pAIMaterialProperty->mDataLength)
	{
		WriteVoid(pAIMaterialProperty->mData, pAIMaterialProperty->mDataLength);
	}
}

bool MATERIAL_PROPERTY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Key.Deserialization(hFile, dwByte);
	ReadVoid(&m_Semantic, sizeof(m_Semantic));
	ReadVoid(&m_Index, sizeof(m_Index));
	ReadVoid(&m_DataLength, sizeof(m_DataLength));
	ReadVoid(&m_eType, sizeof(m_eType));

	if (0 != m_DataLength)
	{
		m_Data = new char[m_DataLength];
		ReadVoid(&m_Data[0], m_DataLength);
	}

	return true;
}

// 애니메이션 부터 하면됨.
ANIMATION::ANIMATION()
	: m_Name{}
{
}

ANIMATION::~ANIMATION()
{
	Safe_Delete_Array(m_MorphMeshChannels);

	Safe_Delete_Array(m_MeshChannels);

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

bool ANIMATION::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_Duration, sizeof(m_Duration));
	ReadVoid(&m_TicksPerSecond, sizeof(m_TicksPerSecond));
	ReadVoid(&m_NumChannels, sizeof(m_NumChannels));

	if (0 != m_NumChannels)
	{
		m_Channels = new NODE_ANIM[m_NumChannels];
		for (size_t i = 0; i < m_NumChannels; ++i)
		{
			m_Channels[i].Deserialization(hFile, dwByte);
		}
	}
	ReadVoid(&m_NumMeshChannels, sizeof(m_NumMeshChannels));
	if(0 != m_MeshChannels)
	{
		m_MeshChannels = new MESH_ANIM[m_NumMeshChannels];
		for (size_t i = 0; i < m_NumMeshChannels; ++i)
		{
			m_MeshChannels[i].Deserialization(hFile, dwByte);
		}
	}	

	ReadVoid(&m_NumMorphMeshChannels, sizeof(m_NumMorphMeshChannels));
	if (0 != m_NumMeshChannels)
	{
		m_MorphMeshChannels = new MESH_MORPH_ANIM[m_NumMorphMeshChannels];
		for (size_t i = 0; i < m_NumMorphMeshChannels; ++i)
		{
			m_MorphMeshChannels[i].Deserialization(hFile, dwByte);
		}
	}

	return true;
}

NODE_ANIM::NODE_ANIM()
	: m_NodeName{}
	, m_ePreState{ _AnimBehaviour_Force32Bit }
	, m_ePostState{ _AnimBehaviour_Force32Bit }
{

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
	for (size_t i = 0; i < pAINodeAnim->mNumPositionKeys; ++i)
	{
		VECTOR_KEY::Serialization(&pAINodeAnim->mPositionKeys[i], hFile, dwByte);
	}

	WriteVoid(&pAINodeAnim->mNumRotationKeys, sizeof(pAINodeAnim->mNumRotationKeys));
	for (size_t i = 0; i < pAINodeAnim->mNumRotationKeys; ++i)
	{
		QUAT_KEY::Serialization(&pAINodeAnim->mRotationKeys[i], hFile, dwByte);
	}

	WriteVoid(&pAINodeAnim->mNumScalingKeys, sizeof(pAINodeAnim->mNumScalingKeys));
	for (size_t i = 0; i < pAINodeAnim->mNumScalingKeys; ++i)
	{
		VECTOR_KEY::Serialization(&pAINodeAnim->mScalingKeys[i], hFile, dwByte);
	}

	WriteVoid(&pAINodeAnim->mPreState, sizeof(pAINodeAnim->mPreState));
	WriteVoid(&pAINodeAnim->mPostState, sizeof(pAINodeAnim->mPostState));
}

bool NODE_ANIM::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_NodeName.Deserialization(hFile, dwByte);

	ReadVoid(&m_NumPositionKeys, sizeof(m_NumPositionKeys));

	if (m_NumPositionKeys != 0)
	{
		m_PositionKeys = new VECTOR_KEY[m_NumPositionKeys];
		for (size_t i = 0; i < m_NumPositionKeys; ++i)
			m_PositionKeys[i].Deserialization(hFile, dwByte);
	}


	ReadVoid(&m_NumRotationKeys, sizeof(m_NumRotationKeys));
	if (0 != m_NumRotationKeys)
	{
		m_RotationKeys = new QUAT_KEY[m_NumRotationKeys];
		for (size_t i = 0; i < m_NumRotationKeys; ++i)
			m_RotationKeys[i].Deserialization(hFile, dwByte);
	}

	ReadVoid(&m_NumScalingKeys, sizeof(m_NumScalingKeys));
	if (0 != m_NumScalingKeys)
	{
		m_ScalingKeys = new VECTOR_KEY[m_NumScalingKeys];
		for (size_t i = 0; i < m_NumScalingKeys; ++i)
			m_ScalingKeys[i].Deserialization(hFile, dwByte);
	}

	ReadVoid(&m_ePreState, sizeof(m_ePreState));
	ReadVoid(&m_ePostState, sizeof(m_ePostState));
	return true;
}

VECTOR_KEY::VECTOR_KEY()
{
	ZeroMemory(&m_Value, sizeof(XMFLOAT3));
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

bool VECTOR_KEY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Time, sizeof(m_Time));
	ReadVoid(&m_Value, sizeof(m_Value));

	return true;
}

QUAT_KEY::QUAT_KEY()
	: m_Value{}
{
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

bool QUAT_KEY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Time, sizeof(m_Time));
	ReadVoid(&m_Value, sizeof(m_Value));

	return true;
}

MESH_ANIM::MESH_ANIM()
	: m_Name{}
	, m_Keys{}
{

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

bool MESH_ANIM::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumKeys, sizeof(m_NumKeys));
	
	if (0 != m_NumKeys)
	{
		m_Keys = new MESH_KEY[m_NumKeys];
		for (size_t i = 0; i < m_NumKeys; ++i)
			m_Keys[i].Deserialization(hFile, dwByte);
	}	

	return true;
}

MESH_KEY::MESH_KEY()
{
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

bool MESH_KEY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Time, sizeof(m_Time));
	ReadVoid(&m_Value, sizeof(m_Value));

	return true;
}

MESH_MORPH_ANIM::MESH_MORPH_ANIM()
	: m_Name{}
{
	
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
	{
		MESH_MORPH_KEY::Serialization(&pAIMeshMorphAnim->mKeys[i], hFile, dwByte);
	}
		
}

bool MESH_MORPH_ANIM::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumKeys, sizeof(m_NumKeys));
	
	if (0 != m_NumKeys)
	{
		m_Keys = new MESH_MORPH_KEY[m_NumKeys];
		for (size_t i = 0; i < m_NumKeys; ++i)
			m_Keys[i].Deserialization(hFile, dwByte);
	}

	return true;
}

BONE::BONE()
	: m_Name{}
	, m_OffsetMatrix{}
{
}

BONE::~BONE()
{
	Safe_Delete(m_Armature);
	Safe_Delete(m_Node);
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
	WriteVoid(&pAIBone->mNumWeights, sizeof(unsigned int));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE::Serialization(pAIBone->mArmature, hFile, dwByte);
	NODE::Serialization(pAIBone->mNode, hFile, dwByte);
#endif
	for (size_t i = 0; i < pAIBone->mNumWeights; ++i)
		VERTEX_WEIGHT::Serialization(&pAIBone->mWeights[i], hFile, dwByte);
	WriteVoid(&pAIBone->mOffsetMatrix, sizeof(pAIBone->mOffsetMatrix));
}

bool BONE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumWeights, sizeof(unsigned int));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	m_Armature = new NODE;
	if (!m_Armature->Deserialization(hFile, dwByte))
		Safe_Delete(m_Armature);

	m_Node = new NODE;
	if(!m_Node->Deserialization(hFile, dwByte))
		Safe_Delete(m_Node);
#endif
	
	if (0 != m_NumWeights)
	{
		m_Weights = new VERTEX_WEIGHT[m_NumWeights];
		for (size_t i = 0; i < m_NumWeights; ++i)
			m_Weights[i].Deserialization(hFile, dwByte);
	}
	ReadVoid(&m_OffsetMatrix, sizeof(m_OffsetMatrix));

	return true;
}

VERTEX_WEIGHT::VERTEX_WEIGHT()
{
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

bool VERTEX_WEIGHT::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_VertexId, sizeof(m_VertexId));
	ReadVoid(&m_Weight, sizeof(m_Weight));

	return true;
}
ANIM_MESH::ANIM_MESH()
	: m_Name{}
	, m_Colors{}
	, m_TextureCoords{}
{
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
	// 이곳 좀 찜찜하긴함. AI_MAX_NUMBER_OF_COLOR_SETS에 대해서 writeenable처리안해줫음
	for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIAnimMesh->mColors[i][j], sizeof(pAIAnimMesh->mColors[i][j]));
		}
	}

	for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		for (size_t j = 0; j < pAIAnimMesh->mNumVertices; ++j)
		{
			WriteVoid(&pAIAnimMesh->mTextureCoords[i][j], sizeof(pAIAnimMesh->mTextureCoords[i][j]));
		}
	}

	WriteVoid(&pAIAnimMesh->mWeight, sizeof(pAIAnimMesh->mWeight));
}

bool ANIM_MESH::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumVertices, sizeof(m_NumVertices));

	if (0 != m_NumVertices)
	{
		m_Vertices = new XMFLOAT3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
			ReadVoid(&m_Vertices[i], sizeof(m_Vertices[i]));

		m_Normals = new XMFLOAT3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
			ReadVoid(&m_Normals[i], sizeof(m_Normals[i]));

		m_Tangents = new XMFLOAT3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
			ReadVoid(&m_Tangents[i], sizeof(m_Tangents[i]));

		m_Bitangents = new XMFLOAT3[m_NumVertices];
		for (size_t i = 0; i < m_NumVertices; ++i)
			ReadVoid(&m_Bitangents[i], sizeof(m_Bitangents[i]));

		for (size_t i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
		{
			m_Colors[i] = new XMFLOAT4[m_NumVertices];
			for (size_t j = 0; j < m_NumVertices; ++j)
			{
				ReadVoid(&m_Colors[i][j], sizeof(m_Colors[i][j]));
			}
		}

		for (size_t i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
		{
			m_TextureCoords[i] = new XMFLOAT3[m_NumVertices];
			for (size_t j = 0; j < m_NumVertices; ++j)
			{
				ReadVoid(&m_TextureCoords[i][j], sizeof(m_TextureCoords[i][j]));
			}
		}
	}	

	ReadVoid(&m_Weight, sizeof(m_Weight));

	return true;
}

TEXTURE::TEXTURE()
	: m_achFormatHint{}
	, m_Filename{}
{
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

bool TEXTURE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Width, sizeof(m_Width));
	ReadVoid(&m_Height, sizeof(m_Height));
	ReadVoid(&m_achFormatHint, sizeof(char) * HINTMAXTEXTURELEN);
	if (0 != m_Width &&  0 != m_Height)
	{
		m_pcData = new TEXEL[m_Width * m_Height];
		for (size_t i = 0; i < m_Width * m_Height; ++i)
			m_pcData[i].Deserialization(hFile, dwByte);
	}

	m_Filename.Deserialization(hFile, dwByte);

	return true;
}

TEXEL::TEXEL()
	: r{}
	, g{}
	, b{}
	, a{}
{
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

bool TEXEL::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&b, sizeof(char) * 4);

	return true;
}

LIGHT::LIGHT()
	: m_Name{}
	, m_eType{ _LightSource_Force32Bit }
	, m_Position{}
	, m_Direction{}
	, m_Up{}
	, m_ColorDiffuse{}
	, m_ColorSpecular{}
	, m_ColorAmbient{}
	, m_Size{}
{
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
	
	AI_STRING::Serialization(&pAILight->mName, hFile, dwByte);
	WriteVoid(&pAILight->mType, sizeof(pAILight->mType));
	WriteVoid(&pAILight->mPosition, sizeof(pAILight->mPosition));
	WriteVoid(&pAILight->mDirection, sizeof(pAILight->mDirection));
	WriteVoid(&pAILight->mUp, sizeof(pAILight->mUp));
	WriteVoid(&pAILight->mAttenuationConstant, sizeof(pAILight->mAttenuationConstant));
	WriteVoid(&pAILight->mAttenuationLinear, sizeof(pAILight->mAttenuationLinear));
	WriteVoid(&pAILight->mAttenuationQuadratic, sizeof(pAILight->mAttenuationQuadratic));
	WriteVoid(&pAILight->mColorDiffuse, sizeof(pAILight->mColorDiffuse));
	WriteVoid(&pAILight->mColorSpecular, sizeof(pAILight->mColorSpecular));
	WriteVoid(&pAILight->mColorAmbient, sizeof(pAILight->mColorAmbient));
	WriteVoid(&pAILight->mAngleInnerCone, sizeof(pAILight->mAngleInnerCone));
	WriteVoid(&pAILight->mAngleOuterCone, sizeof(pAILight->mAngleOuterCone));
	WriteVoid(&pAILight->mSize, sizeof(pAILight->mSize));
}

bool LIGHT::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_eType, sizeof(m_eType));
	ReadVoid(&m_Position, sizeof(m_Position));
	ReadVoid(&m_Direction, sizeof(m_Direction));
	ReadVoid(&m_Up, sizeof(m_Up));
	ReadVoid(&m_AttenuationConstant, sizeof(m_AttenuationConstant));
	ReadVoid(&m_AttenuationLinear, sizeof(m_AttenuationLinear));
	ReadVoid(&m_AttenuationQuadratic, sizeof(m_AttenuationQuadratic));
	ReadVoid(&m_ColorDiffuse, sizeof(m_ColorDiffuse));
	ReadVoid(&m_ColorSpecular, sizeof(m_ColorSpecular));
	ReadVoid(&m_ColorAmbient, sizeof(m_ColorAmbient));
	ReadVoid(&m_AngleInnerCone, sizeof(m_AngleInnerCone));
	ReadVoid(&m_AngleOuterCone, sizeof(m_AngleOuterCone));
	ReadVoid(&m_Size, sizeof(m_Size));
	
	return true;
}

CAMERA::CAMERA()
	: m_Name{}
	, m_Position{}
	, m_Up{}
	, m_LookAt{}
{
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

	AI_STRING::Serialization(&pAICamera->mName, hFile, dwByte);
	WriteVoid(&pAICamera->mPosition, sizeof(m_Position));
	WriteVoid(&pAICamera->mUp, sizeof(m_Up));
	WriteVoid(&pAICamera->mLookAt, sizeof(m_LookAt));
	WriteVoid(&pAICamera->mHorizontalFOV, sizeof(m_HorizontalFOV));
	WriteVoid(&pAICamera->mClipPlaneNear, sizeof(m_ClipPlaneNear));
	WriteVoid(&pAICamera->mClipPlaneFar, sizeof(m_ClipPlaneFar));
	WriteVoid(&pAICamera->mAspect, sizeof(m_Aspect));
	WriteVoid(&pAICamera->mOrthographicWidth, sizeof(m_OrthographicWidth));
}

bool CAMERA::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_Position, sizeof(m_Position));
	ReadVoid(&m_Up, sizeof(m_Up));
	ReadVoid(&m_LookAt, sizeof(m_LookAt));
	ReadVoid(&m_HorizontalFOV, sizeof(m_HorizontalFOV));
	ReadVoid(&m_ClipPlaneNear, sizeof(m_ClipPlaneNear));
	ReadVoid(&m_ClipPlaneFar, sizeof(m_ClipPlaneFar));
	ReadVoid(&m_Aspect, sizeof(m_Aspect));
	ReadVoid(&m_OrthographicWidth, sizeof(m_OrthographicWidth));

	return true;
}

SKELETON::SKELETON()
	: m_Name{}
	, m_Bones{}
{
}

SKELETON::~SKELETON()
{
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

bool SKELETON::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	m_Name.Deserialization(hFile, dwByte);
	ReadVoid(&m_NumBones, sizeof(m_NumBones));

	if (0 != m_NumBones)
	{
		m_Bones = new SKELETON_BONE[m_NumBones];
		for (size_t i = 0; i < m_NumBones; i++) {
			m_Bones[i].Deserialization(hFile, dwByte);
		}
	}

	return true;
}

SKELETON_BONE::SKELETON_BONE()
	: m_OffsetMatrix{}
	, m_LocalMatrix{}
{
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

bool SKELETON_BONE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Parent, sizeof(m_Parent));
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	m_Armature = new NODE;
	if(!m_Armature->Deserialization(hFile, dwByte))
		Safe_Delete(m_Armature);
	m_Node = new NODE;
	if (!m_Node->Deserialization(hFile, dwByte))
		Safe_Delete(m_Node);
#endif
	ReadVoid(&m_NumnWeights, sizeof(m_NumnWeights));
	
	m_MeshId = new MESH;
	if (!m_MeshId->Deserialization(hFile, dwByte))
		Safe_Delete(m_MeshId);
	
	if (0 != m_NumnWeights)
	{
		m_Weights = new VERTEX_WEIGHT[m_NumnWeights];
		for (size_t i = 0; i < m_NumnWeights; ++i)
			m_Weights[i].Deserialization(hFile, dwByte);
	}

	ReadVoid(&m_OffsetMatrix, sizeof(m_OffsetMatrix));
	ReadVoid(&m_LocalMatrix, sizeof(m_LocalMatrix));

	return true;
}

SCENE::SCENE()
	: m_Name{}
{
}

SCENE::~SCENE()
{
	// Skeletons 메모리 해제
	Safe_Delete_Array(m_Skeletons);

	// m_MetaData 메모리 해제
	Safe_Delete(m_MetaData);

	// Cameras 메모리 해제
	Safe_Delete_Array(m_Cameras);

	// Lights 메모리 해제
	Safe_Delete_Array(m_Lights);

	// Textures 메모리 해제
	Safe_Delete_Array(m_Textures);

	// Animations 메모리 해제
	Safe_Delete_Array(m_Animations);

	// Materials 메모리 해제
	Safe_Delete_Array(m_Materials);

	// Meshes 메모리 해제
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
	for (size_t i = 0; i < pAIScene->mNumAnimations; ++i)
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

	WriteVoid(&pAIScene->mNumSkeletons, sizeof(pAIScene->mNumSkeletons));
	for (size_t i = 0; i < pAIScene->mNumCameras; ++i)
		SKELETON::Serialization(pAIScene->mSkeletons[i], hFile, dwByte);
}

bool SCENE::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;
	
	ReadVoid(&m_Flags, sizeof(m_Flags));
	m_RootNode = new NODE;
	if (!m_RootNode->Deserialization(hFile, dwByte))
		Safe_Delete(m_RootNode);

	ReadVoid(&m_NumMeshes, sizeof(m_NumMeshes));
	if (0 != m_NumMeshes)
	{
		m_Meshes = new MESH[m_NumMeshes];
		for (size_t i = 0; i < m_NumMeshes; ++i)
		{
			m_Meshes[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumMaterials, sizeof(m_NumMaterials));
	if (0 != m_NumMaterials)
	{
		m_Materials = new MATERIAL[m_NumMaterials];
		for (size_t i = 0; i < m_NumMaterials; ++i)
		{
			m_Materials[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumAnimations, sizeof(m_NumAnimations));
	if (0 != m_NumAnimations)
	{
		m_Animations = new ANIMATION[m_NumAnimations];
		for (size_t i = 0; i < m_NumAnimations; ++i)
		{
			m_Animations[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumTextures, sizeof(m_NumTextures));
	if (0 != m_NumAnimations)
	{
		m_Textures = new TEXTURE[m_NumTextures];
		for (size_t i = 0; i < m_NumTextures; ++i)
		{
			m_Textures[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumLights, sizeof(m_NumLights));
	if (0 != m_NumLights)
	{
		m_Lights = new LIGHT[m_NumLights];
		for (size_t i = 0; i < m_NumLights; ++i)
		{
			m_Lights[i].Deserialization(hFile, dwByte);
		}
	}

	ReadVoid(&m_NumCameras, sizeof(m_NumCameras));
	if (0 != m_NumCameras)
	{
		m_Cameras = new CAMERA[m_NumCameras];
		for (size_t i = 0; i < m_NumCameras; ++i)
		{
			m_Cameras[i].Deserialization(hFile, dwByte);
		}
	}

	m_MetaData = new META_DATA;
	if (!m_MetaData->Deserialization(hFile, dwByte))
		Safe_Delete(m_MetaData);

	m_Name.Deserialization(hFile, dwByte);

	ReadVoid(&m_NumSkeletons, sizeof(m_NumSkeletons));
	if (0 != m_NumSkeletons)
	{
		m_Skeletons = new SKELETON[m_NumSkeletons];
		for (size_t i = 0; i < m_NumSkeletons; ++i)
		{
			m_Skeletons[i].Deserialization(hFile, dwByte);
		}
	}

	return true;
}

AI_STRING::AI_STRING()
{
	ZeroMemory(m_data, sizeof(char) * 1024Ui64);
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

	WriteVoid(pAIString->data, sizeof(char) * 1024Ui64);
}

bool AI_STRING::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(m_data, sizeof(char) * 1024Ui64);

	return true;
}

MESH_MORPH_KEY::MESH_MORPH_KEY()
{

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

bool MESH_MORPH_KEY::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&m_Time, sizeof(m_Time));
	ReadVoid(&m_NumValuesAndWeights, sizeof(m_NumValuesAndWeights));

	if (0 != m_NumValuesAndWeights)
	{
		m_Values = new unsigned int[m_NumValuesAndWeights];
		for (size_t i = 0; i < m_NumValuesAndWeights; ++i)
			ReadVoid(&m_Values[i], sizeof(m_Values[i]));

		m_Weights = new double[m_NumValuesAndWeights];
		for (size_t i = 0; i < m_NumValuesAndWeights; ++i)
			ReadVoid(&m_Weights[i], sizeof(m_Weights[i]));
	}

	return true;
}

COLOR_RGBA_FLOAT::COLOR_RGBA_FLOAT()
{
}

COLOR_RGBA_FLOAT::~COLOR_RGBA_FLOAT()
{
}

void COLOR_RGBA_FLOAT::Serialization(const aiColor4D* pAIString, HANDLE hFile, DWORD& dwByte)
{
	if (pAIString == nullptr)
	{
		WriteEnable(false);
		return;
	}
	WriteEnable(true);

	WriteVoid(&pAIString->r, sizeof(_float));
	WriteVoid(&pAIString->g, sizeof(_float));
	WriteVoid(&pAIString->b, sizeof(_float));
	WriteVoid(&pAIString->a, sizeof(_float));
}

bool COLOR_RGBA_FLOAT::Deserialization(HANDLE hFile, DWORD& dwByte)
{
	if (false == ReadEnable())
		return false;

	ReadVoid(&r, sizeof(_float));
	ReadVoid(&g, sizeof(_float));
	ReadVoid(&b, sizeof(_float));
	ReadVoid(&a, sizeof(_float));

	return true;
}
