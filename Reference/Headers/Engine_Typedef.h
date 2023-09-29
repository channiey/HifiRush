#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;

	typedef unsigned char					_uchar;
	typedef		char						_char;
	typedef		wchar_t						_tchar;

	typedef		signed short				_short;
	typedef		unsigned short				_ushort;

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;

	typedef		float						_float;
	typedef		double						_double;

	using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;

	typedef		XMFLOAT2					_float2;
	typedef		XMFLOAT3					_float3;
	typedef		XMFLOAT4					_float4;

	typedef		XMVECTOR					_vector;
	typedef		FXMVECTOR					_fvector;
	typedef		GXMVECTOR					_gvector;
	typedef		HXMVECTOR					_hvector;
	typedef		CXMVECTOR					_cvector;

	typedef		XMFLOAT4X4					_float4x4;

	typedef		FXMMATRIX					_fmatrix;
	typedef		XMMATRIX					_matrix;


    enum aiTextureType {
        
        aiTextureType_NONE = 0,
        aiTextureType_DIFFUSE = 1,
        aiTextureType_SPECULAR = 2,
        aiTextureType_AMBIENT = 3,
        aiTextureType_EMISSIVE = 4,
        aiTextureType_HEIGHT = 5,
        aiTextureType_NORMALS = 6,
        aiTextureType_SHININESS = 7,
        aiTextureType_OPACITY = 8,
        aiTextureType_DISPLACEMENT = 9,
        aiTextureType_LIGHTMAP = 10,
        aiTextureType_REFLECTION = 11,
        aiTextureType_BASE_COLOR = 12,
        aiTextureType_NORMAL_CAMERA = 13,
        aiTextureType_EMISSION_COLOR = 14,
        aiTextureType_METALNESS = 15,
        aiTextureType_DIFFUSE_ROUGHNESS = 16,
        aiTextureType_AMBIENT_OCCLUSION = 17,
        aiTextureType_SHEEN = 19,
        aiTextureType_CLEARCOAT = 20,
		aiTextureType_TRANSMISSION = 21,
		aiTextureType_UNKNOWN = 18,
    };

#define AI_TEXTURE_TYPE_MAX aiTextureType_TRANSMISSION


}

#endif // Engine_Typedef_h__
