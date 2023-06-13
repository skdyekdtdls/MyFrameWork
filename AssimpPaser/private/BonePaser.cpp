#include "..\public\BonePaser.h"

HRESULT CBonePaser::Pasing(HANDLE hFile, DWORD& dwByte, aiNode* pNode, class CBone* pParent)
{
	WriteCHAR(hFile, pNode->mName.data, dwByte);

	pNode->mChildren;
	pNode->mMeshes;
	pNode->mMetaData;
	pNode->mName;
	pNode->mNumChildren;
	pNode->mNumMeshes;
	pNode->mParent;
	pNode->mTransformation;

	return S_OK;
}