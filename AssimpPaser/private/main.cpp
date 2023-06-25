// AssimpPaser.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//
#include "Paser.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

fs::path ModelsPath = "../../Resources/Models/";
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
    
    int a = 1;
    std::cout << "Select Mode[ 1(All), 2(One) ] : ";
    std:cin >> a;

    CModel::TYPE eAnimType = { CModel::TYPE_END };
    switch (a)
    {
    case 1:
    {
        fs::directory_iterator ModelPathIter(ModelsPath);
        int iFBXCount = 0;
        int iDATCount = 0;
        std::string folderName;
        while (ModelPathIter != fs::end(ModelPathIter))
        {
            const fs::directory_entry& ModelPathEntry = *ModelPathIter;
            fs::directory_iterator CurrentPathIter(ModelPathEntry);

            while (CurrentPathIter != fs::end(CurrentPathIter))
            {
                const fs::directory_entry& CurrentPathEntry = *CurrentPathIter;
                if (".fbx" == CurrentPathEntry.path().extension() || ".FBX" == CurrentPathEntry.path().extension())
                {
                    Paser.Pasing(CurrentPathEntry.path(), eAnimType);
                }

                ++CurrentPathIter;
            }

            folderName = ModelPathEntry.path().stem().string();

            RenameFromModels(folderName, eAnimType);

            ++ModelPathIter;
        }
    }
    break;
    case 2:
    {
        std::cout << "Input Directory Name : ";
        std::string folderName;
        std::cin.ignore();
        std::getline(std::cin, folderName);
        fs::path ModlePathEntry = ModelsPath / folderName;

        fs::directory_iterator CurrentPathIter(ModlePathEntry);
        while (CurrentPathIter != fs::end(CurrentPathIter))
        {
            const fs::directory_entry& CurrentPathEntry = *CurrentPathIter;
            if (".fbx" == CurrentPathEntry.path().extension() || ".FBX" == CurrentPathEntry.path().extension())
            {
                Paser.Pasing(CurrentPathEntry.path(), eAnimType);
            }

            ++CurrentPathIter;
        }
        RenameFromModels(folderName, eAnimType);
    }
        break;
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

void RenameFromModels(fs::path FolderName, CModel::TYPE eAnimType)
{
	fs::path destination;
	fs::path source = ModelsPath / FolderName;

	if (CModel::TYPE_NONANIM == eAnimType)
		destination = Static_Path / FolderName;
	else
		destination = Skeletal_Path / FolderName;

    if (fs::exists(destination)) {
        fs::remove_all(destination);
    }

    fs::rename(source, destination);
}
