#pragma once

#include "Assimp_Defines.h"
enum class ANIM_TYPE { NONANIM, ANIM, ANIM_END };
using namespace DirectX;

typedef class SCENE;
typedef class NODE;
typedef class FACE;
typedef class MESH;
typedef class MATERIAL;
typedef class MATERIAL_PROPERTY;
typedef class ANIMATION;
typedef class NODE_ANIM;
typedef class VECTOR_KEY;
typedef class QUAT_KEY;
typedef class MESH_ANIM;
typedef class MESH_KEY;
typedef class MESH_MORPH_ANIM;
typedef class MESH_MORPH_KEY;
typedef class BONE;
typedef class VERTEX_WEIGHT;
typedef class ANIM_MESH;
typedef class AABB;
typedef class TEXTURE;
typedef class TEXEL;
typedef class LIGHT;
typedef class CAMERA;
typedef class SKELETON;
typedef class SKELETON_BONE;
typedef class META_DATA;
typedef class META_DATA_ENTRY;
typedef enum E_MORPHING_METHOD;
typedef enum E_PROPERTY_TYPE_INFO;
typedef enum E_ANIM_BEHAVIOR;
typedef enum E_LIGHT_SOURCE_TYPE;
typedef enum E_META_DATA_TYPE;

enum E_MORPHING_METHOD
{
	MorphingMethod_UNKNOWN = 0x0,
	MorphingMethod_VERTEX_BLEND = 0x1,
	MorphingMethod_MORPH_NORMALIZED = 0x2,
	MorphingMethod_MORPH_RELATIVE = 0x3,
#ifndef SWIG
	_MorphingMethod_Force32Bit = INT_MAX
#endif
};

enum E_PROPERTY_TYPE_INFO {
	PTI_Float = 0x1,
	PTI_Double = 0x2,
	PTI_String = 0x3,
	PTI_Integer = 0x4,
	PTI_Buffer = 0x5,
#ifndef SWIG
	_PTI_Force32Bit = INT_MAX
#endif
};

enum E_ANIM_BEHAVIOR
{
	AnimBehaviour_DEFAULT = 0x0,
	AnimBehaviour_CONSTANT = 0x1,
	AnimBehaviour_LINEAR = 0x2,
	AnimBehaviour_REPEAT = 0x3,
#ifndef SWIG
	_AnimBehaviour_Force32Bit = INT_MAX
#endif
};

enum E_LIGHT_SOURCE_TYPE
{
	LightSource_UNDEFINED = 0x0,
	LightSource_DIRECTIONAL = 0x1,
	LightSource_POINT = 0x2,
	LightSource_SPOT = 0x3,
	LightSource_AMBIENT = 0x4,
	LightSource_AREA = 0x5,
#ifndef SWIG
	_LightSource_Force32Bit = INT_MAX
#endif
};

enum E_META_DATA_TYPE {
	_BOOL = 0,
	_INT32 = 1,
	_UINT64 = 2,
	_FLOAT = 3,
	_DOUBLE = 4,
	_AISTRING = 5,
	_AIVECTOR3D = 6,
	_AIMETADATA = 7,
	_INT64 = 8,
	_UINT32 = 9,
	_META_MAX = 10,

#ifndef SWIG
	_FORCE_32BIT = INT_MAX
#endif
};

class AI_STRING
{
public:
	~AI_STRING();
	static void Serialization(const aiString* pAIString, HANDLE hFile, DWORD& dwByte);
	void Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	char m_data[1024Ui64];
};

class META_DATA_ENTRY
{
public:
	~META_DATA_ENTRY();
	static void Serialization(aiMetadataEntry* pAIMetadataEntry, HANDLE hFile, DWORD& dwByte);
	void Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	E_META_DATA_TYPE m_eType;
	void* m_Data = { nullptr };
};

class META_DATA
{
public:
	~META_DATA();
	static void Serialization(aiMetadata* pAIMetadata, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(META_DATA* tMetaData, HANDLE hFile, DWORD& dwByte);

public:
	/* Length of the mKeys and mValues arrays, respectively */
	unsigned int m_NumProperties;
	
	AI_STRING* m_Keys = { nullptr };
	META_DATA_ENTRY* m_Values = { nullptr };
};
/// <summary>
/// ¿Ï·á
/// </summary>
class NODE
{
public:
	~NODE();
	static void Serialization(aiNode* pAINode, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(NODE* tNode, HANDLE hFile, DWORD& dwByte);

private:
	AI_STRING       m_Name;
	XMFLOAT4X4		m_Transformation;
	unsigned int    m_NumChildren = { 0 };
	NODE*			m_Children = { nullptr };
	unsigned int	m_NumMeshes = { 0 };
	unsigned int*	m_Meshes = { nullptr };
	META_DATA*		m_MetaData = { nullptr };
};

class FACE
{
public:
	~FACE();
	static void Serialization(aiFace* pAIFace, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(FACE* tFace, HANDLE hFile, DWORD& dwByte);

	unsigned int  m_NumIndices = { 0 };
	unsigned int* m_Indices = { nullptr };
};

class AABB
{
public:
	~AABB();
	static void Serialization(aiAABB* pAIAABB, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(AABB* tAABB, HANDLE hFile, DWORD& dwByte);

	XMFLOAT3 m_Min;
	XMFLOAT3 m_Max;
};

class MESH
{
public:
	~MESH();
	static void Serialization(aiMesh* pAIMesh, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MESH* tMesh, HANDLE hFile, DWORD& dwByte);

private:
	unsigned int        m_PrimitiveTypes;
	unsigned int        m_NumVertices;
	unsigned int        m_NumFaces;
	XMFLOAT3*			m_Vertices;
	XMFLOAT3*			m_Normals;
	XMFLOAT3*			m_Tangents;
	XMFLOAT3*			m_Bitangents;
	XMFLOAT4*			m_Colors[AI_MAX_NUMBER_OF_COLOR_SETS];
	XMFLOAT3*			m_TextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	unsigned int        m_NumUVComponents[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	FACE*				m_Faces;
	unsigned int        m_NumBones;
	BONE**				m_Bones = { nullptr };
	unsigned int		m_MaterialIndex;
	AI_STRING			m_Name;
	unsigned int		m_NumAnimMeshes;
	ANIM_MESH**			m_AnimMeshes;
	E_MORPHING_METHOD	m_eMethod;
	AABB				m_AABB;
	AI_STRING*			m_TextureCoordsNames;
};

class MATERIAL
{
public:
	~MATERIAL();
	static void Serialization(aiMaterial* pAIMaterial, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MATERIAL* tMaterial, HANDLE hFile, DWORD& dwByte);

	unsigned int m_NumProperties;
	unsigned int m_NumAllocated;
	MATERIAL_PROPERTY** m_Properties;
};



class MATERIAL_PROPERTY
{
public:
	~MATERIAL_PROPERTY();
	static void Serialization(aiMaterialProperty* pAIMaterialProperty, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MATERIAL_PROPERTY* tMaterialProperty, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Key;
	unsigned int m_Semantic;
	unsigned int m_Index;
	unsigned int m_DataLength;
	E_PROPERTY_TYPE_INFO m_eType;
	char* m_Data;
};

class ANIMATION
{
public:
	~ANIMATION();
	static void Serialization(aiAnimation* pAIAnimation, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(ANIMATION* tAnimation, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	double m_Duration;
	double m_TicksPerSecond;
	unsigned int m_NumChannels;
	NODE_ANIM** m_Channels;
	unsigned int m_NumMeshChannels;
	MESH_ANIM** m_MeshChannels;
	unsigned int m_NumMorphMeshChannels;
	MESH_MORPH_ANIM** m_MorphMeshChannels;
};



class NODE_ANIM
{
public:
	~NODE_ANIM();
	static void Serialization(aiNodeAnim* pAINodeAnim, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(NODE_ANIM* tNodeAnim, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_NodeName;
	unsigned int m_NumPositionKeys;
	VECTOR_KEY* m_PositionKeys;
	unsigned int m_NumRotationKeys;
	QUAT_KEY* m_RotationKeys;
	unsigned int m_NumScalingKeys;
	VECTOR_KEY* m_ScalingKeys;
	E_ANIM_BEHAVIOR m_ePreState;
	E_ANIM_BEHAVIOR m_ePostState;
};

class VECTOR_KEY
{
public:
	~VECTOR_KEY();
	static void Serialization(aiVectorKey* pAIVectorKey, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(VECTOR_KEY* tVectorKey, HANDLE hFile, DWORD& dwByte);

	double m_Time;
	XMFLOAT3 m_Value;
};

class QUAT_KEY
{
public:
	~QUAT_KEY();
	static void Serialization(aiQuatKey* pAIQuatKey, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(QUAT_KEY* tQuatKey, HANDLE hFile, DWORD& dwByte);

	double m_Time;
	XMFLOAT4 m_Value;
};

class MESH_ANIM
{
public:
	~MESH_ANIM();
	static void Serialization(aiMeshAnim* pAIMeshAnim, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MESH_ANIM* tMeshAnim, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumKeys = { 0 };
	MESH_KEY* m_Keys;
};

class MESH_KEY
{
public:
	~MESH_KEY();
	static void Serialization(aiMeshKey* pAIMeshKey, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MESH_KEY* tMeshKey, HANDLE hFile, DWORD& dwByte);

	double m_Time;
	unsigned int m_Value;
};

class MESH_MORPH_KEY
{
public:
	~MESH_MORPH_KEY();
	static void Serialization(aiMeshMorphKey* pAIMeshMorphKey, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MESH_MORPH_KEY* tMeshMorphKey, HANDLE hFile, DWORD& dwByte);

	double m_Time;
	unsigned int m_NumValuesAndWeights;
	unsigned int* m_Values;
	double* m_Weights;
};

class MESH_MORPH_ANIM
{
public:
	~MESH_MORPH_ANIM();
	static void Serialization(aiMeshMorphAnim* pAIMeshMorphAnim, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(MESH_MORPH_ANIM* tMeshMorphAnim, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumKeys;
	MESH_MORPH_KEY* m_Keys;
};



class BONE
{
public:
	~BONE();
	static void Serialization(aiBone* pAIBone, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(BONE* tBone, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumWeights;
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE* m_Armature;
	NODE* m_Node;
#endif
	VERTEX_WEIGHT* m_Weights;
	XMFLOAT4X4 m_OffsetMatrix;
};

class VERTEX_WEIGHT
{
public:
	~VERTEX_WEIGHT();
	static void Serialization(aiVertexWeight* pAIVertexWeight, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(VERTEX_WEIGHT* tVertexWeight, HANDLE hFile, DWORD& dwByte);

	unsigned int m_VertexId;
	float m_Weight;
};

class ANIM_MESH
{
public:
	~ANIM_MESH();
	static void Serialization(aiAnimMesh* pAIAnimMesh, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(ANIM_MESH* tAnimMesh, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumVertices;
	XMFLOAT3* m_Vertices;
	XMFLOAT3* m_Normals;
	XMFLOAT3* m_Tangents;
	XMFLOAT3* m_Bitangents;
	XMFLOAT4* m_Colors[AI_MAX_NUMBER_OF_COLOR_SETS];
	XMFLOAT3* m_TextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	float m_Weight;
};

class TEXTURE
{
public:
	~TEXTURE();
	static void Serialization(aiTexture* pAITexture, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(TEXTURE* tTexture, HANDLE hFile, DWORD& dwByte);

	unsigned int m_Width;
	unsigned int m_Height;
	char m_achFormatHint[HINTMAXTEXTURELEN];
	TEXEL* m_pcData;
	AI_STRING m_Filename;
};

class TEXEL
{
public:
	~TEXEL();
	static void Serialization(aiTexel* pAITexel, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(TEXEL* tTexel, HANDLE hFile, DWORD& dwByte);

	unsigned char b, g, r, a;
};

class LIGHT
{
public:
	~LIGHT();
	static void Serialization(aiLight* pAILight, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(LIGHT* tLight, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	E_LIGHT_SOURCE_TYPE m_eType;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Up;
	float m_AttenuationConstant;
	float m_AttenuationLinear;
	float m_AttenuationQuadratic;
	XMFLOAT3 m_ColorDiffuse;
	XMFLOAT3 m_ColorSpecular;
	XMFLOAT3 m_ColorAmbient;
	float m_AngleInnerCone;
	float m_AngleOuterCone;
	XMFLOAT2 m_Size;
};

class CAMERA
{
public:
	~CAMERA();
	static void Serialization(aiCamera* pAICamera, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(CAMERA* tCamera, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_LookAt;
	float m_HorizontalFOV;
	float m_ClipPlaneNear;
	float m_ClipPlaneFar;
	float m_Aspect;
	float m_OrthographicWidth;
};

class SKELETON_BONE
{
public:
	~SKELETON_BONE();
	static void Serialization(aiSkeletonBone* pAISkeletonBone, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(SKELETON_BONE* tSkeletonBone, HANDLE hFile, DWORD& dwByte);

	int m_Parent;
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE* m_Armature;
	NODE* m_Node;
#endif
	unsigned int m_NumnWeights;
	MESH* m_MeshId;
	VERTEX_WEIGHT* m_Weights;
	XMFLOAT4X4 m_OffsetMatrix;
	XMFLOAT4X4 m_LocalMatrix;
};

class SKELETON
{
public:
	~SKELETON();
	static void Serialization(aiSkeleton* pAISkeleton, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(SKELETON* tSkeleton, HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumBones;
	SKELETON_BONE** m_Bones;
};


class SCENE
{
public:
	~SCENE();
	static void Serialization(const aiScene* pAIScene, HANDLE hFile, DWORD& dwByte);
	static void Deserialization(SCENE* tScene, HANDLE hFile, DWORD& dwByte);

	unsigned int		m_Flags = { 0 };
	NODE*				m_RootNode;
	unsigned int		m_NumMeshes = { 0 };
	MESH**				m_Meshes = { nullptr };
	unsigned int		m_NumMaterials = { 0 };
	MATERIAL**			m_Materials = { nullptr };
	unsigned int		m_NumAnimations = { 0 };
	ANIMATION**			m_Animations;
	unsigned int		m_NumTextures = { 0 };
	TEXTURE**			m_Textures = { nullptr };
	unsigned int		m_NumLights = { 0 };
	LIGHT**				m_Lights = { nullptr };
	unsigned int		m_NumCameras = { 0 };
	CAMERA**			m_Cameras = { nullptr };
	META_DATA*			m_MetaData;
	AI_STRING			m_Name;
	unsigned int		m_NumSkeletons = { 0 };
	SKELETON**			m_Skeletons = { nullptr };
};