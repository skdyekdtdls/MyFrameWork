// AssimpPaser.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
//
#include "Assimp_Defines.h"

char pModelFilePath[MAX_PATH];
SCENE g_tScene;
Assimp::Importer g_Importer;
const aiScene* g_pAIScene = { nullptr };

int main()
{
    _uint			iFlag = 0;    

    //TYPE eModelType = TYPE_NONANIM;

    //if (TYPE_NONANIM == eModelType)
    //    iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
    //else
    //    iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
    
    g_pAIScene = g_Importer.ReadFile(ToModelsDir().append("Fiona\\Fiona.fbx").string().c_str(), iFlag);

    g_tScene.m_iNumMaterials = g_pAIScene->mNumMaterials;

    for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
    {
        aiString	StrPath;
    }
}



HRESULT Ready_Meshes(TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModeL)
{
    g_tScene.m_iNumMeshes = g_pAIScene->mNumMeshes;

    for (_uint i = 0; i < g_tScene.m_iNumMeshes; ++i)
    {
        
    }

    return S_OK;
}
