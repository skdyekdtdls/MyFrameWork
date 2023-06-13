#pragma once
#include "Engine_Defines.h"
enum class ANIM_TYPE { NONANIM, ANIM, a_END };

typedef struct tagModel
{
    ANIM_TYPE eAnimType;
    _uint iNumMeshes;
    _uint iNumMaterials;
    _uint iNumAnimation;
}MODEL;

typedef struct tagNode
{
    char szName;

}NODE;

typedef struct tagMesh
{
    char					szName[MAX_PATH] = "";
    _uint					iMaterialIndex = { 0 };
    _uint					iNumBones = { 0 };
    _uint                   iNumVertices;
    _uint                   iNumIndices;
}MESH;

typedef struct tagBone
{

}BONE;