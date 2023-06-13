#include "..\public\MaterialPaser.h"

void CMaterialPaser::Pasing(HANDLE hFile, DWORD& dwByte, const aiScene* pAIScene, const char* pModelFilePath)
{
	WriteUINT(pAIScene->mNumMaterials);

	for (size_t i = 0; i < pAIScene->mNumMaterials; ++i)
	{
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString	strPath;

			if (FAILED(pAIScene->mMaterials[i]->GetTexture(aiTextureType(j), 0, &strPath)))
			{
				WriteUINT(0); // 데이터가 있는지 없는지 검사하는 용도로 임시로 넣어줌.
				continue;
			}
			else
				WriteUINT(1); // 데이터가 있는지 없는지 검사하는 용도로 임시로 넣어줌.

			char		szDrive[MAX_PATH] = "";
			char		szDirectory[MAX_PATH] = "";
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			char		szFileName[MAX_PATH] = "";
			char		szExt[MAX_PATH] = "";
			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			char		szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			WriteCHAR(szFullPath);
		}
	}
}
