#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };	// 마우스 버튼 타입

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };	// 마우스 좌표 타입, z는 스크롤

	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;

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

}

#endif // Engine_Typedef_h__
