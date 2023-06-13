#include "..\public\BonePaser.h"

HRESULT CBonePaser::Pasing(HANDLE hFile, DWORD& dwByte, aiNode* pNode)
{
	WriteCHAR(pNode->mName.data);
	m_BoneNames.push_back(pNode->mName.data);
	WriteAiMatrix4x4(pNode->mTransformation.Transpose());

	for (size_t i = 0; i < pNode->mNumChildren; ++i)
	{
		Pasing(hFile, dwByte, pNode->mChildren[i]);
	}

	return S_OK;
}