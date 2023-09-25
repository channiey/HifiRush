#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Base.h"

namespace Engine
{
	typedef struct tagKeyFrame
	{
		float		fTime;

		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;

	}KEYFRAME;

	typedef struct tagMaterialDesc
	{
		class CTexture* pTexture[AI_TEXTURE_TYPE_MAX];

	}MATERIALDESC;

	typedef struct tagProfilerDesc
	{
		/* https://shkim0811.tistory.com/42 */

		_float fFPS = 0.f;
		_uint iTris = 0;
		_uint iBatches = 0;
		_uint iSetPassCall = 0;
		_uint iAnim = 0;

	}PROFILER_DESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;

	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;

	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;

	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;

	}FACEINDICES32;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int				iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXNORTEX;

	typedef struct tagVertexModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXMODEL_DECLARATION;

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexture;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		XMFLOAT4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;
}

#endif // Engine_Struct_h__
