#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Base.h"

#include "SimpleMath.h"
namespace Engine
{
	typedef struct tagRayCastHitDesc
	{
		Vec3			vHitPoint = Vec3::Zero;
		class CGameObject* pGameObject = { nullptr };
		_float			fDistance = 0.f;

	}RAYHIT_DESC;

	typedef struct tagLightDesc
	{
		enum TYPE { LIGHT_DIRECTIONAL, LIGHT_POINT, LIGHT_END };

		TYPE		eLightType = { LIGHT_END };

		XMFLOAT4	vLightPos;
		float		fLightRange;

		XMFLOAT4	vLightDir;

		XMFLOAT4	vDiffuse;
		XMFLOAT4	vAmbient;
		XMFLOAT4	vSpecular;
	}LIGHT_DESC;

	typedef struct tagKeyFrame
	{
		float		fTime;

		XMFLOAT3	vScale;
		XMFLOAT4	vRotation;
		XMFLOAT3	vPosition;

	}KEYFRAME;

	typedef struct tagMaterialDesc
	{
		/* 매태리얼은 색상을 표현할 정보를 나타낸다. 매태리얼 = 택스처 집합소 */
		/* 하나의 매태리얼은 디퓨즈 텍스처, 노말 텍스처, 스페큘러 텍스처등 */
		/* 여러 개의 텍스처를 갖고 있다. */

		class CTexture* pTexture[AI_TEXTURE_TYPE_MAX];

	}MATERIALDESC;

	typedef struct tagProfilerDesc
	{
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

	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3		vPosition;

		static const unsigned int				iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTXPOS;

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

	typedef struct ENGINE_DLL tagVertex_Position_Cube_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexcoord;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXCUBE;

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
		Vec3		vPosition;
		Vec3		vNormal;
		Vec2		vTexture;
		Vec3		vTangent;
		XMUINT4		vBlendIndex; 
		Vec4		vBlendWeight;
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXMESH;

	typedef struct ENGINE_DLL tagVertex_Animation_Mesh
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		/* 이 정점이 상태를 받아와야할 뼈들의 인덱스 */
		XMUINT4			vBlendIndices;
		XMFLOAT4		vBlendWeights;

		static const unsigned int				iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXANIMMESH;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;
}

#endif // Engine_Struct_h__
