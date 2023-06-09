#pragma once


typedef struct tagScene
{
    _uint m_iNumMeshes;
    _uint m_iNumMaterials;
}SCENE;

typedef struct tagMesh
{
    char					m_szName[MAX_PATH] = "";
    _uint					m_iMaterialIndex = { 0 };
    _uint					m_iNumBones = { 0 };
    vector<_uint>			m_Bones;
    _uint                   m_iNumVertices;
    _uint                   m_iNumIndices;
}MESH;