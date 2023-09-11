#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	}GRAPHIC_DESC;

	typedef struct ENGINE_DLL tagVertex_Position_Texcoord
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexcoord;

		static const unsigned int				iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];

	}VTXPOSTEX;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END };

		TYPE			eType;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		/* �л걤 */
		/* ������ ���� ��Ⱑ �� ����(���) -> ������Ʈ ����('�ڻ���') */
		XMFLOAT4		vDiffuse;

		/* ȯ��, �ֺ��� (�¾�)*/
		/* ������ �ݻ縦 ���� ������ �Һи���, '������(����)' ���� ������ ǥ���ȴ�. */
		XMFLOAT4		vAmbient;

		/* '�ݻ�'��, ���̶���Ʈ */
		/* �� �������� ������ �ݻ�Ǵ� �� -> �� ���� */
		XMFLOAT4		vSpecular;

		/* Emissive */
		/* �ܰ��� ����� ��� �ַ� ������Ʈ�� �θ� */
		/* �븻�� Eye�� ���� Ŭ����(90���� ��������) ������ ������*/

	}LIGHT_DESC;

}


#endif // Engine_Struct_h__
