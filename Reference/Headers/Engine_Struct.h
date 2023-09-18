#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Base.h"

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END };

		TYPE			eType;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		/* 분산광 */
		/* 각도에 따라 밝기가 다 르다(명암) -> 람베르트 공식('코사인') */
		XMFLOAT4		vDiffuse;

		/* 환경, 주변광 (태양)*/
		/* 수많은 반사를 거쳐 광원이 불분명함, '일정한(같은)' 밝기와 색으로 표현된다. */
		XMFLOAT4		vAmbient;

		/* '반사'광, 하이라이트 */
		/* 한 방향으로 완전히 반사되는 빛 -> 퐁 공식 */
		XMFLOAT4		vSpecular;

		/* Emissive */
		/* 외곽선 추출시 사용 주로 림라이트라 부름 */
		/* 노말과 Eye의 각이 클수록(90도에 가까울수록) 색상이 강해짐*/

	}LIGHT_DESC;

	typedef struct tagProfilerDesc
	{
		/* https://shkim0811.tistory.com/42 */

		_float fFPS = 0.f;
		_uint iTris = 0;
		_uint iBatches = 0;
		_uint iSetPassCall = 0;
		_uint iAnim = 0;

	}PROFILER_DESC;

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
}

#endif // Engine_Struct_h__
