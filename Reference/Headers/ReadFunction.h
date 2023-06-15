#pragma once

namespace Engine
{
	static bool WriteEnable(HANDLE hFile, bool bValue, DWORD& dwByte)
	{
		return WriteFile(hFile, &bValue, sizeof(bool), &dwByte, nullptr);
	}
#define WriteEnable(bValue) Engine::WriteEnable(hFile, bValue, dwByte)

	static bool WriteTCHAR(HANDLE hFile, const wchar_t* pBuffer, DWORD& dwByte)
	{
		return WriteFile(hFile, pBuffer, sizeof(wchar_t) * MAX_PATH * 2, &dwByte, nullptr);
	}
#define WriteTCHAR(pBuffer) Engine::WriteTCHAR(hFile, pBuffer, dwByte)

	// char*
	static bool WriteCHAR(HANDLE hFile, const char* pBuffer, DWORD& dwByte)
	{
		char Buffer[MAX_PATH];
		wchar_t LBuffer[MAX_PATH];
		strcpy_s(Buffer, pBuffer);
		MultiByteToWideChar(CP_ACP, 0, Buffer, MAX_PATH, LBuffer, MAX_PATH);
		return WriteFile(hFile, LBuffer, sizeof(wchar_t) * MAX_PATH, &dwByte, nullptr);
	}
#define WriteCHAR(pBuffer) Engine::WriteCHAR(hFile, pBuffer, dwByte)

	// unsigned int
	static bool WriteUINT(HANDLE hFile, unsigned int iBuffer, DWORD& dwByte)
	{
		return WriteFile(hFile, &iBuffer, sizeof(unsigned int), &dwByte, nullptr);
	}
#define WriteUINT(iBuffer) Engine::WriteUINT(hFile, iBuffer, dwByte)

	// float
	static bool WriteFloat(HANDLE hFile, float pVBuffer, DWORD& dwByte)
	{
		return WriteFile(hFile, &pVBuffer, sizeof(float), &dwByte, nullptr);
	}
#define WriteFloat(arg) Engine::WriteFloat(hFile, arg, dwByte)

	// float3
	static bool WriteFloat3(HANDLE hFile, _float3 pVBuffer, DWORD& dwByte)
	{
		return WriteFile(hFile, &pVBuffer, sizeof(_float3), &dwByte, nullptr);
	}
#define WriteFloat3(arg) Engine::WriteFloat3(hFile, arg, dwByte)

	// XMFLOAT4X4
	static bool WriteFloat4x4(HANDLE hFile, _float4x4 MatrixBuffer, DWORD& dwByte)
	{
		return WriteFile(hFile, &MatrixBuffer, sizeof(XMFLOAT4X4), &dwByte, nullptr);
	}
#define WriteFloat4x4(arg) Engine::WriteFloat4x4(hFile, arg, dwByte)



	// void
	static bool WriteVoid(HANDLE hFile, void* pBuffer, _uint iSize, DWORD& dwByte)
	{
		return WriteFile(hFile, pBuffer, iSize, &dwByte, nullptr);
	}
#define WriteVoid(arg, size) Engine::WriteVoid(hFile, arg, size, dwByte)
	///////////////////////////////////////////////////////////

	static bool ReadEnable(HANDLE hFile, DWORD& dwByte)
	{
		bool bValue = { false };
		ReadFile(hFile, &bValue, sizeof(bool), &dwByte, nullptr);
		return bValue;
	}
#define ReadEnable() Engine::ReadEnable(hFile, dwByte)

	static bool ReadCHAR(HANDLE hFile, char* pBuffer, DWORD& dwByte)
	{
		_tchar wszBuffer[MAX_PATH];
		bool bResult = ReadFile(hFile, wszBuffer, sizeof(_tchar) * MAX_PATH, &dwByte, nullptr);
		WideCharToMultiByte(CP_ACP, 0, wszBuffer, MAX_PATH, pBuffer, MAX_PATH, NULL, NULL);
		return bResult;
	}
#define ReadCHAR(pBuffer) Engine::ReadCHAR(hFile, pBuffer, dwByte)

	static bool ReadUINT(HANDLE hFile, _uint& iBuffer, DWORD& dwByte)
	{
		return ReadFile(hFile, &iBuffer, sizeof(_uint), &dwByte, nullptr);
	}
#define ReadUINT(iBuffer) Engine::ReadUINT(hFile, iBuffer, dwByte)

	static bool ReadFloat(HANDLE hFile, _float& pVBuffer, DWORD& dwByte)
	{
		return ReadFile(hFile, &pVBuffer, sizeof(_float), &dwByte, nullptr);
	}
#define ReadFloat(arg) Engine::ReadFloat(hFile, arg, dwByte)

	static bool ReadFloat2(HANDLE hFile, _float2& pVBuffer, DWORD& dwByte)
	{
		return ReadFile(hFile, &pVBuffer, sizeof(_float2), &dwByte, nullptr);
	}
#define ReadFloat2(arg) Engine::ReadFloat2(hFile, arg, dwByte)

	static bool ReadFloat3(HANDLE hFile, _float3& pVBuffer, DWORD& dwByte)
	{
		return ReadFile(hFile, &pVBuffer, sizeof(_float3), &dwByte, nullptr);
	}
#define ReadFloat3(arg) Engine::ReadFloat3(hFile, arg, dwByte)

	static bool ReadFloat4x4(HANDLE hFile, _float4x4& MatrixBuffer, DWORD& dwByte)
	{
		return ReadFile(hFile, &MatrixBuffer, sizeof(_float4x4), &dwByte, nullptr);
	}
#define ReadFloat4x4(arg) Engine::ReadFloat4x4(hFile, arg, dwByte)

	// void
	static bool ReadVoid(HANDLE hFile, void* pBuffer, _uint iSize, DWORD& dwByte)
	{
		return ReadFile(hFile, pBuffer, iSize, &dwByte, nullptr);
	}
#define ReadVoid(arg, size) Engine::ReadVoid(hFile, arg, size, dwByte)
	enum TextureType {
		/** Dummy value.
		 *
		 *  No texture, but the value to be used as 'texture semantic'
		 *  (#aiMaterialProperty::mSemantic) for all material properties
		 *  *not* related to textures.
		 */
		TextureType_NONE = 0,

		/** LEGACY API MATERIALS
		 * Legacy refers to materials which
		 * Were originally implemented in the specifications around 2000.
		 * These must never be removed, as most engines support them.
		 */

		 /** The texture is combined with the result of the diffuse
		  *  lighting equation.
		  *  OR
		  *  PBR Specular/Glossiness
		  */
		  TextureType_DIFFUSE = 1,

		  /** The texture is combined with the result of the specular
		   *  lighting equation.
		   *  OR
		   *  PBR Specular/Glossiness
		   */
		   TextureType_SPECULAR = 2,

		   /** The texture is combined with the result of the ambient
			*  lighting equation.
			*/
			TextureType_AMBIENT = 3,

			/** The texture is added to the result of the lighting
			 *  calculation. It isn't influenced by incoming light.
			 */
			 TextureType_EMISSIVE = 4,

			 /** The texture is a height map.
			  *
			  *  By convention, higher gray-scale values stand for
			  *  higher elevations from the base height.
			  */
			  TextureType_HEIGHT = 5,

			  /** The texture is a (tangent space) normal-map.
			   *
			   *  Again, there are several conventions for tangent-space
			   *  normal maps. Assimp does (intentionally) not
			   *  distinguish here.
			   */
			   TextureType_NORMALS = 6,

			   /** The texture defines the glossiness of the material.
				*
				*  The glossiness is in fact the exponent of the specular
				*  (phong) lighting equation. Usually there is a conversion
				*  function defined to map the linear color values in the
				*  texture to a suitable exponent. Have fun.
			   */
			   TextureType_SHININESS = 7,

			   /** The texture defines per-pixel opacity.
				*
				*  Usually 'white' means opaque and 'black' means
				*  'transparency'. Or quite the opposite. Have fun.
			   */
			   TextureType_OPACITY = 8,

			   /** Displacement texture
				*
				*  The exact purpose and format is application-dependent.
				*  Higher color values stand for higher vertex displacements.
			   */
			   TextureType_DISPLACEMENT = 9,

			   /** Lightmap texture (aka Ambient Occlusion)
				*
				*  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
				*  covered by this material property. The texture contains a
				*  scaling value for the final color value of a pixel. Its
				*  intensity is not affected by incoming light.
			   */
			   TextureType_LIGHTMAP = 10,

			   /** Reflection texture
				*
				* Contains the color of a perfect mirror reflection.
				* Rarely used, almost never for real-time applications.
			   */
			   TextureType_REFLECTION = 11,

			   /** PBR Materials
				* PBR definitions from maya and other modelling packages now use this standard.
				* This was originally introduced around 2012.
				* Support for this is in game engines like Godot, Unreal or Unity3D.
				* Modelling packages which use this are very common now.
				*/

				TextureType_BASE_COLOR = 12,
				TextureType_NORMAL_CAMERA = 13,
				TextureType_EMISSION_COLOR = 14,
				TextureType_METALNESS = 15,
				TextureType_DIFFUSE_ROUGHNESS = 16,
				TextureType_AMBIENT_OCCLUSION = 17,

				/** PBR Material Modifiers
				* Some modern renderers have further PBR modifiers that may be overlaid
				* on top of the 'base' PBR materials for additional realism.
				* These use multiple texture maps, so only the base type is directly defined
				*/

				/** Sheen
				* Generally used to simulate textiles that are covered in a layer of microfibers
				* eg velvet
				* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_sheen
				*/
				TextureType_SHEEN = 19,

				/** Clearcoat
				* Simulates a layer of 'polish' or 'lacquer' layered on top of a PBR substrate
				* https://autodesk.github.io/standard-surface/#closures/coating
				* https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_materials_clearcoat
				*/
				TextureType_CLEARCOAT = 20,

				/** Transmission
				* Simulates transmission through the surface
				* May include further information such as wall thickness
				*/
				TextureType_TRANSMISSION = 21,

				/** Unknown texture
				 *
				 *  A texture reference that does not match any of the definitions
				 *  above is considered to be 'unknown'. It is still imported,
				 *  but is excluded from any further post-processing.
				*/
				TextureType_UNKNOWN = 18,

#ifndef SWIG
				_TextureType_Force32Bit = INT_MAX
#endif
	};

#define TEXTURE_TYPE_MAX TextureType_TRANSMISSION
}


