// AssimpPaser.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//
#include "Paser.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

fs::path ModelsPath = "C:\\KillSquad\\Game\\";
fs::path Static_Path = "../../Resources/Static_Mesh/";
fs::path Skeletal_Path = "../../Resources/Skeletal_Mesh/";
int FBXFileCount(fs::path currentPath);
int DATFileCount(fs::path currentPath);
void RenameFromModels(fs::path FolderName, CModel::TYPE eAnimType);

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	CPaser Paser;

	fs::recursive_directory_iterator ModelPathIter(ModelsPath);

	for (auto& ModelPathEntry : ModelPathIter)
	{
		if (ModelPathEntry.is_regular_file())
		{
			std::string extension = ModelPathEntry.path().extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

			if (extension == ".fbx") {
				Paser.Pasing(ModelPathEntry.path());
			}
		}
	}

	cout << "All Completed swithcing .fbx to .dat" << endl;
}

int FBXFileCount(fs::path currentPath)
{
	fs::directory_iterator iter(currentPath);
	int iCount = 0;
	while (iter != fs::end(iter)) {
		if (".fbx" == (*iter).path().extension() || ".FBX" == (*iter).path().extension())
		{
			iCount++;
		}

		++iter;
	}
	return iCount;
}

int DATFileCount(fs::path currentPath)
{
	fs::directory_iterator iter(currentPath);
	int iCount = 0;
	while (iter != fs::end(iter)) {
		if (".dat" == (*iter).path().extension() || ".DAT" == (*iter).path().extension())
		{
			iCount++;
		}

		++iter;
	}
	return iCount;
}


