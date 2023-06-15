#include "Assimp_Struct.h"
#include "Engine_Defines.h"

void SCENE::Serialization(aiScene* pAIScene, HANDLE hFile, DWORD& dwByte)
{
	WriteUINT(pAIScene->mFlags);
	NODE::Serialization(pAIScene->mRootNode, hFile, dwByte);

	WriteUINT(pAIScene->mNumMeshes);
	for (size_t i = 0; i < pAIScene->mNumMeshes; ++i)
		MESH::Serialization(pAIScene->mMeshes[i], hFile, dwByte);

	WriteUINT(pAIScene->mNumMaterials);
	for (size_t i = 0; i < pAIScene->mNumMaterials; ++i)
		MATERIAL::Serialization(pAIScene->mMaterials[i], hFile, dwByte);

	WriteUINT(pAIScene->mNumAnimations);
	for (size_t i = 0; i < pAIScene->mNumAnimations; ++i)
		ANIMATION::Serialization(pAIScene->mAnimations[i], hFile, dwByte);

	WriteUINT(pAIScene->mNumTextures);
	for (size_t i = 0; i < pAIScene->mNumTextures; ++i)
		TEXTURE::Serialization(pAIScene->mTextures[i], hFile, dwByte);

	WriteUINT(pAIScene->mNumLights);
	for (size_t i = 0; i < pAIScene->mNumLights; ++i)
		LIGHT::Serialization(pAIScene->mLights[i], hFile, dwByte);

	WriteUINT(pAIScene->mNumCameras);
	for (size_t i = 0; i < pAIScene->mNumCameras; ++i)
		CAMERA::Serialization(pAIScene->mCameras[i], hFile, dwByte);
	WriteCHAR(pAIScene->mName.data);

	WriteUINT(pAIScene->mNumSkeletons);
	for (size_t i = 0; i < pAIScene->mNumSkeletons; ++i)
		SKELETON::Serialization(pAIScene->mSkeletons[i], hFile, dwByte);
}

void SCENE::Deserialization(SCENE& tScene, HANDLE hFile, DWORD& dwByte)
{
	ReadUINT(tScene.m_Flags);
	NODE::Deserialization(tScene.m_RootNode, hFile, dwByte);

	ReadUINT(tScene.m_NumMeshes);
	for (size_t i = 0; i < tScene.m_NumMeshes; ++i)
		MESH::Deserialization(tScene.m_Meshes[i], hFile, dwByte);

	ReadUINT(tScene.m_NumMaterials);
	for (size_t i = 0; i < tScene.m_NumMaterials; ++i)
		MATERIAL::Deserialization(tScene.m_Materials[i], hFile, dwByte);

	ReadUINT(tScene.m_NumAnimations);
	for (size_t i = 0; i < tScene.m_NumAnimations; ++i)
		ANIMATION::Deserialization(tScene.m_Animations[i], hFile, dwByte);

	ReadUINT(tScene.m_NumTextures);
	for (size_t i = 0; i < tScene.m_NumTextures; ++i)
		TEXTURE::Deserialization(tScene.m_Textures[i], hFile, dwByte);

	ReadUINT(tScene.m_NumLights);
	for (size_t i = 0; i < tScene.m_NumLights; ++i)
		LIGHT::Deserialization(tScene.m_Lights[i], hFile, dwByte);

	ReadUINT(tScene.m_NumCameras);
	for (size_t i = 0; i < tScene.m_NumCameras; ++i)
		CAMERA::Deserialization(tScene.m_Cameras[i], hFile, dwByte);
	ReadCHAR(tScene.m_Name);

	ReadUINT(tScene.m_NumSkeletons);
	for (size_t i = 0; i < tScene.m_NumSkeletons; ++i)
		SKELETON::Deserialization(tScene.m_Skeletons[i], hFile, dwByte);
}

void NODE::Serialization(aiNode* pAINode, HANDLE hFile, DWORD& dwByte)
{
	WriteCHAR(pAINode->mName.data);
	WriteFloat4x4(*((_float4x4*)&pAINode->mTransformation.Transpose()));
	NODE::Serialization(pAINode->mParent, hFile, dwByte);
	WriteUINT(pAINode->mNumChildren);
	for(size_t i = 0; i < pAINode->mNumChildren; ++i)
		NODE::Serialization(pAINode->mChildren[i], hFile, dwByte);
	WriteUINT(pAINode->mNumMeshes);
	for (size_t i = 0; i < pAINode->mNumMeshes; ++i)
		WriteUINT(pAINode->mMeshes[i]);
	META_DATA::Serialization(pAINode->mMetaData, hFile, dwByte);
}

void NODE::Deserialization(NODE& tNode, HANDLE hFile, DWORD& dwByte)
{
	ReadCHAR(tNode.m_Name);
	ReadFloat4x4(tNode.m_Transformation);
	NODE::Deserialization(*tNode.m_Parent, hFile, dwByte);
	ReadUINT(tNode.m_NumChildren);
	for (size_t i = 0; i < tNode.m_NumChildren; ++i)
		NODE::Deserialization(tNode.m_Children[i], hFile, dwByte);
	ReadUINT(tNode.m_NumMeshes);
	for (size_t i = 0; i < tNode.m_NumMeshes; ++i)
		MESH::Serialization()
	META_DATA::Serialization(pAINode->mMetaData, hFile, dwByte);
}