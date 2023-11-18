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

	enum class LERP_MODE { DEFAULT, EASE_OUT, EASE_IN, EXPONENTIAL, SMOOTH_STEP, SMOOTHER_STEP, TYPEEND };
	typedef struct tagLerpFloatDesc
	{
		/* Time */
		_double		fStartTime = 0.f;
		_double		fEndTime = 0.f;
		_double		fCurTime = 0.f;

		/* Value */
		_double		fStartValue = 0.f;
		_double		fTargetValue = 0.f;
		_double		fCurValue = 0.f;

		_bool		bActive = FALSE;

		LERP_MODE	eMode = LERP_MODE::DEFAULT; // https://chicounity3d.wordpress.com/2014/05/23/how-to-lerp-like-a-pro/

		void Start(const _double _fStartValue, const _double _fTargetValue, const _double _fTime, const LERP_MODE _eMode = LERP_MODE::DEFAULT)
		{
			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			fStartValue = fCurValue = _fStartValue;
			fTargetValue = _fTargetValue;

			eMode = _eMode;
		}
		void Clear()
		{
			bActive = FALSE;

			fStartTime = fCurTime =  fEndTime = 0.f;

			fStartValue = fCurValue =  fTargetValue = 0.f;

			eMode = LERP_MODE::DEFAULT;
		}

		// Mode
		void Update(const _double& fTimeDelta)
		{
			if (!bActive || 0.02f < fTimeDelta) return;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				fCurValue = fTargetValue;
				bActive = FALSE;
				return;
			}

			_double t = fCurTime / fEndTime;

			switch (eMode)
			{
			case LERP_MODE::DEFAULT:
			{
			}
			break;
			case LERP_MODE::EASE_OUT:
			{
				t = sinf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				t = 1.f - cosf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				t = t * t;
			}
			break;
			case LERP_MODE::SMOOTH_STEP:
			{
				t = t * t * (3.f - 2.f * t);

			}
			break;
			case LERP_MODE::SMOOTHER_STEP:
			{
				t = t * t * t * (t * (6.f * t - 15.f) + 10.f);
			}
			break;
			default:
				break;
			}
			
			fCurValue = Lerp_Float(fStartValue, fTargetValue, t);

		}

		_double Lerp_Float(const _double& _f1, const _double& _f2, const _double _fTime) { return (1 - _fTime) * _f1 + (_fTime * _f2); }

	}LERP_FLOAT_DESC;

	typedef struct tagLerpTimeDesc
	{
		/* Time */
		_double		fStartTime = 0.f;
		_double		fEndTime = 0.f;
		_double		fCurTime = 0.f;
		_double		fLerpTime = 0.f;

		_bool		bActive = FALSE;

		LERP_MODE	eMode = LERP_MODE::DEFAULT; // https://chicounity3d.wordpress.com/2014/05/23/how-to-lerp-like-a-pro/

		void Start(const _double _fTime, const LERP_MODE _eMode = LERP_MODE::DEFAULT)
		{
			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;

			eMode = _eMode;
		}

		// Mode
		void Update(const _double& fTimeDelta)
		{
			if (!bActive || 0.02f < fTimeDelta) return;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				fCurTime = fEndTime;
				bActive = FALSE;
				fLerpTime = 1.f;
				return;
			}

			_double t = fCurTime / fEndTime;

			switch (eMode)
			{
			case LERP_MODE::DEFAULT:
			{
			}
			break;
			case LERP_MODE::EASE_OUT:
			{
				t = sinf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				t = 1.f - cosf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				t = t * t;
			}
			break;
			case LERP_MODE::SMOOTH_STEP:
			{
				t = t * t * (3.f - 2.f * t);

			}
			break;
			case LERP_MODE::SMOOTHER_STEP:
			{
				t = t * t * t * (t * (6.f * t - 15.f) + 10.f);
			}
			break;
			default:
				break;
			}

			fLerpTime = t;
		}
	}LERP_TIME_DESC;

	typedef struct tagLerpVec3Desc
	{
		_double		fStartTime	= 0.f;
		_double		fEndTime	= 0.f;
		_double		fCurTime	= 0.f;

		Vec3		vStartVec;
		Vec3		vTargetVec;
		Vec3		vCurVec;

		_bool		bActive = FALSE;

		LERP_MODE	eMode = LERP_MODE::DEFAULT;

		void Start(const Vec3 _fStartValue, const Vec3& _fTargetValue, const _double& _fTime, const LERP_MODE& _eMode = LERP_MODE::DEFAULT)
		{
			bActive = TRUE;

			fCurTime = 0.f;
			fEndTime = _fTime;
			
			vStartVec = vCurVec = _fStartValue;
			vTargetVec = _fTargetValue;

			eMode = _eMode;
		}

		void Update_Lerp(const _double& fTimeDelta)
		{
			if (!bActive || 0.02f < fTimeDelta) return;

			fCurTime += fTimeDelta;

			if (fCurTime >= fEndTime)
			{
				vCurVec = vTargetVec;
				return;
			}

			_double t = fCurTime / fEndTime;

			switch (eMode)
			{
			case LERP_MODE::DEFAULT:
			{
			}
			break;
			case LERP_MODE::EASE_OUT:
			{
				t = sinf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EASE_IN:
			{
				t = 1.f - cosf(t * XM_PI * 0.5f);
			}
			break;
			case LERP_MODE::EXPONENTIAL:
			{
				t = t * t;
			}
			break;
			case LERP_MODE::SMOOTH_STEP:
			{
				t = t * t * (3.f - 2.f * t);
			}
			break;
			case LERP_MODE::SMOOTHER_STEP:
			{
				t = t * t * t * (t * (6.f * t - 15.f) + 10.f);
			}
			break;
			}

			vCurVec = Vec3::Lerp(vStartVec, vTargetVec, t);
		}

		void Reset()
		{
			bActive = FALSE;

			fStartTime = fEndTime = fCurTime = 0;
			vStartVec = vTargetVec = vCurVec = Vec3::Zero;
		}
	}LERP_VEC3_DESC;
}

#endif // Engine_Struct_h__
