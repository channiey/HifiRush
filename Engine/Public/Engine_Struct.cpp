#include "Engine_Defines.h"
#include "Component.h"

namespace Engine
{
	const D3D11_INPUT_ELEMENT_DESC VTXPOSTEX::Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, /* À§Ä¡º¤ÅÍ¿¡ ´ëÇÑ ¹¦»ç. */
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	/* ÅØ½ºÄíµå º¤ÅÍ¿¡ ´ëÇÑ ¹¦»ç. */
	};
}


class CMonoBehaviour :
	public CComponent
{
};
