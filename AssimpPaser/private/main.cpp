// AssimpPaser.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//
#include "Paser.h"
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    CPaser Paser;
    Paser.Pasing("../../Resources/Models/Fiona/Fiona.fbx");
}