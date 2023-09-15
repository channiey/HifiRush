#include "Engine_Defines.h"
#include "Component.h"

namespace Engine
{
	const D3D11_INPUT_ELEMENT_DESC VTXPOSTEX::Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, /* 위치벡터에 대한 묘사. */
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	/* 텍스쿠드 벡터에 대한 묘사. */
	};

	const D3D11_INPUT_ELEMENT_DESC VTXNORTEX::Elements[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, /* 위치벡터에 대한 묘사. */
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }, /* 노말벡터에 대한 묘사. */
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },	/* 텍스쿠드 벡터에 대한 묘사. */
	};
}
