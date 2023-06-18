#pragma once
#include "Engine_Defines.h"
//#include "Assimp_Defines.h"

BEGIN(Engine)

using namespace DirectX;

typedef class COLOR_RGBA_FLOAT;
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

enum TextureType {
	TextureType_NONE = 0,
	TextureType_DIFFUSE = 1,
	TextureType_SPECULAR = 2,
	TextureType_AMBIENT = 3,
	TextureType_EMISSIVE = 4,
	TextureType_HEIGHT = 5,
	TextureType_NORMALS = 6,
	TextureType_SHININESS = 7,
	TextureType_OPACITY = 8,
	TextureType_DISPLACEMENT = 9,
	TextureType_LIGHTMAP = 10,
	TextureType_REFLECTION = 11,
	TextureType_BASE_COLOR = 12,
	TextureType_NORMAL_CAMERA = 13,
	TextureType_EMISSION_COLOR = 14,
	TextureType_METALNESS = 15,
	TextureType_DIFFUSE_ROUGHNESS = 16,
	TextureType_AMBIENT_OCCLUSION = 17,
	TextureType_SHEEN = 19,
	TextureType_CLEARCOAT = 20,
	TextureType_TRANSMISSION = 21,
	TextureType_UNKNOWN = 18,
#ifndef SWIG
	_TextureType_Force32Bit = INT_MAX
#endif
};
#define TEXTURE_TYPE_MAX TextureType_TRANSMISSION


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

class COLOR_RGBA_FLOAT // Test Complete
{
public:
	COLOR_RGBA_FLOAT();
	~COLOR_RGBA_FLOAT();
	static void Serialization(const aiColor4D* pAIString, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	float r = { 0.f };
	float g = { 0.f };
	float b = { 0.f };
	float a = { 0.f };
};

class AI_STRING // Test Complete
{
public:
	AI_STRING();
	~AI_STRING();
	static void Serialization(const aiString* pAIString, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	char m_data[1024];
};

class META_DATA_ENTRY // Test Complete
{
public:
	META_DATA_ENTRY();
	~META_DATA_ENTRY();
	static void Serialization(aiMetadataEntry* pAIMetadataEntry, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	E_META_DATA_TYPE m_eType;
	void* m_Data = { nullptr };
};

class META_DATA // Test Complete
{
public:
	META_DATA();
	~META_DATA();
	static void Serialization(aiMetadata* pAIMetadata, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

public:
	/* Length of the mKeys and mValues arrays, respectively */
	unsigned int m_NumProperties;
	
	AI_STRING* m_Keys = { nullptr };
	META_DATA_ENTRY* m_Values = { nullptr };
};

class NODE // Test Complete
{
public:
	NODE();
	~NODE();
	static void Serialization(aiNode* pAINode, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING       m_Name;
	XMFLOAT4X4		m_Transformation;
	unsigned int    m_NumChildren = { 0 };
	unsigned int	m_NumMeshes = { 0 };
	unsigned int*	m_Meshes = { nullptr };
	META_DATA*		m_MetaData = { nullptr };
	NODE*			m_Children = { nullptr };
};

class FACE // Test Complete
{
public:
	FACE();
	~FACE();
	static void Serialization(aiFace* pAIFace, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned int  m_NumIndices = { 0 };
	unsigned int* m_Indices = { nullptr };
};

class AABB // Test Complete
{
public:
	AABB();
	~AABB();
	static void Serialization(aiAABB* pAIAABB, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	XMFLOAT3 m_Min;
	XMFLOAT3 m_Max;
};

class MESH  // Test Complete
{
public:
	MESH();
	~MESH();
	static void Serialization(aiMesh* pAIMesh, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned int        m_PrimitiveTypes = 0;
	unsigned int        m_NumVertices = 0;
	unsigned int        m_NumFaces = 0;
	XMFLOAT3*			m_Vertices = nullptr;
	XMFLOAT3*			m_Normals = nullptr;
	XMFLOAT3*			m_Tangents = nullptr;
	XMFLOAT3*			m_Bitangents = nullptr;
	COLOR_RGBA_FLOAT*	m_Colors[AI_MAX_NUMBER_OF_COLOR_SETS];
	XMFLOAT3*			m_TextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	unsigned int        m_NumUVComponents[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	FACE*				m_Faces = nullptr;
	unsigned int        m_NumBones = 0;
	BONE*				m_Bones = { nullptr };
	unsigned int		m_MaterialIndex = 0;
	AI_STRING			m_Name;
	unsigned int		m_NumAnimMeshes = 0;
	ANIM_MESH*			m_AnimMeshes = { nullptr };
	E_MORPHING_METHOD	m_eMethod;
	AABB				m_AABB;
	AI_STRING*			m_TextureCoordsNames[AI_MAX_NUMBER_OF_TEXTURECOORDS];
};

class MATERIAL // Test Complete
{
public:
	MATERIAL();
	~MATERIAL();
	bool GetTexture(TextureType type, unsigned int index, AI_STRING* path);
	static void Serialization(aiMaterial* pAIMaterial, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING*	m_TexturePath[TextureType_TRANSMISSION];
	unsigned int m_NumProperties = 0;
	unsigned int m_NumAllocated = 0;
	MATERIAL_PROPERTY* m_Properties = { nullptr };
};



class MATERIAL_PROPERTY // Test Complete
{
public:
	MATERIAL_PROPERTY();
	~MATERIAL_PROPERTY();
	static void Serialization(aiMaterialProperty* pAIMaterialProperty, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Key;
	unsigned int m_Semantic = 0;
	unsigned int m_Index = 0;
	unsigned int m_DataLength = 0;
	E_PROPERTY_TYPE_INFO m_eType;
	char* m_Data = nullptr;
};

class ANIMATION // Test Complete
{
public:
	ANIMATION();
	~ANIMATION();
	static void Serialization(aiAnimation* pAIAnimation, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	double m_Duration = 0.0;
	double m_TicksPerSecond = 0.0;
	unsigned int m_NumChannels = 0;
	NODE_ANIM* m_Channels = nullptr;
	unsigned int m_NumMeshChannels = 0;
	MESH_ANIM* m_MeshChannels = nullptr;
	unsigned int m_NumMorphMeshChannels = 0;
	MESH_MORPH_ANIM* m_MorphMeshChannels = nullptr;
};

class NODE_ANIM // Test Complete
{
public:
	NODE_ANIM();
	~NODE_ANIM();
	static void Serialization(aiNodeAnim* pAINodeAnim, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_NodeName;
	unsigned int m_NumPositionKeys = 0;
	VECTOR_KEY* m_PositionKeys = { nullptr };
	unsigned int m_NumRotationKeys = 0;
	QUAT_KEY* m_RotationKeys = nullptr;
	unsigned int m_NumScalingKeys = 0;
	VECTOR_KEY* m_ScalingKeys = nullptr;
	E_ANIM_BEHAVIOR m_ePreState;
	E_ANIM_BEHAVIOR m_ePostState;
};

class VECTOR_KEY // Test Complete
{
public:
	VECTOR_KEY();
	~VECTOR_KEY();
	static void Serialization(aiVectorKey* pAIVectorKey, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	double m_Time = 0.0;
	XMFLOAT3 m_Value;
};

typedef struct QUATERNION
{
	float w, x, y, z;
};

class QUAT_KEY // Test Complete
{
public:
	QUAT_KEY();
	~QUAT_KEY();
	static void Serialization(aiQuatKey* pAIQuatKey, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	double m_Time = 0.0;
	QUATERNION m_Value;
};

class MESH_ANIM // Test Complete
{
public:
	MESH_ANIM();
	~MESH_ANIM();
	static void Serialization(aiMeshAnim* pAIMeshAnim, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumKeys = { 0 };
	MESH_KEY* m_Keys;
};

class MESH_KEY // Test Complete
{
public:
	MESH_KEY();
	~MESH_KEY();
	static void Serialization(aiMeshKey* pAIMeshKey, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	double m_Time = 0.0;
	unsigned int m_Value = 0;
};

class MESH_MORPH_KEY // Test Complete
{
public:
	MESH_MORPH_KEY();
	~MESH_MORPH_KEY();
	static void Serialization(aiMeshMorphKey* pAIMeshMorphKey, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	double m_Time = 0.0;
	unsigned int m_NumValuesAndWeights = 0;
	unsigned int* m_Values = nullptr;
	double* m_Weights = nullptr;
};

class MESH_MORPH_ANIM // Test Complete
{
public:
	MESH_MORPH_ANIM();
	~MESH_MORPH_ANIM();
	static void Serialization(aiMeshMorphAnim* pAIMeshMorphAnim, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumKeys = 0;
	MESH_MORPH_KEY* m_Keys = { nullptr };
};

class BONE // Test Complete
{
public:
	BONE();
	~BONE();
	static void Serialization(aiBone* pAIBone, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumWeights = 0;
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE* m_Armature = nullptr;
	NODE* m_Node = nullptr;
#endif
	VERTEX_WEIGHT* m_Weights = nullptr;
	XMFLOAT4X4 m_OffsetMatrix;
};

class VERTEX_WEIGHT // Test Complete
{
public:
	VERTEX_WEIGHT();
	~VERTEX_WEIGHT();
	static void Serialization(aiVertexWeight* pAIVertexWeight, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned int m_VertexId = 0;
	float m_Weight = 0.f;
};

class ANIM_MESH // Test Complete
{
public:
	ANIM_MESH();
	~ANIM_MESH();
	static void Serialization(aiAnimMesh* pAIAnimMesh, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumVertices = 0;
	XMFLOAT3* m_Vertices = nullptr;
	XMFLOAT3* m_Normals = nullptr;
	XMFLOAT3* m_Tangents = nullptr;
	XMFLOAT3* m_Bitangents = nullptr;
	XMFLOAT4* m_Colors[AI_MAX_NUMBER_OF_COLOR_SETS];
	XMFLOAT3* m_TextureCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
	float m_Weight = 0.f;
};

class TEXTURE // Test Complete
{
public:
	TEXTURE();
	~TEXTURE();
	static void Serialization(aiTexture* pAITexture, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned int m_Width = 0;
	unsigned int m_Height = 0;
	char m_achFormatHint[HINTMAXTEXTURELEN];
	TEXEL* m_pcData = nullptr;
	AI_STRING m_Filename;
};

class TEXEL // Test Complete
{
public:
	TEXEL();
	~TEXEL();
	static void Serialization(aiTexel* pAITexel, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned char b, g, r, a;
};

class LIGHT // Test Complete
{
public:
	LIGHT();
	~LIGHT();
	static void Serialization(aiLight* pAILight, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	E_LIGHT_SOURCE_TYPE m_eType;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Direction;
	XMFLOAT3 m_Up;
	float m_AttenuationConstant = 0.f;
	float m_AttenuationLinear = 0.f;
	float m_AttenuationQuadratic = 0.f;
	XMFLOAT3 m_ColorDiffuse;
	XMFLOAT3 m_ColorSpecular;
	XMFLOAT3 m_ColorAmbient;
	float m_AngleInnerCone = 0.f;
	float m_AngleOuterCone = 0.f;
	XMFLOAT2 m_Size;
};

class CAMERA // Test Complete
{
public:
	CAMERA();
	~CAMERA();
	static void Serialization(aiCamera* pAICamera, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_LookAt;
	float m_HorizontalFOV = 0.f;
	float m_ClipPlaneNear = 0.f;
	float m_ClipPlaneFar = 0.f;
	float m_Aspect = 0.f;
	float m_OrthographicWidth = 0.f;
};

class SKELETON_BONE // Test Complete
{
public:
	SKELETON_BONE();
	~SKELETON_BONE();
	static void Serialization(aiSkeletonBone* pAISkeletonBone, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	int m_Parent = 0;
#ifndef ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS
	NODE* m_Armature = nullptr;
	NODE* m_Node = nullptr;
#endif
	unsigned int m_NumnWeights = 0;
	MESH* m_MeshId = nullptr;
	VERTEX_WEIGHT* m_Weights = nullptr;
	XMFLOAT4X4 m_OffsetMatrix;
	XMFLOAT4X4 m_LocalMatrix;
};

class SKELETON // Test Complete
{
public:
	SKELETON();
	~SKELETON();
	static void Serialization(aiSkeleton* pAISkeleton, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	AI_STRING m_Name;
	unsigned int m_NumBones = 0;
	SKELETON_BONE* m_Bones;
};


class ENGINE_DLL SCENE
{
public:
	SCENE();
	~SCENE();
	static void Serialization(const aiScene* pAIScene, HANDLE hFile, DWORD& dwByte);
	bool Deserialization(HANDLE hFile, DWORD& dwByte);

	unsigned int		m_Flags = { 0 };
	NODE*				m_RootNode = { nullptr };
	unsigned int		m_NumMeshes = { 0 };
	MESH*				m_Meshes = { nullptr };
	unsigned int		m_NumMaterials = { 0 };
	MATERIAL*			m_Materials = { nullptr };
	unsigned int		m_NumAnimations = { 0 };
	ANIMATION*			m_Animations = { nullptr };
	unsigned int		m_NumTextures = { 0 };
	TEXTURE*			m_Textures = { nullptr };
	unsigned int		m_NumLights = { 0 };
	LIGHT*				m_Lights = { nullptr };
	unsigned int		m_NumCameras = { 0 };
	CAMERA*				m_Cameras = { nullptr };
	META_DATA*			m_MetaData = { nullptr };
	AI_STRING			m_Name;
	unsigned int		m_NumSkeletons = { 0 };
	SKELETON*			m_Skeletons = { nullptr };
};

END